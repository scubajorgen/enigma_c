#include <stdio.h>
#include <string.h>

#include "enigma.h"

#define ROTORS          10
#define MAX_ROTORNAME   10

    char    rotorNames[ROTORS][MAX_ROTORNAME]=
    {
        "I",
        "II",
        "III",
        "IV",
        "V",
        "VI",
        "VII",
        "VIII",
        "Beta",
        "Gamma"
            
    };
    
    char    tables[ROTORS][MAX_POSITIONS]=
    {
        {
            'E','K','M','F','L','G','D','Q','V','Z','N','T','O','W','Y','H','X','U','S','P','A','I','B','R','C','J'
        },
        {
            'A','J','D','K','S','I','R','U','X','B','L','H','W','T','M','C','Q','G','Z','N','P','Y','F','V','O','E'
        },
        {
            'B','D','F','H','J','L','C','P','R','T','X','V','Z','N','Y','E','I','W','G','A','K','M','U','S','Q','O'
        },
        {
            'E','S','O','V','P','Z','J','A','Y','Q','U','I','R','H','X','L','N','F','T','G','K','D','C','M','W','B'
        },
        {
            'V','Z','B','R','G','I','T','Y','U','P','S','D','N','H','L','X','A','W','M','J','Q','O','F','E','C','K'
        },
        {
            'J','P','G','V','O','U','M','F','Y','Q','B','E','N','H','Z','R','D','K','A','S','X','L','I','C','T','W'
        },
        {
            'N','Z','J','H','G','R','C','X','M','Y','S','W','B','O','U','F','A','I','V','L','P','E','K','Q','D','T'
        },
        {
            'F','K','Q','H','T','L','X','O','C','B','J','S','P','D','Z','R','A','M','E','W','N','I','U','Y','G','V'
        },
        {
            'L','E','Y','J','V','C','N','I','X','W','P','B','Q','M','D','R','T','A','K','Z','G','F','U','H','O','S'
        },
        {
            'F','S','O','K','A','N','U','E','R','H','M','B','T','I','Y','C','W','L','Q','P','Z','X','V','G','J','D'
        }
    };

    int                 hasSecondNotch[]={0, 0, 0, 0, 0, 1, 1, 1, 0, 0};
    
    /* Notch positions as seen from the window */
    int                 notchPosition1[]={'Q','E','V','J','Z','Z','Z','Z','?','?'};
    int                 notchPosition2[]={'?','?','?','?','?','M','M','M','?','?'};


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void placeWaltze(Enigma* enigma, int waltze, char rotorName[])
{
    int     index;
    int     found;
    int     j;
    int     pos;
    
    pos=enigma->numberOfRotors-waltze;
    
    index=0;
    found=0;
    while (index<ROTORS && !found)
    {
        if (!strcmp(rotorName, rotorNames[index]))
        {
            found=1;
            j=0;
            while (j<MAX_POSITIONS)
            {
                enigma->rotorFunction[pos][j]                           =tables[index][j]-'A';
                enigma->rotorInverseFunction[pos][tables[index][j]-'A'] =j;
                j++;
            }
            enigma->notches[pos][0]=notchPosition1[index]-'A';
            if (hasSecondNotch[index])
            {
                enigma->notches[pos][1]=notchPosition2[index]-'A';
                enigma->numberOfNotches[pos]=2;
            }
            else
            {
                enigma->numberOfNotches[pos]=1;
            }
        }

        index++;
    }
    if (!found)
    {
        printf("ERROR: invalid waltze\n");
    }
}


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void setRingStellung(Enigma* enigma, int waltze, int ringStellung)
{
    int     pos;
    
    pos=enigma->numberOfRotors-waltze;
    
    enigma->ringStellung[pos]=stellungToPos(ringStellung);
}

/**************************************************************************************************\
* 
*  Sets the RingStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/

void setRingStellungen(Enigma* enigma, char* ringStellungen)
{
    int rotor;
    int numberOfRotors;
    int stellung;
    
    numberOfRotors=enigma->numberOfRotors;
    
    if (strlen(ringStellungen)==numberOfRotors*3-1)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            stellung=(ringStellungen[rotor*3]-'0')*10+(ringStellungen[rotor*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->ringStellung[numberOfRotors-rotor-1]=stellung-1;
            }
            else
            {
                printf("ERROR: Error during coversion of RingStellungen string");
            }    
            rotor++;
        }
    }
    else if (strlen(ringStellungen)==numberOfRotors*2-1)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            enigma->ringStellung[numberOfRotors-rotor-1]=stellungToPos(ringStellungen[rotor*2]);
            rotor++;
        }
    }
    else if (strlen(ringStellungen)==numberOfRotors)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            enigma->ringStellung[numberOfRotors-rotor-1]=stellungToPos(ringStellungen[rotor]);
            rotor++;
        }
    }
    else
    {
        printf("ERROR: unexpected string length when defining the ringstellungen\n");
    }    
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void setGrundStellung(Enigma* enigma, int waltze, int grundStellung)
{
    int     pos;
    
    pos=enigma->numberOfRotors-waltze;
    
    enigma->grundStellung[pos]=stellungToPos(grundStellung);
}

/**************************************************************************************************\
* 
*  Sets the GrundStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/

void setGrundStellungen(Enigma* enigma, char* grundStellungen)
{
    int rotor;
    int numberOfRotors;
    int stellung;
    
    numberOfRotors=enigma->numberOfRotors;
    
    if (strlen(grundStellungen)==numberOfRotors*3-1)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            stellung=(grundStellungen[rotor*3]-'0')*10+(grundStellungen[rotor*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->grundStellung[numberOfRotors-rotor-1]=stellung-1;
            }
            else
            {
                printf("ERROR: Error during coversion of GrundStellungen string");
            }    
            rotor++;
        }
    }
    else if (strlen(grundStellungen)==numberOfRotors*2-1)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            enigma->grundStellung[numberOfRotors-rotor-1]=stellungToPos(grundStellungen[rotor*2]);
            rotor++;
        }
    }
    else if (strlen(grundStellungen)==numberOfRotors)
    {
        rotor=0;
        while (rotor<enigma->numberOfRotors)
        {
            enigma->grundStellung[numberOfRotors-rotor-1]=stellungToPos(grundStellungen[rotor]);
            rotor++;
        }
    }
    else
    {
        printf("ERROR: unexpected string length when defining the GrundStellungen\n");
    }    
}