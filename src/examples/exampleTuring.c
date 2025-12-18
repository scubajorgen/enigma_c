/**************************************************************************************************\
* 
* A number of examples on how to use the Turing bombe
* 
\**************************************************************************************************/
#include <stdio.h>
#include <malloc.h>

#include "log.h"
#include "toolbox.h"
#include "enigma.h"
#include "turing.h"

// EXAMPLE SET 1
char turingCrib[]       ="WETTERVORHERSAGEBISKAYA";
char turingCribPosition =0;
EnigmaSettings turingTestSettings=
{
    3,
    {
        "I",
        "II",
        "III"
    },
    "UKW B",
    {
        1, 24, 3
    },
    {
        22, 17, 12
    },
    "bq cr di ej kw mt os px uz gh",
      "RPVPZILDGRNOPPLOFZNRUALUGCBJFXYNJCFDCOIUMGABPODMHQGVRFW"
    // WETTERVORHERSAGEBISKAYA  
};

// EXAMPLE SET 2
char turingCrib2[]       ="VONBDUXPLANUNGOPERATIONJPAUKENSCH";
char turingCribPosition2 =0;
EnigmaSettings turingTestSettings2=
{
    3,
    {
        "II",
        "IV",
        "I"
    },
    "UKW C",
    {
        1, 3, 21
    },
    {
        3, 11, 5
    },
    "bd cv el gn iz jo kw mt pr sx",
    "KGBJNTWBQYFFJWQKKCTNZJVRKBWPQOFZQTBLCYCMWCWTRXSGKA"
};

// EXAMPLE SET 3
char turingCrib3[]       ="XOBERSTESGEHEIMNISX";
char turingCribPosition3 =21;
EnigmaSettings turingTestSettings3=
{
    3,
    {
        "III",
        "I",
        "II"
    },
    "UKW B",
    {
        1, 4, 13
    },
    {
        4, 17, 10
    },
    "bq cr di ej kw mt os px uz gh",
    "LUYOPWJORDOPKGIJHTHPWKELDLXAWNJNVCMAVHIOCJXWCYKKZUIMUXLHCCECSGSCCUYAOQJZXYS"
    "NLRUWBYXSFIRPDGCEOMKSSEZQFKITYSXOCIYYFRRJNIDIWXWRUIJITMGJNBWPJHJEXDJDAWOWQD"
    "ZWVWFBYALXZSLGLMPDYFIFXRQRKQRXNPRULEYTMFXDRMBEWDNPHJFLYCPTCQJHDPNHHHMGQRWQL"
    "LSJFUIKVAHLWCODRIRCDTBGOZXEIXJDCVKHNZQHOTVMVQYRRDVCH"
};

/**************************************************************************************************\
* 
* Just a test to validate the crib cricle sets, with knowledge of the settings of the Enigma
* that generated the crib. Prints the letter links and crib loops
* 
\**************************************************************************************************/
void turingProve()
{
    Enigma*     enigma;
    char*       decoded;
    char        temp[3];
    int         circle;
    int         step;
    int         set;
    CribCircle*     cribCircle;
    
    printf("#####################################################################################\n");
    printf("# TURING PROVE\n");
    printf("# Inventory of links, finding loops and proving they work\n");
    printf("#####################################################################################\n\n");

    printf("            123456789012345678901234567890\n");    
    printf("Cipher    : %s\n", turingTestSettings.cipher);
    printf("Plain text: %s\n", turingCrib);
    
    turingFindCribCircles(turingTestSettings.cipher, turingCrib, 0, false);
    
    printf("\nMENU (LINKS)\n");
    dumpMenu();

    printf("\n\nCRIB CRICLES\n");
    dumpSets();

    printf("\n\nGOING THE CRIBCIRCLES AROUND\n");
    enigma=createEnigmaM3();

    setEnigma(enigma, &turingTestSettings);
    
    temp[1]='\0';

    decoded=NULL;
    
    set=0;
    while (set<MAX_POSITIONS)
    {
        circle=0;
        while (circle<cribCircleSet[set].numOfCircles)
        {
            cribCircle=&cribCircleSet[set].cribCircles[circle];
            
            temp[0]=cribCircle->orgChars[0];
            printf("Letter %c, Circle %02i: Input: %s, ", cribCircleSet[set].startChar, circle, temp);
     
            step=0;
            while (step<cribCircle->circleSize)
            {
                setText(enigma, temp);
                setGrundStellung(enigma, 1, turingTestSettings.grundStellungen[0]);
                setGrundStellung(enigma, 2, turingTestSettings.grundStellungen[1]);
                setGrundStellung(enigma, 3, turingTestSettings.grundStellungen[2]);

                advances(enigma, cribCircle->advances[step]-1);

                encodeDecode        (enigma);
                decoded=toString    (enigma);
                temp[0]=decoded[0];
                step++;
            }
            printf("output: %s\n", decoded);
            circle++;
        }
        if (cribCircleSet[set].numOfCircles>0)
        {
            printf("\n");
        }
        set++;
    }

   
    destroyEnigma(enigma);    
}

/**************************************************************************************************\
* 
* An example
* 
\**************************************************************************************************/
void turingExample()
{
    printEnigmaSettings(&turingTestSettings, "TURING BOMBE EXAMPLE 1");
    logReport("# Crib                      : %s",        turingCrib);
    logReport("# Crib position             : %d",        turingCribPosition);
    logReport("#####################################################################################");

    TuringRecipe*   recipe          =createDefaultTuringRecipe(turingTestSettings.cipher, turingCrib, turingCribPosition, 4);
    LinkedList*     results         =linkedListCreate();
    TuringStats     statistics;
    recipe->terminateAtFirstSolution=true;
    turingBombe(*recipe, results, &statistics);
    turingReport(MESSAGEFORMAT_TEXT);
    turingReportBombeStatistics(&statistics);
    linkedListDestroy(results, true);
    destroyTuringRecipe(recipe);
}

/**************************************************************************************************\
* 
* Another example. 25-26 character crib results in almost no false negatives
* 
\**************************************************************************************************/
void turingExample2()
{
    // II IV I, UKW C, R 1 3 21 G 3 11 5, bd cv el gn iz jo kw mt pr sx
    printEnigmaSettings(&turingTestSettings2, "TURING BOMBE EXAMPLE 2");
    logReport("# Crib                      : %s",        turingCrib2);
    logReport("# Crib position             : %d",        turingCribPosition2);
    logReport("#####################################################################################");
    TuringRecipe* recipe=createDefaultTuringRecipe(turingTestSettings2.cipher, turingCrib2, turingCribPosition2, 4);
    recipe->startR2='A';
    recipe->endR2='E';
    LinkedList* results=linkedListCreate();
    TuringStats statistics;
    turingBombe(*recipe, results, &statistics);
    turingReport(MESSAGEFORMAT_TEXT);
    turingReportBombeStatistics(&statistics);
    linkedListDestroy(results, true);
    destroyTuringRecipe(recipe);
}

/**************************************************************************************************\
* 
* Another example with short crib (19 letters); only 9 out of 10 steckers are found.
* 
\**************************************************************************************************/
void turingExample3()
{
    // II IV I, UKW C, R 1 3 21 G 3 11 5, bd cv el gn iz jo kw mt pr sx
    printEnigmaSettings(&turingTestSettings3, "TURING BOMBE EXAMPLE 3");
    logReport("# Crib                      : %s",        turingCrib3);
    logReport("# Crib position             : %d",        turingCribPosition3);
    logReport("#####################################################################################");
    TuringRecipe*   recipe      =createDefaultTuringRecipe(turingTestSettings3.cipher, turingCrib3, turingCribPosition3, 4);
    recipe->startR2='M';
    recipe->endR2='M';
    LinkedList*     results     =linkedListCreate();
    TuringStats     statistics;
    turingBombe(*recipe, results, &statistics);
    turingReport(MESSAGEFORMAT_TEXT);
    turingReportBombeStatistics(&statistics);
    linkedListDestroy(results, true);
    destroyTuringRecipe(recipe);
}

/**************************************************************************************************\
* 
* Test Enigma NANO, the reply
* 
\**************************************************************************************************/
extern char replyGC6ZZBB[];

void turingExample4()
{
    TuringStats     statistics;
    TuringRecipe*   recipe          =createDefaultTuringRecipe(replyGC6ZZBB, "whenthiscachecameonlineitimmediately", 0, 4);
    LinkedList*     results         =linkedListCreate();
    recipe->terminateAtFirstSolution=true;
    turingBombe(*recipe, results, &statistics);
    turingReport(MESSAGEFORMAT_TEXT);
    turingReportBombeStatistics(&statistics);
    linkedListDestroy(results, true);
    destroyTuringRecipe(recipe);
}



