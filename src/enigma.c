/**************************************************************************************************\
* enigma.c
* 
* Implements the engima encoding/decoding. Enhanced for performance. All letters  and positions 
* (A-Z, a-z, 1-26) are converted to unified positions (0-25).
* 
* 
\**************************************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "enigma.h"
#include "toolbox.h"

#define ROTATING_WALTZEN 3

#define min(a,b) (a<b?a:b)

EnigmaSettings settings;

/**************************************************************************************************\
* 
* Converts text to the input array of unified positions
* 
\**************************************************************************************************/
void setText(Enigma* engima, char text[])
{
    int  i;
    int  count;
    int  maxCount;
    char digit;
    
    // to upper case, remove non chars and get position
    i=0;
    count=0;
    maxCount=min(strlen(text), MAX_TEXT);
    while (i<maxCount)
    {
        digit=charToPos(text[i]);
        if (digit>=0 && digit<MAX_POSITIONS)
        {
            engima->text[count]=digit;
            count++;
        }
        i++;
    }
    engima->textSize=count;
}


/**************************************************************************************************\
* 
* Advance the rotors. This is done prior to converting one character
* 
\**************************************************************************************************/
void advance(Enigma* enigma)
{
    int rotor;
    int revolveNext;        
    int notch;
  
    // Advance rotors
    rotor           =0;
    revolveNext     =1;
    // Only the 1st three rotors advance
    while (rotor<ROTATING_WALTZEN && revolveNext)
    {
        revolveNext=0;
        notch=0;
        while (notch<enigma->numberOfNotches[rotor])
        {
            if (enigma->grundStellung[rotor]==enigma->notches[rotor][notch])
            {
                revolveNext=1;
            }
            notch++;
        }
        
        // The double step of the 2nd rotor
        if (rotor==0)
        {
            notch=0;
            while (notch<enigma->numberOfNotches[rotor+1])
            {
                if (enigma->grundStellung[rotor+1]==enigma->notches[rotor+1][notch])
                {
                    revolveNext=1;
                }
                notch++;
            }
        }
        
        enigma->grundStellung[rotor]++;
        if (enigma->grundStellung[rotor]==MAX_POSITIONS)
        {
            enigma->grundStellung[rotor]=0;
        }
        
        rotor++;
    }
}

/**************************************************************************************************\
* 
* Reverse the rotors. A real Enigma cannot do this :-)
* enigma    : enigma to use
* 
\**************************************************************************************************/
void reverse(Enigma* enigma)
{
    int rotor;
    int test;
    int reverseNext;        
    int notch;
    int found;
    
    // Reverse rotors
    rotor           =0;
    reverseNext     =1;
    // Only the 1st three rotors advance
    while (rotor<ROTATING_WALTZEN && reverseNext)
    {
        reverseNext=0;
        enigma->grundStellung[rotor]--;
        if (enigma->grundStellung[rotor]<0)
        {
            enigma->grundStellung[rotor]=MAX_POSITIONS-1;
        }

        notch=0;
        while (notch<enigma->numberOfNotches[rotor])
        {
            if (enigma->grundStellung[rotor]==enigma->notches[rotor][notch])
            {
                reverseNext=1;
            }
            notch++;
        }
        
        // The double step of the 2nd rotor
        if (rotor==0)
        {
            test=enigma->grundStellung[rotor]-1;
            if (test<0)
            {
                test=MAX_POSITIONS-1;
            }            
            notch=0;
            found=0;
            while (notch<enigma->numberOfNotches[rotor])
            {
                if (test==enigma->notches[rotor][notch])
                {
                    found=1;
                }
                notch++;
            }

            if (found)
            {
                test=enigma->grundStellung[rotor+1]-1;
                if (test<0)
                {
                    test=MAX_POSITIONS-1;
                }
                notch=0;
                while (notch<enigma->numberOfNotches[rotor+1])
                {
                    if (test==enigma->notches[rotor+1][notch])
                    {
                        reverseNext=1;
                    }
                    notch++;
                }
            }
        }
        rotor++;
    }
}

/**************************************************************************************************\
* 
* Advance the rotor the indicated number of steps; all slower rotating rotors are advances when
* needed.
* enigma: enigma to use 
* steps : number of steps to advance; this may be positive or negative!!
* 
\**************************************************************************************************/
void advances(Enigma* enigma, int steps)
{
    int i;
    
    if (steps>0)
    {
        for(i = 0; i < steps; i++)
        {
            advance(enigma);
        }
    }
    else
    {
        for(i = 0; i < -steps; i++)
        {
            reverse(enigma);
        }        
    }
}

/**************************************************************************************************\
* 
* Encode character
* 
\**************************************************************************************************/

int encodeCharacter(Enigma* enigma, int theCharacter)
{
    int intermediate;
    int rotor;
    int realPosition;
    int entry;
    int exit;
    
    // Via switchboard
    intermediate=enigma->steckerBrett[theCharacter];
    
    // right to left through the rotors
    rotor=0;
    while (rotor<enigma->numberOfRotors)
    {
        realPosition=enigma->grundStellung[rotor]-enigma->ringStellung[rotor];
        
        if (realPosition<0)
        {
            realPosition+=MAX_POSITIONS;
        }
        entry=realPosition+intermediate;
        if (entry>=MAX_POSITIONS)
        {
            entry-=MAX_POSITIONS;
        }
        exit=enigma->rotorFunction[rotor][entry];
        
        intermediate=exit-realPosition;
        
        if (intermediate<0)
        {
            intermediate+=MAX_POSITIONS;
        }
        rotor++;
    }
    
    // Reflector
    intermediate=enigma->umkehrWaltzeFunction[intermediate];
   
    // left to right through the rotors
    rotor=enigma->numberOfRotors-1;
    while (rotor>=0)
    {
        realPosition=enigma->grundStellung[rotor]-enigma->ringStellung[rotor];
        
        if (realPosition<0)
        {
            realPosition+=MAX_POSITIONS;
        }
        entry=realPosition+intermediate;
        if (entry>=MAX_POSITIONS)
        {
            entry-=MAX_POSITIONS;
        }
        exit=enigma->rotorInverseFunction[rotor][entry];
        
        intermediate=exit-realPosition;
        
        if (intermediate<0)
        {
            intermediate+=MAX_POSITIONS;
        }
        
        rotor--;
    }       

    // Via switchboard
    intermediate=enigma->steckerBrett[intermediate];


    return intermediate;
}


/**************************************************************************************************\
* 
* The function to encode or decode the text
* 
\**************************************************************************************************/
void encodeDecode(Enigma* enigma)
{
    int charIndex;
    
    charIndex=0;
    while (charIndex<enigma->textSize)
    {
        advance(enigma);

        enigma->conversion[charIndex]=encodeCharacter(enigma, enigma->text[charIndex]);
        
        charIndex++;
    }
}

/**************************************************************************************************\
* 
* Converts the encode/decode result to a char[] string
* 
\**************************************************************************************************/
char* toString(Enigma* enigma)
{
    int i;
    i=0;
    while (i<enigma->textSize)
    {
        enigma->string[i]=enigma->conversion[i]+'A';
        i++;
    }
    enigma->string[i]='\0';
    return enigma->string;
}

/**************************************************************************************************\
* 
* Count occurences of specified letter
* 
\**************************************************************************************************/
int countLetter(Enigma* enigma, char letter)
{
    int i;
    int max;
    int count;
    int letterPos;
    
    letterPos=charToPos(letter);
    
    count=0;
    max=enigma->textSize;
    i=0;
    while (i<max)
    {
        if (enigma->conversion[i]==letterPos)
        {
            count++;
        }
        i++;
    }
    return count;
}


/**************************************************************************************************\
* 
* Count occurences of specified letter, where the letter is passed as 0-25
* 
\**************************************************************************************************/
int countConvertedChar(Enigma* enigma, int letter)
{
    int i;
    int max;
    int count;
    
    count=0;
    max=enigma->textSize;
    i=0;
    while (i<max)
    {
        if (enigma->conversion[i]==letter)
        {
            count++;
        }
        i++;
    }
    return count;
}

/**************************************************************************************************\
* 
* Count occurences of specified trigram, where the trigram is passed as string, like "EIN"
* 
\**************************************************************************************************/
int countTrigram(Enigma* enigma, char* trigram)
{
    int i;
    int count;
    int tri0, tri1, tri2;
    
    tri0=trigram[0]-'A';
    tri1=trigram[1]-'A';
    tri2=trigram[2]-'A';
    
    count=0;
    i=0;
    while (i<enigma->textSize-2)
    {
        if ((enigma->conversion[i  ]==tri0) &&
            (enigma->conversion[i+1]==tri1) &&
            (enigma->conversion[i+2]==tri2))
        {
            count++;
        }
        i++;
    }
    
    return count;
}

/**************************************************************************************************\
* 
* Count occurences of specified sequence
* 
\**************************************************************************************************/
int countNgram(Enigma* enigma, char* ngram, int n)
{
    int i;
    int j;
    int found;
    int count;
    
    count=0;
    i=0;
    while (i<enigma->textSize-(n-1))
    {
        j=0;
        found=1;
        while ((j<n) && found)
        {
            if (enigma->conversion[i+j]!=ngram[j]-'A')
            {
                found=0;
            }
            j++;
        }
        if (found)
        {
            count++;
        }
        
        i++;
    }
    
    return count;
}



/**************************************************************************************************\
* 
* Set the Enigma key based on settings array
* 
\**************************************************************************************************/

void setEnigma(Enigma* enigma, EnigmaSettings* settings)
{
    int i;
    
    enigma->numberOfRotors=settings->numberOfRotors;
    i=0;
    while (i<settings->numberOfRotors)
    {
        placeWaltze         (enigma, i+1, settings->rotors[i]);
        setRingStellung     (enigma, i+1, settings->ringStellungen[i]);
        setGrundStellung    (enigma, i+1, settings->grundStellungen[i]);
        i++;
    }
    placeUmkehrWaltze       (enigma, settings->ukw);
    if (strncmp(settings->steckers, "", MAX_STECKER_STRING)==0)
    {
        clearSteckerBrett(enigma);
    }
    else
    {
        placeSteckers           (enigma, settings->steckers);
    }
    setText(enigma, settings->cypher);
}


/**************************************************************************************************\
* 
* Print the engima settings
* 
\**************************************************************************************************/

void printEnigmaSettings(EnigmaSettings* settings, char* title)
{
    printf("\n");
    printf("#####################################################################################\n");
    printf("# %s\n", title);
    printf("# Cypher                    : \n");
    printCypher(settings->cypher);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], settings->rotors[1], settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");
}

/**************************************************************************************************\
* 
* Decode and print solution
* 
\**************************************************************************************************/

void dumpDecoded(EnigmaSettings* settings)
{
    Enigma* enigma;
    
    enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    printf("Solution: %s\n", toString(enigma));
    destroyEnigma(enigma);
}