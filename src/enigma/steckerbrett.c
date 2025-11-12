/**************************************************************************************************\
* steckerbrett.c
* 
* Steckerbrett functions. The Steckerbrett is implemented as a table consisting of integers with
* value 0 for 'A' to value 25 for 'Z'.
* This file consists functions for working with Stecker strings defining how Steckers should be 
* placed (like "ab cd ef") as well Steckerbrett tables
* 
\**************************************************************************************************/
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
    for (int i=0; i<MAX_POSITIONS;i++)
    {
        enigma->steckerBrett[i]=i;
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


/**************************************************************************************************\
* 
* Helper: converts the engima stecker string to the stecker table
* The stecker string is like: "AG HI PQ CV". Note: must be CAPITALS!
* 
\**************************************************************************************************/
int steckersToSteckerbrettTable(char steckers[], int* steckerBrettTable)
{
    // Initialise stecker brett table: no steckers
    for (int s=0; s<MAX_POSITIONS; s++)
    {
        steckerBrettTable[s]=s;
    }
  
    // Convert the steckers from the Enigma to the stecker table
    int count=0;

    for  (int s=0; s<strlen(steckers); s+=3)
    {
        steckerBrettTable[steckers[s+0]-'A']=steckers[s+1]-'A';
        steckerBrettTable[steckers[s+1]-'A']=steckers[s+0]-'A';
        count++;
    }  
    return count;
}

/**************************************************************************************************\
* 
* Converts the steckerbrett table to a engima stecker string
* 
\**************************************************************************************************/
void steckerbrettTableToSteckers(int *steckerbrettTable, char steckers[])
{
    steckers[0]='\0';
    // Convert the stecker positions to a stecker brett string
    int s1=0;
    int s2=0;
    while (s1<MAX_POSITIONS)
    {
        if (steckerbrettTable[s1]>s1)
        {
            if (s2>0)
            {
                steckers[3*s2-1]=' ';
            }
            steckers[3*s2]=s1+'A';
            steckers[3*s2+1]=steckerbrettTable[s1]+'A';
            steckers[3*s2+2]='\0';
            s2++;
        }
        s1++;
    }
}


