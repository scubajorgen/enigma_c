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
char turingCypher[]     ="RPVPZILDGRNOPPLOFZNRUALXKHEXLDMQYCDFAQ";

char testGrundStellung[]="22 17 12";
char testRingStellung []="01 24 03";
char testWaltzen[3][5]  ={"I", "II", "III"};
char testUkw[]          ="UKW B";
char testSteckers[]     ="bq cr di ej kw mt os px uz gh";

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
    printf("Cypher    : %s\n", turingCypher);
    printf("Plain text: %s\n", turingCrib);
    
    turingFindLoops(turingCypher, turingCrib);
    
    dumpMenu();
    dumpSets();
    
    enigma=createEnigmaM3();
    
    placeWaltze         (enigma, 1, testWaltzen[0]);
    placeWaltze         (enigma, 2, testWaltzen[1]);
    placeWaltze         (enigma, 3, testWaltzen[2]);
    setRingStellungen   (enigma, testRingStellung);
    
    placeSteckers       (enigma, testSteckers);
    placeUmkehrWaltze   (enigma, testUkw);
    
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
                setGrundStellungen  (enigma, testGrundStellung);
                advances(enigma, cribCircle->advances[step]-1);

                encodeDecode        (enigma);
                decoded=toString    (enigma);
                temp[0]=decoded[0];
                step++;
            }
            printf("output: %s\n", decoded);
            circle++;
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
    printf("# Cypher                    : %s\n", turingCypher);
    printf("# Crib                      : %s\n", turingCrib);
    printf("# Original Waltzen          : %s %s %s\n", testWaltzen[0], testWaltzen[1], testWaltzen[2]);
    printf("# Original RingStellungen   : %s\n", testRingStellung);
    printf("# Original GrundStellungen  : %s\n", testGrundStellung);
    printf("# Original Steckers         : %s\n", testSteckers);
    printf("#####################################################################################\n");

    turingBombe(turingCypher, turingCrib, 1);
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





