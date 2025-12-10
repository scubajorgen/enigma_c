/**************************************************************************************************\
*
* test.c
* Module tests
*
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>

#include "log.h"
#include "testframe.h"
#include "test.h"
#include "turing.h"
#include "toolbox.h"

/**************************************************************************************************\
* 
* Test turing bombe without knowledge on where to look for the crib, engima nano
* 
\**************************************************************************************************/
char* testTuringCipher6    =
                            "CRSMDDACIGRLKUPAHWCYFGDSEPBHSYPYYDDNWUDMBRKT"
                            "GPDULRSDTRWWYWVLWKVRIBHHWFVCDXVIEREHLGSQSBIQU"
                            "DGACRUFWBMYRSPYGBKESJLNNFTVAGSOXBDIOXJACSJRSAV"
                            "VXDAWBIMEDLQSLQKLFHATDCUGFPOYSBKPBSFLRHUPXDMM"
                            "TTDPRKZCOCNZDSULIHEJGLCLVQWXVAZZPWMOOKPPFWDSQ"
                            "VW";      
char* testTuringCrib6_1    ="NORDZWEIUNDFUNFZIGGRADEIN";
char* testTuringCrib6_2    ="DREIUNDFUNFZIGGRADEINUNDZWANZ";
char* testTuringCrib6_3    ="NORDXFUNFXDREIXGRADXZWEIXEINS";
char* testTuringCrib6_4    ="NORDFUNFDREIGRADZWEIXEINSPUNKT";
char* testTuringCrib6_5    ="FUNFDREIGRADZWEIEINSPUNKTVIER";

char* testTuringCrib6_6    ="DERCACHEBEFINDETSICHANDER"; //!
char* testTuringCrib6_7    ="DERCACHEBEFINDETSICHAUFDEM";
char* testTuringCrib6_8    ="DERCACHEBEFINDETSICHAUFDER";
char* testTuringCrib6_9    ="DERCACHEBEFINDETSICHAM";

void testTuringBombe6()
{
    testStart("Turing Bombe++");

    // Create own permutations - only UKW B used
    LinkedList* walzen=getWalzenPermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    LinkedList* ukws  =linkedListCreate();
    int* ukw=malloc(sizeof(int));
    *ukw=1;
    linkedListAppendObject(ukws, ukw);
    LinkedList* permutations=combinePermutations(ukws, 1, walzen, 3);
    linkedListDestroy(walzen, true);
    linkedListDestroy(ukws, true);

    logInfo("Number of permutations: %d", linkedListLength(permutations));

    // Request scanning
    TuringRecipe* recipe        =createDefaultTuringRecipe(testTuringCipher6, testTuringCrib6_6, -1, 1);
    recipe->R1                  ='A'; // we now Ringstellungen used are all 1
    recipe->startR2             ='A';
    recipe->endR2               ='A';
    recipe->startR3             ='A';
    recipe->endR3               ='A';
//    recipe->startCribPosition   =0;
//    recipe->endCribPosition     =10;
    recipe->customPermutations  =permutations;
    LinkedList* results         =linkedListCreate();
    turingBombe(*recipe, results, NULL);
    assertIntEquals(2, linkedListLength(results));

    turingReport(MESSAGEFORMAT_TEXT);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}


void testTuringBombe6test()
{
    turingFindCribCircles(testTuringCipher6, testTuringCrib6_1, 92, false);
    dumpSets();

    // UKW B - I II III R 1 1 1, G 1 7 20, FO NX RS
    // UKW B - I II III R 1 1 1, G 1 7 23, AY DK FO NZ RQ UV
    // UKW B - I II III R 1 1 1, G 14 8 5, AD FE NY RJ UZ
}




/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

int main()
{
    setLogLevel(LOGLEVEL_INFO);
    testSessionStart("CRACK");

    testTuringBombe6();

    testSessionWrapUp();
    return 0;
}
