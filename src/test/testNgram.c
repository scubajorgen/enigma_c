/**************************************************************************************************\
* 
*  testNgram.c
*  Test the NGRAM functions
* 
\**************************************************************************************************/
#include <stdio.h>

#include "testframe.h"
#include "log.h"
#include "enigma.h"
#include "ngramscore.h"

/**************************************************************************************************\
* 
* Test NGRAM score
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
    assertFloatEquals(-1371.680908, ngramScore(enigma)); 
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
* Test main function
* 
\**************************************************************************************************/
void testNgram()
{
    moduleTestStart("NGRAM");
    testNgramScore1();
    testNgramScore2();
    moduleTestWrapUp();
}