/**************************************************************************************************\
* 
*  testUkw.c
*  Test the Umkehrwalze functions
* 
\**************************************************************************************************/
#include "enigma.h"
#include "testframe.h"
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
* Test main function
* 
\**************************************************************************************************/
void testUkw()
{
    moduleTestStart("ukw");
    testUkwB();
    testUkwC();
    moduleTestWrapUp();
}