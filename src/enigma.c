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

/**************************************************************************************************\
* 
* Converts text to the input array of unified positions
* 
\**************************************************************************************************/
void setText(Enigma* engima, char text[])
{
    int  i;
    int  count;
    char digit;
    
    // to upper case, remove non chars and get position
    i=0;
    count=0;
    while (i<strlen(text))
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
            while (notch<enigma->numberOfNotches[rotor])
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
* Advance the rotors the indicated number of steps
* 
\**************************************************************************************************/
void advances(Enigma* enigma, int steps)
{
    int i;
    
    i=0;
    while (i<steps)
    {
        advance(enigma);
        i++;
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
int countConvertedChar(Enigma* enigma, char letter)
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
    
    count=0;
    i=0;
    while (i<enigma->textSize-2)
    {
        if ((enigma->conversion[i  ]==trigram[0]-'A') &&
            (enigma->conversion[i+1]==trigram[1]-'A') &&
            (enigma->conversion[i+2]==trigram[2]-'A'))
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

void dumpDecoded(EnigmaSettings* settings)
{
    Enigma* enigma;
    
    enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    printf("%s", toString(enigma));
    destroyEnigma(enigma);
}