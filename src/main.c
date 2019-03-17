#include <stdio.h>

#include "enigma.h"
#include "crack.h"






int main()
{
    printf("###############################################################\n");
    printf("ENIGMA SIMULATION\n");
    printf("###############################################################\n");
    
    char  encoded[]="vztuq ordcb sirgg pnwrl lwxvp";
    char* decoded;
        
    Enigma* enigma;
    
    enigma=createEnigmaM3();
    
    placeWaltze         (enigma, 1, "I");
    placeWaltze         (enigma, 2, "II");
    placeWaltze         (enigma, 3, "III");
    setRingStellungen   (enigma, "26 13 01");
    setGrundStellungen  (enigma, "01 02 03");
    
    placeSteckers       (enigma, "bq cr di ej kw mt os px uz gh");
    placeUmkehrWaltze   (enigma, "UKW B");
    
    setText             (enigma, encoded);
    
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    printf("Encoded: %s \nDecoded %s\n", encoded, decoded);
    
    destroyEnigma(enigma);
   
    
    return 0;
}
