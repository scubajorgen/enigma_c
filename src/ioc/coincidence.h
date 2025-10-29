/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software tries all rotor settings, 
* RingStellungen included 
*
* Basically, the method consists of following steps
* 1. Finding the rotor position and Grundstellung with the best IoC value, fixed Ringstellung
* 2. Finding the best Ringstellung/Grundstellung with the best IoC value
* 3. Finding the steckers resulting in the best IoC value
*
* This implementation allows for variations on these steps, going in the direction of brute forcing
* 1. Use NGRAM scoring i.s.o. IoC
* 2. Vary Ringstellungen as well
* 3. Use steckers during step 1.
*
\**************************************************************************************************/

#include "toolbox.h"

/**************************************************************************************************\
* DEFINES
\**************************************************************************************************/

#define TOP_RESULTS_SHOW    5
#define TOP_RESULTS_SIZE    500
#define MAX_THREADS         10
#define MAX_WORK_ITEMS      32
#define MAX_NGRAMSETSIZE    5

typedef enum 
{
    DEPTH_NONE,             // James Gillogly, original described method, RING STELLUNG fixed to 1 1 1
    DEPTH_R3,               // James Gillogly, RING STELLUNG R1 and R2 fixed, varying R3
    DEPTH_R2_R3             // James Gillogly, RING STELLUNG R1 fixed, varying R2 and R3
} Depth_t;

typedef enum 
{
    EVAL_IOC,         // Evaluation by index of coincidence, James Gillogly
    EVAL_NGRAM        // Evaluation by bigram or trigrams
} Evaluation_t;


typedef struct
{
    Enigma_t        enigmaType;
    RotorSet_t      rotorSet;           
    Depth_t         method;                     // The method
    Evaluation_t    evalWalzen;                 // Evaluation to use for finding the Walzen, Ringstellungen and Grundstellungen
    Evaluation_t    evalSteckers;               // Evaluation to use for finding Steckers
    int             maxSteckersInline;          // Maximum number of Steckers to try during finding of rotor positions
    int             maxSteckers;                // Maximum number of Steckers to try
    int             ngramSize;                  // NGRAMS: NGRAM size: 2 or 3
    char            ngramSet[MAX_NGRAMSETSIZE]; // NGRAM set: "DE", "EN", "GC"
} IocRecipe;

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
void    iocEvaluateEngimaSettings   (IocWorkItem* work);
void    iocFindSteckeredChars       (IocResults* results, int maxNumOfSteckers);
void    iocFindSteckeredCharsNgram  (IocResults* results, int maxNumOfSteckers, int ngramSize);
void    iocDumpTopResults           (int number, int withDecode);
void    iocFindRingStellung         (IocResults*  results, int startRotor, int endRotor);
void    iocWorkerFunction           (int worker, int workItem, void* params);
void    iocFinishFunction           (void* params);
void    setWalzePermutations        (LinkedList* permutations);

// Public methods
void    setOperation                (IocRecipe recipe);
void    iocDecodeText               (char* cipher, int numOfThreads);

