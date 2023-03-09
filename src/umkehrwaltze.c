/**************************************************************************************************\
*
* This file implements the functions and definitions of the Enigma Umkehrwaltze, or reflector
* 
\**************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "enigma.h"

#define UMKEHR_WALTZEN          5
#define MAX_UMKEHRWALTZE_NAME   10


char umkehrWaltzeNames[UMKEHR_WALTZEN][MAX_UMKEHRWALTZE_NAME]=
{
    "UKW A",
    "UKW B",
    "UKW C",
    "UKW B2",
    "UKW C2"
};

int umkehrWaltzeTables[UMKEHR_WALTZEN][MAX_POSITIONS]=
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


/**************************************************************************************************\
* 
* Choose the Umkehrwaltze to use. 
* enigima: enigma definition
* name: name of the Umkehrwaltze. "UKW A", "UKW B" or "UKW C" for Engima M3, "UKW B2" and "UKW C2"
*       are the thin UKWs for Engima M4 (though all UKWs can be used for all Enigmas in this 
*       software) 
* 
\**************************************************************************************************/
void placeUmkehrWaltze(Enigma* enigma, char name[])
{
    int     index;
    int     found;
    int     j;
    
    index=0;
    found=0;
    while (index<UMKEHR_WALTZEN && !found)
    {
        
        if (!strcmp(name, umkehrWaltzeNames[index]))
        {
            if ((index==3 || index==4) && (enigma->numberOfRotors<4))
            {
                printf("UKW B2 an C2 are only allowed on Engima M4\n");
            }
            found=1;
            j=0;
            while (j<MAX_POSITIONS)
            {
                enigma->umkehrWaltzeFunction[j]=umkehrWaltzeTables[index][j]-'A';
                j++;
            }
        }

        index++;
    }  
    if (!found)
    {
        printf("ERROR: invalid umkehrwaltze %s\n", name);
    }
}