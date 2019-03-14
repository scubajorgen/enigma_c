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
* 
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
        printf("ERROR: invalid umkehrwaltze\n");
    }
    
}