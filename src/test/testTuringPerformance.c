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


#define TURINGTEST_CIPHER_LENGTH        50  // Uses cipher; not very relevant...

#define TURINGCRIBTEST_MAX_ITERATIONS   30
#define TURINGCRIBTEST_MIN_CRIB_LENGTH  5
#define TURINGCRIBTEST_MAX_CRIB_LENGTH  37

#define TURINGBOMBETEST_MIN_CRIB_LENGTH 23
#define TURINGBOMBETEST_MAX_CRIB_LENGTH 36
#define TURINGBOMBETEST_MAX_ITERATIONS  30
#define TURINGBOMBETEST_SAMPLETEXT      testTextEnigma

char turingTestPlainText[MAX_TEXT];
char turingTestCrib[MAX_TEXT];
char turingTestCipher[MAX_TEXT];

typedef struct
{
    int     iteration;
    int     cribLength;
    bool    solutionFound;
    float   bombeTime;
    bool    ringstellung2Used;
    int     minCribCircleSize;
    int     maxCribCircleSize;
    float   aveCribCircleSize;
    int     numberOfCribCircles;
    long    decrypts;
    long    candidates;
    long    validCandidates;
    long    solutions;
} TuringBombResult;

typedef struct
{
    int     iteration;
    int     cribLength;
    int     numberOfCircles;
    int     minCircleLength;
    int     maxCircleLength;
    float   averageCircleLength;
} TuringCribCircleResult;


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
* This function creates the cribcircles and collects some statistics
* 
\**************************************************************************************************/

TuringCribCircleResult ccResults[TURINGCRIBTEST_MAX_ITERATIONS*(TURINGCRIBTEST_MAX_CRIB_LENGTH-TURINGCRIBTEST_MIN_CRIB_LENGTH)];

void testTuringCribCircles()
{
    testStart("Cribcircles");
    int cribPosition    =0;
    int numOfSteckers   =10;

    int numOfCribLengths=TURINGCRIBTEST_MAX_CRIB_LENGTH-TURINGCRIBTEST_MIN_CRIB_LENGTH;
    int count=0;
    for (int cribLength=TURINGCRIBTEST_MIN_CRIB_LENGTH; cribLength<TURINGCRIBTEST_MAX_CRIB_LENGTH; cribLength++)
    {
        for(int iteration=0; iteration<TURINGCRIBTEST_MAX_ITERATIONS; iteration++)
        {
            logInfo("------------------------------------------------------------------------------------");
            logInfo("Iteration %d %d", iteration, cribLength);

            
            createSampleText(turingTestPlainText, TURINGBOMBETEST_SAMPLETEXT, TURINGTEST_CIPHER_LENGTH);
            memcpy(turingTestCrib, turingTestPlainText+cribPosition, cribLength+1);
            generateCipher(turingTestPlainText, turingTestCipher, numOfSteckers);
            logInfo("Plain : %s", turingTestPlainText);
            logInfo("Crib  : %s", turingTestCrib);
            logInfo("Cipher: %s", turingTestCipher);

            turingFindCribCircles(turingTestCipher, turingTestCrib, 0, true);
            ccResults[count].iteration          =iteration;
            ccResults[count].cribLength         =cribLength;
            ccResults[count].numberOfCircles    =totalNumberOfCribCircles();
            ccResults[count].minCircleLength    =minCribCircleSize();
            ccResults[count].maxCircleLength    =maxCribCircleSize();
            ccResults[count].averageCircleLength=averageCribCircleSize();
            count++;
        }
    }
    printf("criblength, iteration, number of crib circles, average circle size, min circle, max circle\n");
    for (int i=0; i<numOfCribLengths*TURINGCRIBTEST_MAX_ITERATIONS; i++)
    {
        printf("%d, %d, %d, %f, %d, %d\n",
                ccResults[i].cribLength, ccResults[i].iteration, ccResults[i].numberOfCircles, 
                ccResults[i].averageCircleLength, ccResults[i].minCircleLength, ccResults[i].maxCircleLength);
    }
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/

TuringBombResult  bResults[TURINGBOMBETEST_MAX_ITERATIONS*(TURINGBOMBETEST_MAX_CRIB_LENGTH-TURINGBOMBETEST_MIN_CRIB_LENGTH)];
TuringStats       bStatistics;

void testTuringBombe()
{
    testStart("Turing Bombe");
    int cribPosition    =0;
    int numOfSteckers   =10;
    for (int cribLength=TURINGBOMBETEST_MAX_CRIB_LENGTH-1; cribLength>=TURINGBOMBETEST_MIN_CRIB_LENGTH; cribLength--)
    {
        for(int iteration=0; iteration<TURINGBOMBETEST_MAX_ITERATIONS; iteration++)
        {
            logInfo("------------------------------------------------------------------------------------");
            logInfo("Iteration %d", iteration);
            createSampleText(turingTestPlainText, TURINGBOMBETEST_SAMPLETEXT, TURINGTEST_CIPHER_LENGTH);
            memcpy(turingTestCrib, turingTestPlainText+cribPosition, cribLength+1);
            generateCipher(turingTestPlainText, turingTestCipher, numOfSteckers);
            logInfo("Plain : %s", turingTestPlainText);
            logInfo("Crib  : %s", turingTestCrib);
            logInfo("Cipher: %s", turingTestCipher);

            TuringRecipe* recipe    =createDefaultTuringRecipe(turingTestCipher, turingTestCrib, 0, 6);
            LinkedList*   results   =linkedListCreate();

            turingBombe(*recipe, results, &bStatistics);
            turingReport(MESSAGEFORMAT_TEXT);

            
            bool ringstellung2Used=false;
            if (linkedListLength(results)==0)
            {
                logInfo("No solution found, incorporating Ringstellung 2");
                recipe->startR2='A';
                recipe->endR2='Z';
                turingBombe(*recipe, results, &bStatistics);
                turingReport(MESSAGEFORMAT_TEXT);
                ringstellung2Used=true;
            }

            bool solutionFound=false;
            if (linkedListLength(results)>0)
            {
                TuringResult* result=(TuringResult*)linkedListObjectAt(results, 0);
                if (strncmp(result->decoded, turingTestPlainText, MAX_TEXT)==0)
                {
                    solutionFound=true;
                }

            }

            int resultIndex=(cribLength-TURINGBOMBETEST_MIN_CRIB_LENGTH)*TURINGBOMBETEST_MAX_ITERATIONS+iteration;
            bResults[resultIndex].cribLength            =cribLength;
            bResults[resultIndex].iteration             =iteration;
            bResults[resultIndex].solutions             =linkedListLength(results);
            bResults[resultIndex].ringstellung2Used     =ringstellung2Used;
            bResults[resultIndex].solutionFound         =solutionFound;
            bResults[resultIndex].minCribCircleSize     =bStatistics.minCribCircleSize;
            bResults[resultIndex].maxCribCircleSize     =bStatistics.maxCribCircleSize;
            bResults[resultIndex].aveCribCircleSize     =bStatistics.aveCribCircleSize;
            bResults[resultIndex].aveCribCircleSize     =bStatistics.aveCribCircleSize;
            bResults[resultIndex].numberOfCribCircles   =bStatistics.numberOfCribCircles;
            bResults[resultIndex].bombeTime             =bStatistics.milliseconds/1000;
            bResults[resultIndex].decrypts              =bStatistics.decrypts;
            bResults[resultIndex].candidates            =bStatistics.candidates;
            bResults[resultIndex].validCandidates       =bStatistics.validCandidates;

            linkedListDestroy(results, true);
            destroyTuringRecipe(recipe);

            // Print after each iteration in case stuff blocks
            printf("crib length, iteration, solution found, ringstellung 2, time (s), number of circles, ");
            printf("average circle size, min circle size, max circle size, decrypts, candidates, valid candidates, solutions\n");
            for (int i=0;i<TURINGBOMBETEST_MAX_ITERATIONS*(TURINGBOMBETEST_MAX_CRIB_LENGTH-TURINGBOMBETEST_MIN_CRIB_LENGTH); i++)
            {
                printf("%d, %d, %d, %d, %f, %d, %f, %d, %d, %ld, %ld, %ld, %ld\n",
                        bResults[i].cribLength, bResults[i].iteration,
                        bResults[i].solutionFound, bResults[i].ringstellung2Used,
                        bResults[i].bombeTime, bResults[i].numberOfCribCircles,
                        bResults[i].aveCribCircleSize, bResults[i].minCribCircleSize, bResults[i].maxCribCircleSize,
                        bResults[i].decrypts, bResults[i].candidates, bResults[i].validCandidates, bResults[i].solutions);
            }
        }
    }
    testWrapUp();
}

/**************************************************************************************************
* 
* Test main function
* 
\**************************************************************************************************/
void testTuringPerformance()
{
    srand(1);
    moduleTestStart("TURING PERFORMANCE");
    testTuringCribCircles();
    testTuringBombe();
    moduleTestWrapUp();
}