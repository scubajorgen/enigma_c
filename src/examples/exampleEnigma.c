/**************************************************************************************************\
* 
* This file presents a number of Engima examples
*
\**************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "enigma.h"


/**************************************************************************************************\
* 
* Engima M3 Example
*
\**************************************************************************************************/
void enigmaExampleM3()
{
    char    someText[]="TEST MESSAGE ENCODED ON ENIGMA";
    char*   cipher; 
    char*   plain;

    printf("#####################################################################################\n");
    printf("# ENIGMA M3 ENCODE/DECODE EXAMPLE\n");
    printf("#####################################################################################\n");

        
    Enigma* enigma;
    
    enigma=createEnigmaM3();
    
    placeWalze          (enigma, 1, "I");
    placeWalze          (enigma, 2, "II");
    placeWalze          (enigma, 3, "III");
    setRingStellungen   (enigma, "26 13 01");
    setGrundStellungen  (enigma, "01 02 03");
    placeSteckers       (enigma, "bq cr di ej kw mt os px uz gh");
    placeUmkehrWalze    (enigma, "UKW B");
    setText             (enigma, someText);
    encodeDecode(enigma);
    cipher=toString(enigma);
    printf("Original  : %s\n", someText);
    printf("Cipher    : %s\n", cipher);

    setText             (enigma, cipher);
    setGrundStellungen  (enigma, "01 02 03");
    encodeDecode(enigma);
    plain=toString(enigma);
    printf("Plain text: %s\n", plain);
    
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Engima M3 Example
*
\**************************************************************************************************/
void enigmaExampleM4()
{
    char    someText[]="TEST MESSAGE ENCODED ON ENIGMA";
    char*   cipher; 
    char*   plain;

    printf("#####################################################################################\n");
    printf("# ENIGMA M4 ENCODE/DECODE EXAMPLE\n");
    printf("#####################################################################################\n");

        
    Enigma* enigma;
    
    enigma=createEnigmaM4();
    
    placeWalze          (enigma, 1, "Beta");
    placeWalze          (enigma, 2, "I");
    placeWalze          (enigma, 3, "II");
    placeWalze          (enigma, 4, "III");
    setRingStellungen   (enigma, "02 17 12 01");
    setGrundStellungen  (enigma, "01 02 03 04");
    placeSteckers       (enigma, "bq cr di ej kw mt os px uz gh");
    placeUmkehrWalze    (enigma, "UKW B2");
    setText             (enigma, someText);
    encodeDecode(enigma);
    cipher=toString(enigma);
    printf("Original  : %s\n", someText);
    printf("Cipher    : %s\n", cipher);

    setText             (enigma, cipher);
    setGrundStellungen  (enigma, "01 02 03 04");
    encodeDecode(enigma);
    plain=toString(enigma);
    printf("Plain text: %s\n", plain);
    
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Engima M3 Example - Random
*
\**************************************************************************************************/
void enigmaExampleRandomM3()
{
    char    someText[]="TEST MESSAGE ENCODED ON ENIGMA WITH RANDOM SETTINGS";
    char*   cipher; 
    char*   plain;

    printf("#####################################################################################\n");
    printf("# ENIGMA M3 ENCODE/DECODE EXAMPLE - Random Settings\n");
    printf("#####################################################################################\n");

    Enigma*         enigma;
    EnigmaSettings* settings;

    srand(time(NULL));
    enigma  =createEnigmaM3();
    settings=createRandomSettings(enigma, M3_ARMY_1938, 10);
    strncpy(settings->cipher, someText, MAX_TEXT);
    printEnigmaSettings(settings, "Enigma settings");
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    cipher=toString(enigma);
    printf("Original  : %s\n", someText);
    printf("Cipher    : %s\n", cipher);

    setEnigma(enigma, settings);    // reset
    setText(enigma, cipher);        // now put in cipher
    encodeDecode(enigma);
    plain=toString(enigma);
    printf("Plain text: %s\n", plain);
    
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Engima M3 Example - Random
*
\**************************************************************************************************/
void enigmaExampleRandomM4()
{
    char    someText[]="TEST MESSAGE ENCODED ON ENIGMA M4 SHARK WITH RANDOM SETTINGS";
    char*   cipher; 
    char*   plain;

    printf("#####################################################################################\n");
    printf("# ENIGMA M4 ENCODE/DECODE EXAMPLE - Random Settings\n");
    printf("#####################################################################################\n");

    Enigma*         enigma;
    EnigmaSettings* settings;

    srand(time(NULL));
    enigma  =createEnigmaM4();
    settings=createRandomSettings(enigma, M4_NAVAL_1941, 10);

    strncpy(settings->cipher, someText, MAX_TEXT);
    printEnigmaSettings(settings, "Enigma settings");
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    cipher=toString(enigma);
    printf("Original  : %s\n", someText);
    printf("Cipher    : %s\n", cipher);

    setEnigma(enigma, settings);    // reset
    setText(enigma, cipher);        // now put in cipher
    encodeDecode(enigma);
    plain=toString(enigma);
    printf("Plain text: %s\n", plain);
    
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);
}