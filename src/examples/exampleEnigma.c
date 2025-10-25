/**************************************************************************************************\
* 
* This file presents a number of Engima examples
*
\**************************************************************************************************/
#include <stdio.h>

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
    
    printf("Original  : %s \nCipher    : %s\n", someText, cipher);

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
    
    printf("Original  : %s \nCipher    : %s\n", someText, cipher);

    setText             (enigma, cipher);
    
    setGrundStellungen  (enigma, "01 02 03 04");
    
    encodeDecode(enigma);

    plain=toString(enigma);
    
    printf("Plain text: %s\n", plain);
    
    destroyEnigma(enigma);
}