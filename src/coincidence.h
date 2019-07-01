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

#define TOP_RESULTS_SIZE    25
#define MAX_THREADS         8
#define MAX_WORK_ITEMS      32


typedef enum 
{
    METHOD_IOC,             // James Gillogly
    METHOD_IOC_DEEP,        // Improved James Gillogly method
    METHOD_IOC_NGRAM        // Improved James Gillogly method combined with ngrams
} method_t;



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


void    setEvaluationMethod (method_t method, int maxSteckers, int maxSteckersIoc, int ngramSize, char* ngrams);
void    iocDecodeText       (char* cypher, int numOfThreads);
void    iocExecuteWorkItems (int numOfThreads, LinkedList* permutations);
void    iocExample          ();
void    iocExampleDeep1     ();
void    iocExampleDeep2     ();
void    iocExampleNgram     ();
void    ngramTest           ();
void    ngramTest2          ();
void    ngramTest3          ();