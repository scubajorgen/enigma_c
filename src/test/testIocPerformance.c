/**************************************************************************************************\
* 
*  testIoCPerformance.c
*  Test indicating in how many cases the Gillogly method decodes the cipher.
*  Refer to the graph in the original Gillogly article
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testframe.h"
#include "test.h"
#include "enigma.h"
#include "log.h"
#include "coincidence.h"
#include "toolbox.h"


#define NUMBER_OF_CIPHERLENGTHS 6
#define NUMBER_OF_TESTS         50
#define MIN_NUMBER_OF_STECKERS  0
#define MAX_NUMBER_OF_STECKERS  12

/*
#define NUMBER_OF_CIPHERLENGTHS 2
#define NUMBER_OF_TESTS         2
#define MIN_NUMBER_OF_STECKERS  0
#define MAX_NUMBER_OF_STECKERS  0
*/

char iocTestPlainText[MAX_TEXT];
char iocTestCipher[MAX_TEXT];

typedef struct 
{
    int     cipherLength;
    int     numberOfSteckers;
    float   score[NUMBER_OF_TESTS];
} Result;

int maxResults=NUMBER_OF_CIPHERLENGTHS*(MAX_NUMBER_OF_STECKERS-MIN_NUMBER_OF_STECKERS+1);
Result results[NUMBER_OF_CIPHERLENGTHS*(MAX_NUMBER_OF_STECKERS-MIN_NUMBER_OF_STECKERS+1)];

int cipherLengths[]={161, 316, 481, 647, 795, 1463};


/**************************************************************************************************\
*
* Create a random section out of the source of length size
* 
\**************************************************************************************************/
char* createSampleText(char* source, int size)
{
    int startMax=strlen(source)-size;
    int start=randint(startMax);
    strncpy(iocTestPlainText, source+start, size);
    iocTestPlainText[size]='\0';
    return iocTestPlainText;
}

/**************************************************************************************************\
*
* Calculates the percentage overlap between texts
* 
\**************************************************************************************************/
float calculateOverlap(char* plaintext, char* try)
{
    if (strlen(plaintext)!=strlen(try))
    {
        logFatal("Texts not same size!");
    }
    int count=0;
    for (int i=0; i<strlen(plaintext); i++)
    {
        if (plaintext[i]==try[i])
        {
            count++;
        }
    }
    return (100.0*count/strlen(plaintext));
}


/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testIocScoring()
{
    testStart("IoC Scoring");
    Enigma*     enigma=createEnigmaM3();

    int resultCount=0;
    for (int index=0; index<NUMBER_OF_CIPHERLENGTHS; index++)
    {
        for (int numOfSteckers=MIN_NUMBER_OF_STECKERS; numOfSteckers<=MAX_NUMBER_OF_STECKERS; numOfSteckers++)
        {
            for (int test=0; test<NUMBER_OF_TESTS; test++)
            {
                int             cipherLength=cipherLengths[index];
                EnigmaSettings* settings=createRandomSettings(enigma, M3_ARMY_1938, numOfSteckers);
                printEnigmaSettings(settings, "Test");
                char*           plain   =createSampleText(testTextEnigma, cipherLength);


                setEnigma(enigma, settings);
                setText(enigma, plain);
                encodeDecode(enigma);
                strncpy(iocTestCipher, toString(enigma), MAX_TEXT-1);

                IocRecipe*  recipe      =createDefaultRecipe(iocTestCipher, 6);
                //recipe->maxSteckers     =numOfSteckers;
                EnigmaSettings* found   =iocDecodeText(*recipe, NULL);
                setEnigma(enigma, found);
                setText(enigma, iocTestCipher);
                encodeDecode(enigma);

                float score=calculateOverlap(plain, toString(enigma));
                printf("plain: \n%s\n", plain);
                printf("cipgher: \n%s\n", toString(enigma));
                printf("OVERLAP %f\n\n", score);
                destroyRecipe(recipe);
                destroyEnigmaSettings(settings);

                results[resultCount].cipherLength       =cipherLength;
                results[resultCount].numberOfSteckers   =numOfSteckers;
                results[resultCount].score[test]        =score;
            }
            resultCount++;
        }
    }

    // Print the scores
    printf("cipher length, steckers, scores\n");
    for (int i=0; i<resultCount; i++)
    {
        printf("%03d, %02d", results[i].cipherLength, results[i].numberOfSteckers);
        for (int test=0; test<NUMBER_OF_TESTS; test++)
        {
            printf(", %5.2f", results[i].score[test]);
        }
        printf("\n");
    }

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testIocPerformance()
{
    srand(1);
    moduleTestStart("IOC PERFORMANCE");

    testIocScoring();

    moduleTestWrapUp();
}