#include <stdio.h>
#include <string.h>

#include "enigma.h"
#include "toolbox.h"


/**************************************************************************************************\
* 
* Remove all steckers from the SteckerBrett, which means no swap of letters
* 
\**************************************************************************************************/
void clearSteckerBrett(Enigma* enigma)
{
    int i;
    
    i=0;
    while (i<MAX_POSITIONS)
    {
        enigma->steckerBrett[i]=i;
        i++;
    }

}

/**************************************************************************************************\
* 
* Place steckers. Pass stecker positions as string, like "ab cd ef ..."
* 
\**************************************************************************************************/
void placeSteckers(Enigma* enigma, char steckers[])
{
    int length;
    int pos1;
    int pos2;
    int i;
    
    clearSteckerBrett(enigma);
    
    length=strlen(steckers);
    
    if (length%3!=2)
    {
        printf("WARNING: unexpected length of Stecker string\n");
    }
    i=0;
    while (i<strlen(steckers)+1)
    {
        if (steckers[i]!=' ' && steckers[i+1]!=' ')
        {
            pos1=stellungToPos(steckers[i]);
            pos2=stellungToPos(steckers[i+1]);
            enigma->steckerBrett[pos1]=pos2;
            enigma->steckerBrett[pos2]=pos1;
        }
        else
        {
            printf("ERROR: Steckerstring appears not correct: %s", steckers);
        }    
        i+=3;
    }
    
    
}
