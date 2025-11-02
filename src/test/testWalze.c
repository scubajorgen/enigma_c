/**************************************************************************************************\
* 
* testWalze.c
* Test the Walze functions
* 
\**************************************************************************************************/

#include "enigma.h"
#include "testframe.h"

/**************************************************************************************************\
* 
* Test the Ringstellungen
* 
\**************************************************************************************************/
void testWalzeRingstellung()
{
    testStart("ringstellungen");
    int         stellung;
    Enigma* enigma=createEnigmaM3();

    setRingStellung(enigma, 1, 'j'); 
    assertIntEquals(9, enigma->ringStellung[2]);

    setRingStellung(enigma, 1, 12); 
    assertIntEquals(11, enigma->ringStellung[2]);
    
    setRingStellungen(enigma, "JKL");
    assertIntEquals('L'-'A', enigma->ringStellung[0]);
    
    setRingStellungen(enigma, "p q r");
    assertIntEquals('P'-'A', enigma->ringStellung[2]);

    stellung=getRingStellung(enigma, 1);
    assertIntEquals('P'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 2);
    assertIntEquals('Q'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 3);
    assertIntEquals('R'-'A'+1, stellung);

    setRingStellungen(enigma, "09 10 11");
    assertIntEquals('I'-'A', enigma->ringStellung[2]);
    
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the Ringstellungen
* 
\**************************************************************************************************/
void testWalzeGrundstellung()
{
    testStart("ringstellungen");

    Enigma* enigma=createEnigmaM3();

    setGrundStellungen(enigma, "STU");
    assertIntEquals('U'-'A', enigma->grundStellung[0]);
    
    setGrundStellungen(enigma, "b c d");
    assertIntEquals('B'-'A', enigma->grundStellung[2]);

    setGrundStellungen(enigma, "26 10 01");
    assertIntEquals('Z'-'A', enigma->grundStellung[2]);
    
    destroyEnigma(enigma);
    testWrapUp();

}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testWalze()
{
    moduleTestStart("walze");
    testWalzeRingstellung();
    testWalzeGrundstellung();
    moduleTestWrapUp();
}