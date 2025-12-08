/**************************************************************************************************\
* 
*  testTuringPerformance.c
*  Test indicating the performance of the Turing Bombe function
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testframe.h"
#include "test.h"
#include "enigma.h"
#include "log.h"
#include "turing.h"
#include "toolbox.h"


#define TURINGBOMBETEST_CIPHER_LENGTH   50
#define TURINGBOMBETEST_MAX_ITERATIONS  10
#define TURINGBOMBETEST_SAMPLETEXT      testTextEnigma

char turingTestPlainText[MAX_TEXT];
char turingTestCrib[MAX_TEXT];
char turingTestCipher[MAX_TEXT];

typedef struct
{
    int     cribLength;
    int     solutions;
    bool    solutionFound;
    float   bombeTime;
} TuringBombResult;

/**************************************************************************************************\
* 
* Helper: given the plain text, it generates the cipher using a random enigma with 10 Steckers
* 
\**************************************************************************************************/
void generateCipher(char* plain, char* cipher, int numOfSteckers)
{
    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, numOfSteckers);
    setEnigma(enigma, settings);
    setText(enigma, plain);
    printEnigmaSettings(settings, "Test");
    encodeDecode(enigma);
    strncpy(cipher, toString(enigma), MAX_TEXT-1);
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);
}


/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testTuringCribCircles()
{
    testStart("Cribcircles");
    int cribLengths[]={25, 30};
    int cribPosition    =0;
    int numOfSteckers   =10;

    for (int index=0; index<sizeof(cribLengths)/sizeof(int); index++)
    {
        int cribLength=cribLengths[index];
        for(int iteration=0; iteration</*TURINGBOMBETEST_MAX_ITERATIONS*/2; iteration++)
        {
            logInfo("------------------------------------------------------------------------------------");
            logInfo("Iteration %d", iteration);

            
            createSampleText(turingTestPlainText, TURINGBOMBETEST_SAMPLETEXT, TURINGBOMBETEST_CIPHER_LENGTH);
            memcpy(turingTestCrib, turingTestPlainText+cribPosition, cribLength+1);
            generateCipher(turingTestPlainText, turingTestCipher, numOfSteckers);
            logInfo("Plain : %s", turingTestPlainText);
            logInfo("Crib  : %s", turingTestCrib);
            logInfo("Cipher: %s", turingTestCipher);

            turingFindCribCircles(turingTestCipher, turingTestCrib, 0, true);
            dumpSets();
        }
    }
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testTuringBombe()
{
    testStart("Turing Bombe");
    int cribLengths[]={25, 30};
    int cribPosition    =0;
    int numOfSteckers   =10;
    for (int index=0; index<sizeof(cribLengths)/sizeof(int); index++)
    {
        int cribLength=cribLengths[index];
        for(int iteration=0; iteration<TURINGBOMBETEST_MAX_ITERATIONS; iteration++)
        {
            logInfo("------------------------------------------------------------------------------------");
            logInfo("Iteration %d", iteration);
            createSampleText(turingTestPlainText, TURINGBOMBETEST_SAMPLETEXT, TURINGBOMBETEST_CIPHER_LENGTH);
            memcpy(turingTestCrib, turingTestPlainText+cribPosition, cribLength+1);
            generateCipher(turingTestPlainText, turingTestCipher, numOfSteckers);
            logInfo("Plain : %s", turingTestPlainText);
            logInfo("Crib  : %s", turingTestCrib);
            logInfo("Cipher: %s", turingTestCipher);

            TuringRecipe* recipe    =createDefaultTuringRecipe(turingTestCipher, turingTestCrib, 0, 4);
            LinkedList*   results   =linkedListCreate();
            turingBombe(*recipe, results);
            turingReport(MESSAGEFORMAT_TEXT);

            if (linkedListLength(results)==0)
            {
                logInfo("No solution found, incorporating Ringstellung 2");
                recipe->startR2='A';
                recipe->endR2='Z';
                turingBombe(*recipe, results);
                turingReport(MESSAGEFORMAT_TEXT);
            }

            assertIntEquals(1, linkedListLength(results));
            TuringResult* result=(TuringResult*)linkedListObjectAt(results, 0);
            assertStringEquals(turingTestPlainText, result->decoded);

            linkedListDestroy(results, true);
            destroyTuringRecipe(recipe);
        }
    }   
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testTuringPerformance()
{
    srand(1);
    moduleTestStart("TURING PERFORMANCE");
    testTuringCribCircles();
    //testTuringBombe();
    moduleTestWrapUp();
}