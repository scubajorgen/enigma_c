/**************************************************************************************************\
* 
* testWalze.c
* Test the Walze functions
* 
\**************************************************************************************************/

#include "enigma.h"
#include "testframe.h"
#include "toolbox.h"


/**************************************************************************************************\
* 
* Test the Walze placememt
* 
\**************************************************************************************************/
void testWalzePlaceWalze()
{
    testStart("place walze");
    Enigma* enigma=createEnigmaM3();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");

    // Walze 3 is stored in walzeFunction[0]
    assertIntEquals( 1, enigma->walzeFunction[0][ 0]); // III: B = 1
    assertIntEquals( 3, enigma->walzeFunction[0][ 1]); // III: D = 3
    assertIntEquals(14, enigma->walzeFunction[0][25]); // III: O = 14

    assertIntEquals(0, enigma->walzeFunction[1][ 0]); // II  : A = 0

    assertIntEquals(9, enigma->walzeFunction[2][25]); // I   : J = 9

    destroyEnigma(enigma);

    enigma=createEnigmaM4();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "IV");
    placeWalze(enigma, 4, "III");

    // Walze 3 is stored in walzeFunction[0]
    assertIntEquals( 1, enigma->walzeFunction[0][ 0]); // III: B = 1
    assertIntEquals( 3, enigma->walzeFunction[0][ 1]); // III: D = 3
    assertIntEquals(14, enigma->walzeFunction[0][25]); // III: O = 14

    assertIntEquals(9, enigma->walzeFunction[3][25]); // I   : J = 9

    destroyEnigma(enigma);
    testWrapUp();
}

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
    testStart("grundstellungen");

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
* Test the getWalzenPermutations
* 
\**************************************************************************************************/
void testWalzePermutations()
{
    testStart("permutations");

    LinkedList* p=getWalzenPermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    assertIntEquals(60, linkedListLength(p));
    linkedListReset(p);
    int* permutation=(int *)linkedListNextObject(p);
    assertIntEquals(0, permutation[0]);
    assertIntEquals(1, permutation[1]);
    assertIntEquals(2, permutation[2]);
    permutation=(int *)linkedListNextObject(p);
    assertIntEquals(0, permutation[0]);
    assertIntEquals(1, permutation[1]);
    assertIntEquals(3, permutation[2]);
    destroyPermutations(p);

    p=getWalzenPermutations(ENIGMATYPE_M4, M4_NAVAL_1941);
    assertIntEquals(672, linkedListLength(p));
    linkedListReset(p);
    permutation=(int *)linkedListNextObject(p);
    assertIntEquals(8, permutation[0]);
    assertIntEquals(0, permutation[1]);
    assertIntEquals(1, permutation[2]);
    assertIntEquals(2, permutation[3]);
    permutation=(int *)linkedListNextObject(p);
    assertIntEquals(8, permutation[0]);
    assertIntEquals(0, permutation[1]);
    assertIntEquals(1, permutation[2]);
    assertIntEquals(3, permutation[3]);
    destroyPermutations(p);

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
    testWalzePlaceWalze();
    testWalzeRingstellung();
    testWalzeGrundstellung();
    testWalzePermutations();
    moduleTestWrapUp();
}