/**************************************************************************************************\
* enigma.c
* 
* Implements the engima encoding/decoding. Enhanced for performance. All letters  and positions 
* (A-Z, a-z, 1-26) are converted to unified positions (0-25).
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "enigma.h"
#include "toolbox.h"

#define ROTATING_WALZEN 3

#define min(a,b) (a<b?a:b)

EnigmaSettings settings;

/**************************************************************************************************\
* 
* Sets the text to convert. Converts text to the input array of unified positions.
* enigma: Enigma to use
* text  : input text consisting of a-zA-Z or 1-26, any other char is omitted
* 
\**************************************************************************************************/
void setText(Enigma* engima, char text[])
{
    // to upper case, remove non chars and get position
    int count=0;
    int maxCount=min(strlen(text), MAX_TEXT);
    for (int i=0; i<maxCount; i++)
    {
        char digit=charToPos(text[i]);
        if (digit>=0 && digit<MAX_POSITIONS)
        {
            engima->text[count]=digit;
            count++;
        }
    }
    engima->textSize=count;
}

/**************************************************************************************************\
* 
* Advance the rotors. This is done prior to converting one character
* enigma    : Enigma to use
* 
\**************************************************************************************************/
void advance(Enigma* enigma)
{
    // Advance rotors
    int revolveNext=1;
    // Only the 1st three rotors advance
    for (int rotor=0; rotor<ROTATING_WALZEN && revolveNext; rotor++)
    {
        revolveNext=0;
        for (int notch=0; notch<enigma->numberOfNotches[rotor]; notch++)
        {
            if (enigma->grundStellung[rotor]==enigma->notches[rotor][notch])
            {
                revolveNext=1;
            }
        }
        
        // The double step of the 2nd rotor
        if (rotor==0)
        {
            for (int notch=0; notch<enigma->numberOfNotches[rotor+1]; notch++)
            {
                if (enigma->grundStellung[rotor+1]==enigma->notches[rotor+1][notch])
                {
                    revolveNext=1;
                }
            }
        }
        
        enigma->grundStellung[rotor]++;
        if (enigma->grundStellung[rotor]==MAX_POSITIONS)
        {
            enigma->grundStellung[rotor]=0;
        }
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
    // Reverse rotors
    int reverseNext     =1;
    // Only the 1st three rotors advance
    for (int rotor=0; rotor<ROTATING_WALZEN && reverseNext; rotor++)
    {
        reverseNext=0;
        enigma->grundStellung[rotor]--;
        if (enigma->grundStellung[rotor]<0)
        {
            enigma->grundStellung[rotor]=MAX_POSITIONS-1;
        }

        for (int notch=0; notch<enigma->numberOfNotches[rotor]; notch++)
        {
            if (enigma->grundStellung[rotor]==enigma->notches[rotor][notch])
            {
                reverseNext=1;
            }
        }
        
        // The double step of the 2nd rotor
        if (rotor==0)
        {
            int test=enigma->grundStellung[rotor]-1;
            if (test<0)
            {
                test=MAX_POSITIONS-1;
            }            
            bool found=false;
            for (int notch=0; notch<enigma->numberOfNotches[rotor]; notch++)
            {
                if (test==enigma->notches[rotor][notch])
                {
                    found=true;
                }
            }

            if (found)
            {
                test=enigma->grundStellung[rotor+1]-1;
                if (test<0)
                {
                    test=MAX_POSITIONS-1;
                }
                for (int notch=0; notch<enigma->numberOfNotches[rotor+1]; notch++)
                {
                    if (test==enigma->notches[rotor+1][notch])
                    {
                        reverseNext=1;
                    }
                }
            }
        }
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
    if (steps>0)
    {
        for(int i = 0; i < steps; i++)
        {
            advance(enigma);
        }
    }
    else
    {
        for(int i = 0; i < -steps; i++)
        {
            reverse(enigma);
        }        
    }
}

/**************************************************************************************************\
* 
* Encode character
* enigma      : Enigma to use
* theCharacter: The character to encode, 0-25
* returns     : The encoded character, 0-25
* 
\**************************************************************************************************/

int encodeCharacter(Enigma* enigma, int theCharacter)
{
    // Via switchboard
    int intermediate=enigma->steckerBrett[theCharacter];
    
    // right to left through the rotors
    for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
    {
        int realPosition=enigma->grundStellung[rotor]-enigma->ringStellung[rotor];
        
        if (realPosition<0)
        {
            realPosition+=MAX_POSITIONS;
        }
        int entry=realPosition+intermediate;
        if (entry>=MAX_POSITIONS)
        {
            entry-=MAX_POSITIONS;
        }
        int exit=enigma->rotorFunction[rotor][entry];
        
        intermediate=exit-realPosition;
        
        if (intermediate<0)
        {
            intermediate+=MAX_POSITIONS;
        }
    }
    
    // Reflector
    intermediate=enigma->umkehrWalzeFunction[intermediate];
   
    // left to right through the rotors
    for (int rotor= enigma->numberOfRotors-1; rotor>=0; rotor--)
    {
        int realPosition=enigma->grundStellung[rotor]-enigma->ringStellung[rotor];
        
        if (realPosition<0)
        {
            realPosition+=MAX_POSITIONS;
        }
        int entry=realPosition+intermediate;
        if (entry>=MAX_POSITIONS)
        {
            entry-=MAX_POSITIONS;
        }
        int exit=enigma->rotorInverseFunction[rotor][entry];
        
        intermediate=exit-realPosition;
        
        if (intermediate<0)
        {
            intermediate+=MAX_POSITIONS;
        }
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
    for (int charIndex=0; charIndex<enigma->textSize; charIndex++)
    {
        advance(enigma);
        enigma->conversion[charIndex]=encodeCharacter(enigma, enigma->text[charIndex]);
    }
}

/**************************************************************************************************\
* 
* Converts the encode/decode result to a char[] string
* 
\**************************************************************************************************/
char* toString(Enigma* enigma)
{
    for (int i=0; i<enigma->textSize; i++)
    {
        enigma->string[i]=enigma->conversion[i]+'A';
    }
    enigma->string[enigma->textSize]='\0';
    return enigma->string;
}

/**************************************************************************************************\
* 
* Count occurences of specified letter
* 
\**************************************************************************************************/
int countLetter(Enigma* enigma, char letter)
{
    int letterPos=charToPos(letter);
    int count=0;
    int max=enigma->textSize;
    for (int i=0; i<max; i++)
    {
        if (enigma->conversion[i]==letterPos)
        {
            count++;
        }
    }
    return count;
}

/**************************************************************************************************\
* 
* Count occurences of specified letter
* 
\**************************************************************************************************/
void countConvertedChars(Enigma* enigma, int counts[MAX_POSITIONS])
{
    int max     =enigma->textSize;
    for (int i=0; i<MAX_POSITIONS; i++)
    {
        counts[i]=0;
    }
    for (int i=0; i<max; i++)
    {
        counts[enigma->conversion[i]]++;
    }
}

/**************************************************************************************************\
* 
* Count occurences of specified trigram, where the trigram is passed as string, like "EIN"
* 
\**************************************************************************************************/
int countTrigram(Enigma* enigma, char* trigram)
{
    int tri0=trigram[0]-'A';
    int tri1=trigram[1]-'A';
    int tri2=trigram[2]-'A';
    
    int count=0;
    for (int i=0; i<enigma->textSize-2; i++)
    {
        if ((enigma->conversion[i  ]==tri0) &&
            (enigma->conversion[i+1]==tri1) &&
            (enigma->conversion[i+2]==tri2))
        {
            count++;
        }
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
    int count=0;
    for (int i=0; i<enigma->textSize-(n-1); i++)
    {
        bool found=true;
        for (int j=0; (j<n) && found; j++)
        {
            if (enigma->conversion[i+j]!=ngram[j]-'A')
            {
                found=false;
            }
        }
        if (found)
        {
            count++;
        }
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
    enigma->numberOfRotors=settings->numberOfRotors;
    for (int i=0; i<settings->numberOfRotors; i++)
    {
        placeWalze          (enigma, i+1, settings->rotors[i]);
        setRingStellung     (enigma, i+1, settings->ringStellungen[i]);
        setGrundStellung    (enigma, i+1, settings->grundStellungen[i]);
    }
    placeUmkehrWalze        (enigma, settings->ukw);
    if (strncmp(settings->steckers, "", MAX_STECKER_STRING)==0)
    {
        clearSteckerBrett(enigma);
    }
    else
    {
        placeSteckers           (enigma, settings->steckers);
    }
    setText(enigma, settings->cipher);
}

/**************************************************************************************************\
* 
* Print the engima settings
* 
\**************************************************************************************************/
void printEnigmaSettings(EnigmaSettings* settings, char* title)
{
    // Cipher size when given to Enigma
    printf("\n");
    printf("#####################################################################################\n");
    printf("# %s\n", title);
    printf("# Cipher/text               : \n");
    printCipher(settings->cipher);
    printf("# Cipher size               : %d characters\n", (int)strlen(settings->cipher));
    printf("# Number of Walzen          : %d\n", settings->numberOfRotors);
    printf("# Original Walzen           : ");
    printf("%s, ", settings->ukw);
    for (int i=0;i<settings->numberOfRotors;i++)
    {
        printf("%s", settings->rotors[i]);
        if (i<settings->numberOfRotors-1)
        {
            printf(", ");
        }
    }
    printf("\n");
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
* Create random settings for the Enigma
* enigma   : enigma to create settings for
* rotorSet : set of Walzen to use
* steckers : number of steckers to use
* 
\**************************************************************************************************/
EnigmaSettings* createRandomSettings(Enigma* enigma, RotorSet_t rotorSet, int numberOfSteckers)
{
    int offset;
    int indices[54];

    EnigmaSettings* random=malloc(sizeof(EnigmaSettings));

    if ((enigma->numberOfRotors==4 && rotorSet!=M4_NAVAL_1941) ||
        (enigma->numberOfRotors==3 && rotorSet==M4_NAVAL_1941))
    {
        printf("Illegal rotor set for given engima");
        exit(0);
    }
    // Number of rotors
    random->numberOfRotors=enigma->numberOfRotors;

    // 4th rotor selection (rotor 1, M4)
    if (enigma->numberOfRotors==4)
    {
        selectRandomIndices(fourthRotorSets[rotorSet], ROTORS, 1, indices);
        strncpy(random->rotors[0], rotorNames[indices[0]], MAX_ROTOR_NAME-1);
        offset=1;
    }
    else
    {
        offset=0;
    }

    // 1st 3 Rotor selection
    selectRandomIndices(rotorSets[rotorSet], ROTORS, 3, indices);
    for (int i=0;i<3; i++)
    {
        strncpy(random->rotors[i+offset], rotorNames[indices[i]], MAX_ROTOR_NAME-1);
    }

    // UKW
    selectRandomIndices(ukwSets[rotorSet], UMKEHR_WALZEN, 1, indices);
    strncpy(random->ukw, umkehrWalzeNames[indices[0]], MAX_ROTOR_NAME-1);

    // Grundstelling
    for (int i=0; i<random->numberOfRotors;i++)
    {
        random->grundStellungen[i]=1+rand() % 26;
    }

    // Ringstellung
    for (int i=0; i<random->numberOfRotors;i++)
    {
        random->ringStellungen[i]=1+rand() % 26;
    }

    // Steckers
    int available[26]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    selectRandomIndices(available, 26, numberOfSteckers*2, indices);

    for(int i=0; i<numberOfSteckers; i++)
    {
        random->steckers[i*3]  ='A'+indices[i*2];
        random->steckers[i*3+1]='A'+indices[i*2+1];
        if (i<numberOfSteckers-1)
        {
            random->steckers[i*3+2]=' ';
        }
        else
        {
            random->steckers[i*3+2]='\0';
        }
    }

    // Cipher - empty
    random->cipher[0]='\0';
    return random;
}

/**************************************************************************************************\
* 
* Clean up the settings
* 
\**************************************************************************************************/
void destroyEnigmaSettings(EnigmaSettings* settings)
{
    free(settings);
}

/**************************************************************************************************\
* 
* Decode and print solution
* 
\**************************************************************************************************/
void dumpDecoded(EnigmaSettings* settings)
{
    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    printf("Solution: %s\n", toString(enigma));
    destroyEnigma(enigma);
}


/**************************************************************************************************\
* 
* Display decoded text in more digestible format,
* 
\**************************************************************************************************/
// TO DO: improve
void displayEnigmaMessage(char* message, MessageFormat_t type)
{
    switch (type)
    {
        default:
        case MESSAGEFORMAT_TEXT:
            printf("%s", message);
            break;
        // X=. XX=: ZZ=, FRAGE=? FRAQ=? 
        case MESSAGEFORMAT_WEHRMACHT: 
            for (int i=0;i<strlen(message);i++)
            {
                char c=message[i];
                if (c=='X' || c=='x')
                {
                    if ((i<strlen(message)-1) && (message[i+1]=='X' || message[i+1]=='x'))
                    {
                        printf(": ");
                        i++;
                    }
                    else
                    {
                        printf(". ");
                    }
                }
                else if (c=='Q')
                {
                    printf("CH");
                }
                else if (c=='q')
                {
                    printf("ch");
                }
                else 
                {
                    printf("%c", c);
                }
            }
            break;
        // X=. Y=, UD=? Q=ch CENTA=00 MILLE=000 MYRIA=0000
        case MESSAGEFORMAT_KRIEGSMARINE:
            for (int i=0;i<strlen(message);i++)
            {
                char c=message[i];
                if (c=='X' || c=='x')
                {
                    printf(". ");
                }
                else if (c=='y' || c=='Y')
                {
                    printf(", ");
                }
                else 
                {
                    printf("%c", c);
                }
            }
            break;
    }
    printf("\n");
}

