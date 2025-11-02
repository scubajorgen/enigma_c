/**************************************************************************************************\
* 
* testSteckerbrett.c
* Test the steckerbrett functions
* 
\**************************************************************************************************/

#include "enigma.h"
#include "testframe.h"

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
    
    placeSteckers(enigma, "bq gh");
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
* Test main function
* 
\**************************************************************************************************/
void testSteckerbrett()
{
    moduleTestStart("steckerbrett");
    testPlaceSteckers();
    moduleTestWrapUp();
}