/**************************************************************************************************\
* 
* testSteckerbrett.c
* Test the steckerbrett functions
* 
\**************************************************************************************************/
#include <stdio.h>
#include "enigma.h"
#include "testframe.h"

/**************************************************************************************************\
* 
* Test clearSteckerBrett
* 
\**************************************************************************************************/
void testClearSteckerbrett(void)
{
    testStart("clear steckerbrett");
    Enigma*enigma=createEnigmaM3();
    for (int i=0; i<MAX_POSITIONS;i++)
    {
        enigma->steckerBrett[i]=-1;
    }
    clearSteckerBrett(enigma);
    for (int i=0; i<MAX_POSITIONS;i++)
    {
        assertIntEquals(i, enigma->steckerBrett[i]);
    }
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test placeSteckers
* 
\**************************************************************************************************/
void testPlaceSteckers(void)
{
    testStart("place steckers");
    Enigma*enigma=createEnigmaM3();

    clearSteckerBrett(enigma);
    
    placeSteckers(enigma, "Qb Gh");
    assertIntEquals( 9, enigma->steckerBrett[9]);
    assertIntEquals(16, enigma->steckerBrett[1]);
    assertIntEquals( 1, enigma->steckerBrett[16]);
    assertIntEquals( 6, enigma->steckerBrett[7]);
    assertIntEquals( 7, enigma->steckerBrett[6]);

    clearSteckerBrett(enigma);

    assertIntEquals( 7, enigma->steckerBrett[7]);
    
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test steckersToSteckerbrettTable
* 
\**************************************************************************************************/
void testSteckersToSteckerbrettTable()
{
//                 P   B   C   F   E   D   G   H   I   J   K   L   M
//                 N   O   A   Q   R   U   T   S   V   W   X   Z   Y
    int result[]={15,  1,  2,  5,  4,  3,  6,  7,  8,  9, 10, 11, 12,
                  13, 14,  0, 16, 17, 20, 19, 18, 21, 22, 23, 25, 24};

    int  table[MAX_POSITIONS];
    char steckers[] ="AP FD SU ZY";
    testStart("stecker to table");
    int count=steckersToSteckerbrettTable(steckers, table);
    assertIntEquals(4, count);
    for (int i=0;i<MAX_POSITIONS;i++)
    {
        assertIntEquals(result[i], table[i]);
    }
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test steckerbrettTableToSteckers
* 
\**************************************************************************************************/
void testSteckerbrettTableToSteckers()
{
//                 P   B   C   F   E   D   G   H   I   J   K   L   M
//                 N   O   A   Q   R   U   T   S   V   W   X   Z   Y
    int table[] ={15,  1,  2,  5,  4,  3,  6,  7,  8,  9, 10, 11, 12,
                  13, 14,  0, 16, 17, 20, 19, 18, 21, 22, 23, 25, 24};
    char result[] ="AP DF SU YZ";
    char steckers[MAX_STECKER_STRING];

    testStart("table to steckers");
    steckerbrettTableToSteckers(table, steckers);
    for (int i=0; i<12; i++)
    {
        assertIntEquals(result[i], steckers[i]);
    }
    testWrapUp();
}


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testSteckerbrett()
{
    moduleTestStart("steckerbrett");
    testClearSteckerbrett();
    testPlaceSteckers();
    testSteckersToSteckerbrettTable();
    testSteckerbrettTableToSteckers();
    moduleTestWrapUp();
}