#include <stdio.h>

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
    printf("#####################################################################################\n");
    printf("# ENIGMA SIMULATION\n");
    printf("#####################################################################################\n");
    
	
    // Simple encode/decode example v
    //enigmaExample();   
    
    // The Turing method: find the crib loops and show they work v
    //turingProve();
   
    // The Turing method: working example -> readable message, not all steckersettings are found
    //turingExample();

    // The James Gillogly method example v
    //iocExample();

    // Deep method example - Gillogly text v
    //iocExampleDeep1();

    // Deep  method example - short message -> readable message, few misfits within steckered chars
    //iocExampleDeep2();

    // NGRAM  method example - short message v
    iocExampleNgram();
    
    // NGRAM method example: just find the steckers
    // ngramTest();


    return 0;
}
