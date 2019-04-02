/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software tries all rotor settings, 
* RingStellungen included 
*
\**************************************************************************************************/

#include "toolbox.h"

/**************************************************************************************************\
* DEFINES
\**************************************************************************************************/

#define TOP_RESULTS_SIZE    10
#define MAX_THREADS         8
#define MAX_WORK_ITEMS      32

typedef struct
{
    int         state;
    char*       cypher;
    LinkedList* permutations;
    int         startPermutation;       // Permutation start
    int         endPermutation;
    int         startR2;
    int         endR2;
    int         startR3;
    int         endR3;
    int         maxCypherChars;
    char        ukw[MAX_ROTOR_NAME];
} IocWorkItem;


/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

extern IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
extern int                 iocNumberOfWorkItems;



/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/


void    iocDecodeText       (char* cypher, int numOfThreads, int isDeep);
void    iocExecuteWorkItems (int numOfThreads, int isDeep, LinkedList* permutations);
void    iocExample          ();
void    iocExampleDeep      ();