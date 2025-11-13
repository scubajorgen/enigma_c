/**************************************************************************************************\
*
* This file implements the functions and definitions of the Enigma Walze, or rotors
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "enigma.h"
#include "log.h"
#include "toolbox.h"

char    rotorNames[ROTORS][MAX_ROTOR_NAME]=
{
    "I",
    "II",
    "III",
    "IV",
    "V",
    "VI",
    "VII",
    "VIII",
    "Beta", // 4th rotor in M4
    "Gamma" // 4th roter in M4
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


int rotorSets[MAX_ROTOR_SETS][ROTORS]=
{
    { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}
};

int fourthRotorSets[MAX_ROTOR_SETS][ROTORS]=
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
};


/**************************************************************************************************\
* 
* Place the given rotor at given position
* enigima: enigma definition
* walze    : rotor position counted from left to right (1..4)
* rotorName: name of the rotor to place, like 'I', 'VI' 
* 
\**************************************************************************************************/
void placeWalze(Enigma* enigma, int walze, char rotorName[])
{
    int     pos     =enigma->numberOfRotors-walze;
    bool    found   =false;
    for (int index=0; index<ROTORS && !found; index++)
    {
        if (!strcmp(rotorName, rotorNames[index]))
        {
            found=true;
            
            if ((index==8 || index==9) && 
                (enigma->numberOfRotors<4 || (enigma->numberOfRotors==4 && walze!=1)))
            {
                logError("Beta and Gamma rotors are only allowed on position 1 of Enigma M4");
            }
            
            for (int j=0; j<MAX_POSITIONS; j++)
            {
                enigma->rotorFunction[pos][j]                           =tables[index][j]-'A';
                enigma->rotorInverseFunction[pos][tables[index][j]-'A'] =j;
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
    }
    if (!found)
    {
        logFatal("invalid walze");
    }
}


/**************************************************************************************************\
* 
* Set the Ringstellung of the Walze at given position
* enigima: enigma definition
* walze: rotor position counted from left to right (1..4)
* ringStellung: the Ringstellung. Supported: "12", "G" or "G" or "g"
*
\**************************************************************************************************/
void setRingStellung(Enigma* enigma, int walze, int ringStellung)
{
    int pos=enigma->numberOfRotors-walze;
    enigma->ringStellung[pos]=stellungToPos(ringStellung);
}

/**************************************************************************************************\
* 
* Return the Ringstellung of given Walze
* 
\**************************************************************************************************/
int getRingStellung(Enigma* enigma, int walze)
{
    int pos=enigma->numberOfRotors-walze;
    return posToStellung(enigma->ringStellung[pos]);
}


/**************************************************************************************************\
* 
*  Sets the RingStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/

void setRingStellungen(Enigma* enigma, char* ringStellungen)
{
    int numberOfRotors=enigma->numberOfRotors;
    
    if (strlen(ringStellungen)==numberOfRotors*3-1)
    {
        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            int stellung=(ringStellungen[rotor*3]-'0')*10+(ringStellungen[rotor*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->ringStellung[numberOfRotors-rotor-1]=stellung-1;
            }
            else
            {
                logFatal("Error during coversion of RingStellungen string");
            }    
        }
    }
    else if (strlen(ringStellungen)==numberOfRotors*2-1)
    {
        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            enigma->ringStellung[numberOfRotors-rotor-1]=stellungToPos(ringStellungen[rotor*2]);
        }
    }
    else if (strlen(ringStellungen)==numberOfRotors)
    {
        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            enigma->ringStellung[numberOfRotors-rotor-1]=stellungToPos(ringStellungen[rotor]);
        }
    }
    else
    {
        logFatal("Unexpected string length when defining the ringstellungen");
    }    
}

/**************************************************************************************************\
* 
* Set the Grundstellung of the Walze at given position
* 
\**************************************************************************************************/
void setGrundStellung(Enigma* enigma, int walze, int grundStellung)
{
    int pos=enigma->numberOfRotors-walze;
    enigma->grundStellung[pos]=stellungToPos(grundStellung);
}

/**************************************************************************************************\
* 
* Return the Grundstellung of given Walze
* 
\**************************************************************************************************/
int getGrundStellung(Enigma* enigma, int walze)
{
    int pos=enigma->numberOfRotors-walze;
    return posToStellung(enigma->grundStellung[pos]);
}

/**************************************************************************************************\
* 
*  Sets the GrundStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/
void setGrundStellungen(Enigma* enigma, char* grundStellungen)
{
    int numberOfRotors=enigma->numberOfRotors;
    if (strlen(grundStellungen)==numberOfRotors*3-1)
    {

        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            int stellung=(grundStellungen[rotor*3]-'0')*10+(grundStellungen[rotor*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->grundStellung[numberOfRotors-rotor-1]=stellung-1;
            }
            else
            {
                logFatal("Error during coversion of GrundStellungen string");
            }    
        }
    }
    else if (strlen(grundStellungen)==numberOfRotors*2-1)
    {
        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            enigma->grundStellung[numberOfRotors-rotor-1]=stellungToPos(grundStellungen[rotor*2]);
            rotor++;
        }
    }
    else if (strlen(grundStellungen)==numberOfRotors)
    {
        for (int rotor=0; rotor<enigma->numberOfRotors; rotor++)
        {
            enigma->grundStellung[numberOfRotors-rotor-1]=stellungToPos(grundStellungen[rotor]);
        }
    }
    else
    {
        logFatal("Unexpected string length when defining the GrundStellungen");
    }    
}


/**************************************************************************************************\
* 
* Given the rotorSet, returns a linked list with all valid permutations of the rotors in the set
* For the M4 the 1st rotor is chosed from the fourthRotorSets, for the remaining rotors (2-4) a 
* selection is made from the regular rotorSets
* User must destroy permutations after use, using destoryPermutations()
* 
\**************************************************************************************************/
LinkedList* getWalzenPermutations(Enigma_t enigmaType, RotorSet_t rotorSet)
{
    LinkedList* permutations=NULL;
    int indices[ROTORS];
    int count;

    // Sanity checks
    if ((enigmaType==ENIGMATYPE_M3) && (rotorSet==M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M3 Engima and Naval rotor set");
    }
    if ((enigmaType==ENIGMATYPE_M4) && (rotorSet!=M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M4 Engima and rotor set for M3");
    }

    if (enigmaType==ENIGMATYPE_M4)
    {
        // Rotor 1 (thin one)
        count=0;
        for (int i=0; i<ROTORS; i++)
        {
            if (fourthRotorSets[rotorSet][i]>0)
            {
                indices[count]=i;
                count++;
            }
        }
        LinkedList* permutations1=createPermutations(indices, count, 1);

        // Rotor 2-4 (normal)
        count=0;
        for (int i=0; i<ROTORS; i++)
        {
            if (rotorSets[rotorSet][i]>0)
            {
                indices[count]=i;
                count++;
            }
        }
        LinkedList* permutations2_4=createPermutations(indices, count, 3);
        permutations=combinePermutations(permutations1, 1, permutations2_4, 3);
        destroyPermutations(permutations1);
        destroyPermutations(permutations2_4);
    }
    else if (enigmaType==ENIGMATYPE_M3)
    {
        // Rotors 1-3
        count=0;
        for (int i=0; i<ROTORS; i++)
        {
            if (rotorSets[rotorSet][i]>0)
            {
                indices[count]=i;
                count++;
            }
        }
        permutations=createPermutations(indices, count, 3);
    }
    return permutations;
}