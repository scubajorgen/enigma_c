/**************************************************************************************************\
*
* This file implements the functions and definitions of the Enigma Walzen, or rotors
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "enigma.h"
#include "log.h"
#include "toolbox.h"

char    walzeNames[WALZEN][MAX_WALZE_NAME]=
{
    "I",
    "II",
    "III",
    "IV",
    "V",
    "VI",
    "VII",
    "VIII",
    "Beta", // 4th Walze in M4
    "Gamma" // 4th Walze in M4
};

char    tables[WALZEN][MAX_POSITIONS]=
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


int walzeSets[MAX_WALZE_SETS][WALZEN]=
{
    { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}
};

int fourthWalzeSets[MAX_WALZE_SETS][WALZEN]=
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}
};


/**************************************************************************************************\
* 
* Place the given Walze at given position
* enigima: enigma definition
* walze    : Walze position counted from left to right (1..4)
* walzeName: name of the Walze to place, like 'I', 'VI' 
* 
\**************************************************************************************************/
void placeWalze(Enigma* enigma, int walze, char walzeName[])
{
    int     pos     =enigma->numberOfWalzen-walze;
    bool    found   =false;
    for (int index=0; index<WALZEN && !found; index++)
    {
        if (!strcmp(walzeName, walzeNames[index]))
        {
            found=true;
            
            if ((index==8 || index==9) && 
                (enigma->numberOfWalzen<4 || (enigma->numberOfWalzen==4 && walze!=1)))
            {
                logError("Beta and Gamma Walzen are only allowed on position 1 of Enigma M4");
            }
            
            for (int j=0; j<MAX_POSITIONS; j++)
            {
                enigma->walzeFunction[pos][j]                           =tables[index][j]-'A';
                enigma->walzeInverseFunction[pos][tables[index][j]-'A'] =j;
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
* walze  : Walze position counted from left to right (1..4)
* ringStellung: the Ringstellung. Supported: "12", "G" or "G" or "g"
*
\**************************************************************************************************/
void setRingStellung(Enigma* enigma, int walze, int ringStellung)
{
    int pos=enigma->numberOfWalzen-walze;
    enigma->ringStellung[pos]=stellungToPos(ringStellung);
}

/**************************************************************************************************\
* 
* Return the Ringstellung of given Walze
* 
\**************************************************************************************************/
int getRingStellung(Enigma* enigma, int walze)
{
    int pos=enigma->numberOfWalzen-walze;
    return posToStellung(enigma->ringStellung[pos]);
}


/**************************************************************************************************\
* 
*  Sets the RingStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/

void setRingStellungen(Enigma* enigma, char* ringStellungen)
{
    int numberOfWalzen=enigma->numberOfWalzen;
    
    if (strlen(ringStellungen)==numberOfWalzen*3-1)
    {
        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            int stellung=(ringStellungen[walze*3]-'0')*10+(ringStellungen[walze*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->ringStellung[numberOfWalzen-walze-1]=stellung-1;
            }
            else
            {
                logFatal("Error during coversion of RingStellungen string");
            }    
        }
    }
    else if (strlen(ringStellungen)==numberOfWalzen*2-1)
    {
        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            enigma->ringStellung[numberOfWalzen-walze-1]=stellungToPos(ringStellungen[walze*2]);
        }
    }
    else if (strlen(ringStellungen)==numberOfWalzen)
    {
        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            enigma->ringStellung[numberOfWalzen-walze-1]=stellungToPos(ringStellungen[walze]);
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
    int pos=enigma->numberOfWalzen-walze;
    enigma->grundStellung[pos]=stellungToPos(grundStellung);
}

/**************************************************************************************************\
* 
* Return the Grundstellung of given Walze
* 
\**************************************************************************************************/
int getGrundStellung(Enigma* enigma, int walze)
{
    int pos=enigma->numberOfWalzen-walze;
    return posToStellung(enigma->grundStellung[pos]);
}

/**************************************************************************************************\
* 
*  Sets the GrundStellungen as string. Supported: "12 02 03", "G B C" or "GBC"
* 
\**************************************************************************************************/
void setGrundStellungen(Enigma* enigma, char* grundStellungen)
{
    int numberOfWalzen=enigma->numberOfWalzen;
    if (strlen(grundStellungen)==numberOfWalzen*3-1)
    {

        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            int stellung=(grundStellungen[walze*3]-'0')*10+(grundStellungen[walze*3+1]-'0');
            if (stellung>=1 && stellung<=26)
            {
                enigma->grundStellung[numberOfWalzen-walze-1]=stellung-1;
            }
            else
            {
                logFatal("Error during coversion of GrundStellungen string");
            }    
        }
    }
    else if (strlen(grundStellungen)==numberOfWalzen*2-1)
    {
        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            enigma->grundStellung[numberOfWalzen-walze-1]=stellungToPos(grundStellungen[walze*2]);
            walze++;
        }
    }
    else if (strlen(grundStellungen)==numberOfWalzen)
    {
        for (int walze=0; walze<enigma->numberOfWalzen; walze++)
        {
            enigma->grundStellung[numberOfWalzen-walze-1]=stellungToPos(grundStellungen[walze]);
        }
    }
    else
    {
        logFatal("Unexpected string length when defining the GrundStellungen");
    }    
}


/**************************************************************************************************\
* 
* Given the walzeSet, returns a linked list with all valid permutations of the Walzen in the set
* For the M4 the 1st Walze is chosed from the fourthWalzeSets, for the remaining Walzen (2-4) a 
* selection is made from the regular walzeSets
* User must destroy permutations after use, using destoryPermutations()
* 
\**************************************************************************************************/
LinkedList* getWalzenPermutations(Enigma_t enigmaType, WalzeSet_t walzeSet)
{
    LinkedList* permutations=NULL;
    int indices[WALZEN];
    int count;

    // Sanity checks
    if ((enigmaType==ENIGMATYPE_M3) && (walzeSet==M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M3 Engima and Naval Walze set");
    }
    if ((enigmaType==ENIGMATYPE_M4) && (walzeSet!=M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M4 Engima and Walze set for M3");
    }

    if (enigmaType==ENIGMATYPE_M4)
    {
        // Walze 1 (thin one)
        count=0;
        for (int i=0; i<WALZEN; i++)
        {
            if (fourthWalzeSets[walzeSet][i]>0)
            {
                indices[count]=i;
                count++;
            }
        }
        LinkedList* permutations1=createPermutations(indices, count, 1);

        // Walze 2-4 (normal)
        count=0;
        for (int i=0; i<WALZEN; i++)
        {
            if (walzeSets[walzeSet][i]>0)
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
        // Walzen 1-3
        count=0;
        for (int i=0; i<WALZEN; i++)
        {
            if (walzeSets[walzeSet][i]>0)
            {
                indices[count]=i;
                count++;
            }
        }
        permutations=createPermutations(indices, count, 3);
    }
    return permutations;
}