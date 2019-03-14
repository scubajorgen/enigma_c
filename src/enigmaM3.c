#include <stdio.h>
#include <malloc.h>

#include "enigma.h"

Enigma* createEnigmaM3(void)
{
    Enigma* enigma;
    
    enigma                  =malloc(sizeof(Enigma));
    enigma->numberOfRotors  =3;
    
    return enigma;
}

void destroyEnigma(Enigma* enigma)
{
    free(enigma);
}