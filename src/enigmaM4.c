#include <stdio.h>
#include <malloc.h>

#include "enigma.h"

Enigma* createEnigmaM4(void)
{
    Enigma* enigma;
    
    enigma                  =malloc(sizeof(Enigma));
    enigma->numberOfRotors  =4;
    
    return enigma;
}

