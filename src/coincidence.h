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
#define MAX_THREADS         10
#define MAX_WORK_ITEMS      32


typedef enum 
{
    METHOD_IOC,             // James Gillogly, original described method, RING STELLUNG fixed to 1 1 1
    METHOD_IOC_R3,          // James Gillogly, RING STELLUNG R1 and R2 fixed, varying R3
    METHOD_IOC_R2R3,        // James Gillogly, RING STELLUNG R1 fixed, varying R2 and R3
    METHOD_IOC_DEEP,        // Improved James Gillogly method
    METHOD_IOC_NGRAM        // Improved James Gillogly method combined with ngrams
} Method_t;


typedef struct
{
    char*       cipher;                 // The Cipher
    LinkedList* permutations;           // List of Walzen permutations
    int         startPermutation;       // Permutation start
    int         endPermutation;         // permutation end; end included
    int         R1;                     // Value of R1 to use
    int         startR2;                // RingStellung 2 start
    int         endR2;                  // RingStellung 2 end; end included
    int         startR3;                // RingStellung 3 start
    int         endR3;                  // RingStellung 3 end; end included
    int         maxCipherChars;         // Part of the message to process; MAX_TEXT for full message
    char        ukw[MAX_ROTOR_NAME];    // UKW to use
    int         threadId;               // Thread ID, to be filled in by the trhead
} IocWorkItem;

typedef struct
{
    float           indexOfCoincidence;
    EnigmaSettings  settings;
    int             steckerTable[MAX_POSITIONS];
} IocResults;

/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

extern IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
extern int                 iocNumberOfWorkItems;

// temp
extern IocResults          iocTopResults[TOP_RESULTS_SIZE];
extern int                 iocNumberOfResults;

/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/

// Methods for own cracking methods
void    iocEvaluateEngimaSettings   (IocWorkItem* work, int maxSteckers);
void    iocFindSteckeredChars       (IocResults* results, int maxNumOfSteckers);
void    iocFindSteckeredCharsNgram  (IocResults* results, int maxNumOfSteckers, int ngramSize);
void    iocDumpTopResults           (int withDecode);
void    iocFindRingStellung         (IocResults*  results, int startRotor, int endRotor);
void    iocWorkerFunction           (int worker, int workItem, void* params);
void    iocFinishFunction           (void* params);
void    setWalzePermutations        (LinkedList* permutations);

// Public methods
void    setEvaluationMethod         (Method_t method, int maxSteckers, int maxSteckersIoc, int ngramSize, char* ngrams);
void    iocDecodeText               (char* cipher, int numOfThreads);

