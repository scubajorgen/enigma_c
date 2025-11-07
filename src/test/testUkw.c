/**************************************************************************************************\
* 
*  testUkw.c
*  Test the Umkehrwalze functions
* 
\**************************************************************************************************/
#include "enigma.h"
#include "testframe.h"
#include "toolbox.h"
/**************************************************************************************************\
* 
* Test Ukw B
* 
\**************************************************************************************************/
void testUkwB(void)
{
    testStart("UKW B");
    Enigma* enigma=createEnigmaM4();
    placeUmkehrWalze(enigma, "UKW B");
    assertIntEquals(23, enigma->umkehrWalzeFunction[9]);   // J (9) maps to X (23)
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Ukw C
* 
\**************************************************************************************************/
void testUkwC()
{
    testStart("UKW C");
    Enigma* enigma=createEnigmaM4();
    placeUmkehrWalze(enigma, "UKW C2");
    assertIntEquals(23, enigma->umkehrWalzeFunction[18]);   //  maps to X
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the getUkwPermutations
* 
\**************************************************************************************************/
void testUkwPermutations()
{
    testStart("permutations");

    LinkedList* p=getUkwPermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    assertIntEquals(2, linkedListLength(p));
    resetLinkedList(p);
    int* permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(1, permutation[0]);
    permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(2, permutation[0]);
    destroyPermutations(p);

    p=getUkwPermutations(ENIGMATYPE_M3, M3_ARMY_1939);
    assertIntEquals(2, linkedListLength(p));
    resetLinkedList(p);
    permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(1, permutation[0]);
    permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(2, permutation[0]);
    destroyPermutations(p);

    p=getUkwPermutations(ENIGMATYPE_M4, M4_NAVAL_1941);
    assertIntEquals(2, linkedListLength(p));
    resetLinkedList(p);
    permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(3, permutation[0]);
    permutation=(int *)nextLinkedListObject(p);
    assertIntEquals(4, permutation[0]);
    destroyPermutations(p);

    testWrapUp();
}


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testUkw()
{
    moduleTestStart("ukw");
    testUkwB();
    testUkwC();
    testUkwPermutations();
    moduleTestWrapUp();
}
