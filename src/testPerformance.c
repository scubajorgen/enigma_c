#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>

#include "log.h"
#include "testframe.h"
#include "test.h"


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

int main()
{
    setLogLevel(LOGLEVEL_INFO);
    testSessionStart("PERFORMANCE TEST");
    testEnigmaPerformance();
    testSessionWrapUp();
    return 0;
}
