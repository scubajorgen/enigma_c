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
    char*       cypher;                 // The Cypher
    LinkedList* permutations;           // List of Waltzen permutations
    int         startPermutation;       // Permutation start
    int         endPermutation;         // permutation end; end included
    int         startR2;                // RingStellung 2 start
    int         endR2;                  // RingStellung 2 end; end included
    int         startR3;                // RingStellung 3 start
    int         endR3;                  // RingStellung 3 end; end included
    int         maxCypherChars;         // Part of the message to process; MAX_TEXT for full message
    char        ukw[MAX_ROTOR_NAME];    // UKW to use
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
void    iocExampleDeep1     ();
void    iocExampleDeep2     ();