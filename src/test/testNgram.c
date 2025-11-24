/**************************************************************************************************\
* 
*  testNgram.c
*  Test the NGRAM functions
* 
\**************************************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "testframe.h"
#include "test.h"
#include "log.h"
#include "enigma.h"
#include "ngramScore.h"
#include "coincidence.h"

/**************************************************************************************************\
* 
* Test NGRAM score
* 
\**************************************************************************************************/
void testNgramPrepare()
{
    testStart("Prepare NGRAM");
    prepareNgramScore(3, "DE");
    assertIntEquals(17142, ngrams);

    assertStringEquals("AAA", ngramFrequencies[    0].ngram);
    assertStringEquals("***", ngramFrequencies[  257].ngram);
    assertStringEquals("AJY", ngramFrequencies[  258].ngram);
    assertStringEquals("ZZZ", ngramFrequencies[17575].ngram);

    assertIntEquals(3428 ,ngramFrequencies[    0].frequency);
    assertIntEquals(   8 ,ngramFrequencies[  258].frequency);
    assertIntEquals( 122 ,ngramFrequencies[17575].frequency);

    assertFloatEquals(3.785905E-6 ,ngramFrequencies[    0].chance);   // = 3428/905463743
    assertFloatEquals(-12.4842255 ,ngramFrequencies[    0].logChance);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test NGRAM score, against Excel
* 
\**************************************************************************************************/
void testNgramScore1()
{
    testStart("NGRAM Score 1");

    Enigma* enigma=createEnigmaM3();
    prepareNgramScore(3, "DE");
    enigma->textSize=3;
    enigma->conversion[0]='E'-'A';  // ENS - -5.618547
    enigma->conversion[1]='N'-'A';
    enigma->conversion[2]='S'-'A';
    assertFloatEquals(-5.618547, ngramScore(enigma));   
    
    enigma->textSize=8;
    enigma->conversion[0]='E'-'A';  //END -  -5.427283
    enigma->conversion[1]='N'-'A';  //NDE -  -4.939303
    enigma->conversion[2]='D'-'A';  //DEX -  -9.533161
    enigma->conversion[3]='E'-'A';  //EXL - -12.648394
    enigma->conversion[4]='X'-'A';  //XLI - -13.109703
    enigma->conversion[5]='L'-'A';  //LIC -  -6.167728
    enigma->conversion[6]='I'-'A';  
    enigma->conversion[7]='C'-'A';
    assertFloatEquals(-51.825572, ngramScore(enigma));   

    enigma->textSize=4;
    enigma->conversion[0]='A'-'A';  // AJX - not in list, hence -20.623959
    enigma->conversion[1]='J'-'A';  // JXX -                    -19.930811
    enigma->conversion[2]='X'-'A';
    enigma->conversion[3]='X'-'A';
    assertFloatEquals(-40.554770, ngramScore(enigma));   

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test NGRAM score 2
* 
\**************************************************************************************************/
void testNgramScore2()
{
    testStart("NGRAM Score 2");
    int i;

    char* text01="TOUGEVXHALJYNHDGTYFYTDQEWUNAKSGPUXLKTNIGGTJYFQNNVOOHMSQGMGMONGBNHBCYHJVMTERATBGMNUUQBPAXJWUAPKEHJJSL";
    char* text02="AUFBEFEHLDESOBERSTENBEFEHLSHABERSSINDIMFALLEXZXZTXUNWAHRSCHEINLICHENXFRANZOESISQENANGRIFFSDIEWESTBEF";

    Enigma* enigma=createEnigmaM3();

    prepareNgramScore(3, "DE");

    enigma->textSize=100;
    for(i=0;i<100;i++)
    {
        enigma->conversion[i]=text01[i]-'A';
    }
    logDebug("Random text of 100 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-1371.680786, ngramScore(enigma)); 
    for(i=0;i<100;i++)
    {
        enigma->conversion[i]=text02[i]-'A';
    }
    logDebug("German text of 100 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-857.664429, ngramScore(enigma)); 
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test NGRAM score 2
* 
\**************************************************************************************************/
void testNgramScore3()
{
    testStart("NGRAM Score 3");
    int i;

    Enigma* enigma=createEnigmaM3();

    prepareNgramScore(3, "DE");

    enigma->textSize=200;
    for(i=0;i<200;i++)
    {
        enigma->conversion[i]=testTextRandom[i]-'A';
    }
    logDebug("Random text of 200 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-2810.236084, ngramScore(enigma)); 
    for(i=0;i<200;i++)
    {
        enigma->conversion[i]=testTextGerman[i]-'A';
    }
    logDebug("German text of 200 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-1446.260132, ngramScore(enigma)); 
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test NGRAM score 4 - English bi and trigrams
* 
\**************************************************************************************************/
void testNgramScore4()
{
    testStart("NGRAM English");

    Enigma* enigma=createEnigmaM3();
    prepareNgramScore(3, "GB");

    enigma->textSize=1000;
    for(int i=0;i<1000;i++)
    {
        enigma->conversion[i]=testTextRandom[i]-'A';
    }
    logDebug("Random text of 1000 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-13153.831055, ngramScore(enigma)); 
    for(int i=0;i<1000;i++)
    {
        enigma->conversion[i]=testTextEnglish[i]-'A';
    }
    logDebug("English text of 1000 chars: %f\n", ngramScore(enigma));
    assertFloatEquals(-7958.878906, ngramScore(enigma)); 
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/

char testCipher[MAX_TEXT];

char failResult[]=
    "QAQPEQEUDSCHEDMILHDURSEINEWIEQEUHPLUIGQERINZWUSGNE"
    "NBEKANNDEEUOEXENENCEYVDPGRAVQISEHENJADASDQUVHEQESE"
    "RSDEIWELHKRIEGSUNGQQINGDOERMEIQENWPLLDENBEDEJCHDED"
    "ENSFEQNECEUZUUNTERMESCHINELLENOERSCHLUSSELUNGDLEQI";

void testNgramHillClimb()
{
    testStart("NGRAM Hillclimb");

    // Create cipher
    Enigma* enigma=createEnigmaM3();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");
    placeUmkehrWalze(enigma, "UKW B");
    setRingStellungen(enigma, "XTR");
    setGrundStellungen(enigma, "AER");
    setText(enigma, testTextGerman);
    placeSteckers(enigma, "ab cl df er gk op yz");
    encodeDecode(enigma);    
    strncpy(testCipher, toString(enigma), MAX_TEXT-1);
    destroyEnigma(enigma);
    
    printf("Cipher %s\n", testCipher);
    IocRecipe* recipe=createDefaultIocRecipe(testCipher, 1);

    // Define recipe using NGRAM for letterfinding -> succeed
    LinkedList*     permutations=linkedListCreate();
    int* permutation   =malloc(4*sizeof(int)); 
    permutation[0]=1; // Index of UKW B
    permutation[1]=0; // I
    permutation[2]=1; // II
    permutation[3]=2; // III
    linkedListAppendObject(permutations, permutation); 
    recipe->evalWalzen       =EVAL_IOC;
    recipe->evalSteckers     =EVAL_NGRAM;
    recipe->maxSteckers      =8;
    recipe->ngramSize        =3;
    strncpy(recipe->ngramSet, "DE", 3);
    recipe->scoreListSize    =10;
    recipe->numberOfSolutions=1;
    EnigmaSettings* bestSettings=iocDecodeText(*recipe, permutations);

    enigma=createEnigmaM3();
    setEnigma(enigma, bestSettings);
    encodeDecode(enigma);
    assertStringEquals(testTextGerman, toString(enigma));
    destroyEnigma(enigma);

    // Define recipe using IOC for letterfinding -> fail
    permutations=linkedListCreate();
    permutation   =malloc(4*sizeof(int)); 
    permutation[0]=1; // Index of UKW B
    permutation[1]=0; // I
    permutation[2]=1; // II
    permutation[3]=2; // III
    linkedListAppendObject(permutations, permutation); 
    recipe->evalSteckers     =EVAL_IOC;
    bestSettings=iocDecodeText(*recipe, permutations);
    enigma=createEnigmaM3();
    setEnigma(enigma, bestSettings);
    encodeDecode(enigma);
    assertStringEquals(failResult, toString(enigma));
    destroyEnigma(enigma);

    destroyIocRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testNgram()
{
    moduleTestStart("NGRAM");
    testNgramPrepare();
    testNgramScore1();
    testNgramScore2();
    testNgramScore3();
    testNgramScore4();
    testNgramHillClimb();
    moduleTestWrapUp();
}