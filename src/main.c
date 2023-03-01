#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigma.h"
#include "exampleTuring.h"
#include "exampleIoc.h"
#include "crack.h"


void enigmaExample()
{
    char    someText[]="TEST MESSAGE ENCODED ON ENIGMA";
    char*   cypher; 
    char*   plain;

    printf("#####################################################################################\n");
    printf("# ENIGMA ENCODE/DECODE EXAMPLE\n");
    printf("#####################################################################################\n");

        
    Enigma* enigma;
    
    enigma=createEnigmaM3();
    
    placeWaltze         (enigma, 1, "I");
    placeWaltze         (enigma, 2, "II");
    placeWaltze         (enigma, 3, "III");
    setRingStellungen   (enigma, "26 13 01");
    setGrundStellungen  (enigma, "01 02 03");
    
    placeSteckers       (enigma, "bq cr di ej kw mt os px uz gh");
    placeUmkehrWaltze   (enigma, "UKW B");
    
    setText             (enigma, someText);
    
    encodeDecode(enigma);
    
    cypher=toString(enigma);
    
    printf("Original  : %s \nCypher    : %s\n", someText, cypher);

    setText             (enigma, cypher);
    
    setGrundStellungen  (enigma, "01 02 03");
    
    encodeDecode(enigma);

    plain=toString(enigma);
    
    printf("Plain text: %s\n", plain);
    
    destroyEnigma(enigma);

}

int main()
{
    char input[10];
    int  i;
    
    printf("#####################################################################################\n");
    printf("# ENIGMA SIMULATION\n");
    printf("#####################################################################################\n");
    
	printf(" 1.     Enigma example                  \n");
	printf(" 2.     Turing crib circles/loops       \n");
	printf(" 3.     Turing example                  \n");
	printf(" 4.     Gillogly IoC example            \n");
	printf(" 5.     Gillogly IoC deep example       \n");

	printf("20-29.  Engima Challenge message 1-10   \n");


    fgets(input, 10, stdin);
    i=atoi(input);

    switch(i)
    {
        case 1: 
            // Simple encode/decode example v
            enigmaExample();           
            break;
        case 2:
            // The Turing method: find the crib loops and show they work v
            turingProve();
            break;        
        case 3: 
            // The Turing method: working example -> readable message, not all steckersettings are found
            turingExample();
            break;    
        case 4: 
            // The James Gillogly method example v
            iocExample();
            break;
        case 5:  
            // Deep method example - Gillogly text v
            iocExampleDeep1();
            break;
        case 20:
            message01();
            break;
        case 21:
            message02();
            break;
        case 22:
            message03();
            break;
        case 23:
            message04();
            break;
        case 24:
            message05();
            break;
        case 25:
            message06();
            break;
        case 26:
            message07();
            break;
        case 27:
            message08();
            break;
        case 28:
            message09();
            break;
        case 29:
            message10();
            break;
    }


    // Deep  method example - short message -> readable message, few misfits within steckered chars
    //iocExampleDeep2();

    // NGRAM  method example - short message v
    //iocExampleNgram();
    
    // NGRAM method example: just find the steckers v
    //ngramTest();

    //ngramTest2();
    //ngramTest3();
    //ngramTest4();
//    ngramTest5();

//theSixthTest();
//theSixthProblem();

//theFifthProblem();
//theFourthProblem();
//theThirdProblem();

//decode();

    return 0;
}
