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
#include "ngramScore.h"
#include "testframe.h"
#include "toolbox.h"

#define TEST_SIZE       10000000
#define TEST_TEXT_MAX   1000
int testTextLengths[]={2, 5, 10, 12, 25, 50, 75, 100, 125, 150, 200, 400, 600, 800, 1000};


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

    struct timeval stop, start;
    float diff;

    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    strncpy(settings->cipher, testTextRandom, MAX_TEXT-1);
    setEnigma(enigma, settings);

    printf("ENCODE/DECODE\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=testTextLengths[i];

        // Just the encode/decode
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            encodeDecode(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }

    printf("\nCONFIGURE, ENCODE/DECODE\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=testTextLengths[i];

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
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }

    printf("\nCONFIGURE, ENCODE/DECODE, IOC\n");
    printf("text size, encodes, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {
        enigma->textSize=testTextLengths[i];
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
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Test calculation of the Index of Coincidence
* 
\**************************************************************************************************/
void testEnigmaIocPerformance()
{
    testStart("IoC");

    struct  timeval stop, start;
    float           diff;
    int             iocInt;
    float           iocFloat;

    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    setEnigma(enigma, settings);
    for (int i=0;i<TEST_TEXT_MAX;i++)
    {
        enigma->conversion[i]=testTextRandom[i]-'A';
    }

    printf("\nFLOAT BASED IOC CALCULATION\n");
    printf("text size, calculations, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {   
        enigma->textSize=testTextLengths[i];

        // Calculating IoC - float
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            iocFloat=iocIndexOfCoincidence(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }


    printf("\nFLOAT BASED IOC CALCULATION\n");
    printf("text size, calculations, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {   
        enigma->textSize=testTextLengths[i];
        // Calculating IoC - int
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            iocInt=iocIndexOfCoincidenceFast(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }
    printf("Last calculated ioc: float %f int %i\n", iocFloat, iocInt);
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testEnigmaNgramPerformance()
{
    testStart("NGRAM");

    struct timeval  stop, start;
    float           diff;
    float           score;

    prepareNgramScore(3, "GB");

    Enigma*         enigma  =createEnigmaM3();
    EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    setEnigma(enigma, settings);
    for (int i=0;i<TEST_TEXT_MAX;i++)
    {
        enigma->conversion[i]=testTextEnglish[i]-'A';
    }

    printf("\nNGRAM CALCULATION\n");
    printf("text size, calculations, elapsed time (ms), rate (/s), time/decode (us)\n");
    for (int i=0; i<15; i++)
    {   
        enigma->textSize=testTextLengths[i];
        gettimeofday(&start, NULL);
        for (int i=0;i<TEST_SIZE; i++)
        {
            score=ngramScore(enigma);
        }
        gettimeofday(&stop, NULL);
        diff=timeDifference(start, stop);
        printf("%3d, %d, %.0f, %.0f, %.3f\n", testTextLengths[i], TEST_SIZE, diff, 1e9/diff, 1000.0*diff/TEST_SIZE);
    }
    printf("Latest score calculated: %f\n", score);
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
    moduleTestStart("PERFORMANCE");
    testEnigmaEncodeDecodePerformance();
    testEnigmaIocPerformance();
    testEnigmaNgramPerformance();
    moduleTestWrapUp();
}