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
* The function to encode or decode the text
* 
\**************************************************************************************************/
void encodeDecode(Enigma* enigma)
{
    int intermediate;
    int charIndex;
    int rotor;
    int realPosition;
    int entry;
    int exit;


    
    charIndex=0;
    while (charIndex<enigma->textSize)
    {
        advance(enigma);
        
        // Character to process
        intermediate=enigma->text[charIndex];
        
        // Via switchboard
        intermediate=enigma->steckerBrett[intermediate];
        
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

        enigma->conversion[charIndex]=intermediate;
        
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



