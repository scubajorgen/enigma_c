/**************************************************************************************************\
* 
*  testEnigmaPerformance.c
*  Test the enigma performance. It does not assert but is informative
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "log.h"
#include "test.h"
#include "enigma.h"
#include "coincidence.h"
#include "testframe.h"
#include "toolbox.h"

#define TEST_SIZE 10000000

/**************************************************************************************************\
*
* Helper, time dif in ms
* 
\**************************************************************************************************/
float timeDifference(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testEnigmaEncodeDecodePerformance()
{
    testStart("decode");
    int textLengths[]={2, 5, 10, 12, 25, 50, 75, 100, 125, 150, 200, 400, 600, 800, 1000};

    struct timeval stop, start;
    float diff;

    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    strncpy(settings->cipher, testTextRandom, MAX_TEXT-1);
    setEnigma(enigma, settings);



    printf("Encode/decode\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=textLengths[i];

        // Just the encode/decode
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            encodeDecode(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", textLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }

    printf("\nConfigure, Encode/decode\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=textLengths[i];

        // Configuration + encode/decode
        clearSteckerBrett(enigma);
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            placeWalze(enigma, 1, "I");
            placeWalze(enigma, 2, "II");
            placeWalze(enigma, 3, "III");
            
            placeUmkehrWalze(enigma, "UKW B");
            
            setRingStellung(enigma, 1, 1);
            setRingStellung(enigma, 2, 17);
            setRingStellung(enigma, 3, 12);

            setGrundStellung(enigma, 1, 'A');
            setGrundStellung(enigma, 2, 'B');
            setGrundStellung(enigma, 3, 'C');
            
            placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
            
            encodeDecode(enigma);

            toString(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", textLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }

    printf("\nConfigure, Encode/decode, IoC\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=textLengths[i];
        // Configuration + encode/decode
        clearSteckerBrett(enigma);
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            placeWalze(enigma, 1, "I");
            placeWalze(enigma, 2, "II");
            placeWalze(enigma, 3, "III");
            
            placeUmkehrWalze(enigma, "UKW B");
            
            setRingStellung(enigma, 1, 1);
            setRingStellung(enigma, 2, 17);
            setRingStellung(enigma, 3, 12);

            setGrundStellung(enigma, 1, 'A');
            setGrundStellung(enigma, 2, 'B');
            setGrundStellung(enigma, 3, 'C');
            
            placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
            
            encodeDecode(enigma);

            toString(enigma);

            iocIndexOfCoincidence(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", textLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }



    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testEnigmaIocPerformance()
{
    testStart("decode");

    struct timeval stop, start;
    float diff;

    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    strncpy(settings->cipher, testTextGerman, MAX_TEXT-1);
    setEnigma(enigma, settings);

    // Calculating IoC - float
    gettimeofday(&start, NULL);
    float ioc;
    for (int i=0;i<TEST_SIZE; i++)
    {
        ioc=iocIndexOfCoincidence(enigma);
    }
    gettimeofday(&stop, NULL);
    diff=timeDifference(start, stop);
    logInfo("IoC: %f Time elapsed %.0f ms for %d IoC, %.3f million/sec", ioc, diff, TEST_SIZE, 1000.0f/diff);

    // Calculating IoC - int
    gettimeofday(&start, NULL);
    int iocInt;
    for (int i=0;i<TEST_SIZE; i++)
    {
        iocInt=iocIndexOfCoincidenceFast(enigma);
    }
    gettimeofday(&stop, NULL);
    diff=timeDifference(start, stop);
    logInfo("IoC: %d Time elapsed %.0f ms for %d IoC, %.3f million/sec", iocInt, diff, TEST_SIZE, 1000.0f/diff);

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testEnigmaPerformance()
{
    moduleTestStart("Enigma Performance");
    testEnigmaEncodeDecodePerformance();
    testEnigmaIocPerformance();
    moduleTestWrapUp();
}