/**************************************************************************************************\
* 
* A number of examples on how to use the Turing bombe
* 
\**************************************************************************************************/
#include <stdio.h>

#include "toolbox.h"
#include "enigma.h"
#include "turing.h"

// EXAMPLE SET
char turingCrib[]       ="WETTERVORHERSAGEBISKAYA";

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
    "RPVPZILDGRNOPPLOFZNRUALXKHEXLDMQYCDFAQ"
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
    printf("Cypher    : %s\n", turingTestSettings.cypher);
    printf("Plain text: %s\n", turingCrib);
    
    turingFindLoops(turingTestSettings.cypher, turingCrib);
    
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
    printf("\n");
    printf("#####################################################################################\n");
    printf("# TURING BOMBE\n");
    printf("# Cypher                    : %s\n",        turingTestSettings.cypher);
    printf("# Crib                      : %s\n",        turingCrib);
    printf("# Original Waltzen          : %s %s %s\n",  turingTestSettings.rotors[0],
                                                        turingTestSettings.rotors[1], 
                                                        turingTestSettings.rotors[2]);
    printf("# Original RingStellungen   : %d %d %d\n",  turingTestSettings.ringStellungen[0],
                                                        turingTestSettings.ringStellungen[1],
                                                        turingTestSettings.ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n",  turingTestSettings.grundStellungen[0],
                                                        turingTestSettings.grundStellungen[1],
                                                        turingTestSettings.grundStellungen[2]);
    printf("# Original Steckers         : %s\n",        turingTestSettings.steckers);
    printf("#####################################################################################\n");

    turingBombe(turingTestSettings.cypher, turingCrib, 1);
}

/**************************************************************************************************\
* 
* Another example. 25-26 character crib results in almost no false negatives
* 
\**************************************************************************************************/
void turingExample2()
{
    // II IV I, UKW C, R 1 3 21 G 3 11 5, bd cv el gn iz jo kw mt pr sx
    turingBombe("KGBJNTWBQYFFJWQKKCTNZJVRKBWPQOFZQTBLCYCMWCWTRXSGKA",
                "VONBDUXPLANUNGOPERATIONJPAUKENSCH", 3);    
}





