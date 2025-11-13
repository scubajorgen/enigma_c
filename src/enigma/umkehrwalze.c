/**************************************************************************************************\
*
* This file implements the functions and definitions of the Enigma Umkehrwalze, or reflector
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "enigma.h"
#include "log.h"
#include "toolbox.h"

char umkehrWalzeNames[UMKEHR_WALZEN][MAX_ROTOR_NAME]=
{
    "UKW A",  // Used before WWII
    "UKW B",
    "UKW C",
    "UKW B2", // thin B
    "UKW C2"  // thin C
};

int umkehrWalzeTables[UMKEHR_WALZEN][MAX_POSITIONS]=
{
    {
        'E','J','M','Z','A','L','Y','X','V','B','W','F','C','R','Q','U','O','N','T','S','P','I','K','H','G','D'
    },
    {
        'Y','R','U','H','Q','S','L','D','P','X','N','G','O','K','M','I','E','B','F','Z','C','W','V','J','A','T'
    },
    {
        'F','V','P','J','I','A','O','Y','E','D','R','Z','X','W','G','C','T','K','U','Q','S','B','N','M','H','L'
    },
    {
        'E','N','K','Q','A','U','Y','W','J','I','C','O','P','B','L','M','D','X','Z','V','F','T','H','R','G','S'
    },
    {
        'R','D','O','B','J','N','T','K','V','E','H','M','L','F','C','W','Z','A','X','G','Y','I','P','S','U','Q'
    }
};


// The sets of UKWs
int ukwSets[MAX_ROTOR_SETS][UMKEHR_WALZEN]=
{
    { 0, 1, 1, 0, 0},
    { 0, 1, 1, 0, 0},
    { 0, 1, 1, 0, 0},
    { 0, 0, 0, 1, 1}
};

/**************************************************************************************************\
* 
* Choose the Umkehrwalze to use. 
* enigima: enigma definition
* name: name of the Umkehrwalze. "UKW A", "UKW B" or "UKW C" for Engima M3, "UKW B2" and "UKW C2"
*       are the thin UKWs for Engima M4 (though all UKWs can be used for all Enigmas in this 
*       software) 
* 
\**************************************************************************************************/
void placeUmkehrWalze(Enigma* enigma, char name[])
{
    bool found=false;
    for (int index=0; index<UMKEHR_WALZEN && !found; index++)
    {
        
        if (!strcmp(name, umkehrWalzeNames[index]))
        {
            if ((index==3 || index==4) && (enigma->numberOfRotors<4))
            {
                printf("UKW B2 an C2 are only allowed on Engima M4\n");
            }
            found=true;
            for (int j=0; j<MAX_POSITIONS; j++)
            {
                enigma->umkehrWalzeFunction[j]=umkehrWalzeTables[index][j]-'A';
            }
        }
    }  
    if (!found)
    {
        logFatal("ERROR: invalid umkehrwalze %s", name);
    }
}

/**************************************************************************************************\
* 
* Given the rotorSet, returns a linked list with all valid permutations of the rotors in the set
* For the M4 the 1st rotor is chosed from the fourthRotorSets, for the remaining rotors (2-4) a 
* selection is made from the regular rotorSets.
* User must destroy permutations after use, using destoryPermutations()
* 
\**************************************************************************************************/
LinkedList* getUkwPermutations(Enigma_t enigmaType, RotorSet_t rotorSet)
{
    LinkedList* permutations;
    int indices[UMKEHR_WALZEN];

    // Sanity checks
    if ((enigmaType==ENIGMATYPE_M3) && (rotorSet==M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M3 Engima and Naval rotor set");
    }
    if ((enigmaType==ENIGMATYPE_M4) && (rotorSet!=M4_NAVAL_1941))
    {
        logFatal("Illegal combination of M4 Engima and rotor set for M3");
    }

    // Rotors 1-3
    int count=0;
    for (int i=0; i<UMKEHR_WALZEN; i++)
    {
        if (ukwSets[rotorSet][i]>0)
        {
            indices[count]=i;
            count++;
        }
    }
    permutations=createPermutations(indices, count, 1);

    return permutations;
}
