/**************************************************************************************************\
*
* crack.c
* Example enigma cipher crack, using Turing method. 
* Geocaching 'Engima Nano' https://www.geocaching.com/geocache/GC6ZZBB
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


char tries[17][MAX_CRIB_SIZE]=
{
//   123456789012345678901234567890
    "NORDDREIUNDFUNFZIGGRADEINUND",
    "DREIUNDFUNFZIGGRADEINUNDZWANZIG",
    "NORDXFUNFXDREIXGRADXZWEIXEINS",
    "NORDFUNFDREIGRADZWEIXEINSPUNKT",
    "FUNFDREIGRADZWEIEINSPUNKTVIER",
    "NULNULSIEBENEINSNULFUNF",
    "NULNULSIEBENEINSNULVIER",
    "DERCACHEBEFINDETSICHANDER", //!
    "DERCACHEBEFINDETSICHAUFDEM",
    "DERCACHEBEFINDETSICHAUFDER",
    "DERCACHEBEFINDETSICHAM",
    "MANKANNDENCACHEFINDEN",
    "SIEWERDENDENCACHEFINDEN",
    "DukannstdenCacheloggen",
    "DIEKOORDINATEN",
    "DiesenCachedarfmanloggen",
    "NORTHFIFTYTHREEDEGREESTWENTYONE"
};


void testTuringBombeCrack()
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
    TuringRecipe* recipe        =createDefaultTuringRecipe(testTuringCipher6, tries[0], -1, 1);
    recipe->R1                  ='A'; // we now Ringstellungen used are all 1
    recipe->startR2             ='A';
    recipe->endR2               ='B';
    recipe->startR3             ='A';
    recipe->endR3               ='A';
    recipe->customPermutations  =permutations;


logInfo("MAX CRIB SIZE %d %d", MAX_CRIB_SIZE, strlen(recipe->crib));
    TuringStats statistics;
    bool found=false;
    for(int i=0; i<sizeof(tries)/MAX_CRIB_SIZE && !found; i++)
    {
        logInfo("_____________________________________________________________________");
        logInfo("Trying %s", tries[i]);
        memcpy(recipe->crib, tries[i], MAX_CRIB_SIZE);
        LinkedList* results         =linkedListCreate();
        turingBombe(*recipe, results, &statistics);
        assertIntEquals(2, linkedListLength(results));
        turingReport(MESSAGEFORMAT_TEXT);
        turingReportBombeStatistics(&statistics);
        if (statistics.solutions>0)
        {
            found=true;
        }
        linkedListDestroy(results, true);
    }
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);

    testWrapUp();
}


void testTuringBombe6test()
{
    turingFindCribCircles(testTuringCipher6, tries[5], 92, false);
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

    testTuringBombeCrack();

    testSessionWrapUp();
    return 0;
}
