/**************************************************************************************************\
* 
* Enigma M3 
* 
\**************************************************************************************************/

#include <stdio.h>
#include <malloc.h>

#include "enigma.h"

/**************************************************************************************************\
* 
* Constructor
* 
\**************************************************************************************************/
Enigma* createEnigmaM3(void)
{
    Enigma* enigma;
    
    enigma                  =malloc(sizeof(Enigma));
    enigma->numberOfRotors  =3;
    
    return enigma;
}

/**************************************************************************************************\
* 
* Destructor
* 
\**************************************************************************************************/
void destroyEnigma(Enigma* enigma)
{
    free(enigma);
}