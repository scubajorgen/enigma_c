/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software tries all rotor settings, 
* RingStellungen included 
*
* Basically, the method consists of following steps
* 1. Finding the rotor position and Grundstellung with the best IoC value, fixed Ringstellung (1-1-1)
* 2. Finding the best Ringstellung/Grundstellung with the best IoC value
* 3. Finding the steckers resulting in the best IoC value
*
* This implementation allows for variations on these steps, going in the direction of brute forcing
* 1. Use NGRAM scoring i.s.o. IoC
* 2. Vary Ringstellungen as well
* 3. Use steckers during step 1.
*
* The programm calculates a list of N solutions that score highest.
* A different number can be used for for the 1st 2 steps (e.g. N1=100) and for the 3rd step 
* (e.g. N2=5). 
* Following are the assumptions based on observation:
* 1. After step 1 the solution is on the list, but may be down below (e.g. place 54)
* 2. After step 2 the solution is on top of the list (e.g. place 1 or 2)
* 3. Step 3 may lead to false positives if not applied to other configurations than the solution
* Because of the list is fixed (its order doesn't change anymore based on score) and 
* step 3 (finding steckers) is applied to the N2 items on top of the list
* At step 2 the order of the list may change on improvement of IoC due to ring settings.
* At step 3 the order of the list will not change
*
\**************************************************************************************************/

#include "toolbox.h"

/**************************************************************************************************\
* DEFINES
\**************************************************************************************************/

#define MAX_THREADS             10
#define MAX_WORK_ITEMS          32
#define MAX_NGRAMSETSIZE        5

typedef enum 
{
    DEPTH_NONE,             // James Gillogly, original described method, RING STELLUNG fixed to 1 1 1
    DEPTH_R3,               // James Gillogly, RING STELLUNG R1 and R2 fixed, varying R3
    DEPTH_R2_R3             // James Gillogly, RING STELLUNG R1 fixed, varying R2 and R3
} Depth_t;

typedef enum 
{
    EVAL_IOC,               // Evaluation by index of coincidence, James Gillogly
    EVAL_NGRAM              // Evaluation by bigram or trigrams
} Evaluation_t;


typedef struct
{
    Enigma_t        enigmaType;
    RotorSet_t      rotorSet;           
    Depth_t         method;                             // The method
    Evaluation_t    evalWalzen;                         // Evaluation to use for finding the Walzen, Ringstellungen and Grundstellungen
    Evaluation_t    evalSteckers;                       // Evaluation to use for finding Steckers
    int             maxSteckers;                        // Maximum number of Steckers to try
    int             maxSteckersInline;                  // Maximum number of Steckers to try during finding of rotor positions
    char            knownSteckers[MAX_STECKER_STRING];  // If steckers are known, mention them here e.g. "AB DE", else "". USE CAPITALS
    int             ngramSize;                          // NGRAMS: NGRAM size: 2, 3 or 4
    char            ngramSet[MAX_NGRAMSETSIZE];         // NGRAM set: "DE", "EN", "GC"
    int             scoreListSize;                      // The number of solutions to take into account for finding Walzen (N1)
    int             numberOfSolutions;                  // The number of solutions to show (N2)
    int             numberOfThreads;                    // Number of threads to use
    char*           cipher;                             // Cipher to decrypt
    int             cipherSize;                         // Length of the cipher to take into account or MAX_TEXT for full cipher
    MessageFormat_t displayFormat;                      // Formatting for display of decoded text
} IocRecipe;

typedef struct
{
    int             startPermutation;           // Permutation start
    int             endPermutation;             // permutation end; end included
    int             R1;                         // Value of R1 to use
    int             startR2;                    // RingStellung 2 start
    int             endR2;                      // RingStellung 2 end; end included
    int             startR3;                    // RingStellung 3 start
    int             endR3;                      // RingStellung 3 end; end included
    int             maxCipherChars;             // Part of the message to process; MAX_TEXT for full message
    int             threadId;                   // Thread ID, to be filled in by the trhead
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
extern IocResults*         iocHighScores;
extern int                 iocNumberOfResults;

/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/

// Methods for own cracking methods
float       iocIndexOfCoincidence           (Enigma* enigma);
void        iocEvaluateEngimaSettings       (IocWorkItem* work);
void        iocFindSteckeredChars           (IocResults* results, int maxNumOfSteckers);
void        iocFindSteckeredCharsNgram      (IocResults* results, int maxNumOfSteckers, int ngramSize);
void        iocFindRingStellung             (IocResults*  results, int startRotor, int endRotor);
void        iocWorkerFunction               (int worker, int workItem, void* params);
void        iocFinishFunction               (void* params);

// Public methods
IocRecipe*  createDefaultRecipe             (char* cipher, int numberOfThreads);
void        destroyRecipe                   (IocRecipe* recipe);
void        iocInitialize                   (IocRecipe recipe, LinkedList* permutations);
void        iocDecodeText                   (IocRecipe recipe, LinkedList* customPermutations);
void        iocReportMethod                 ();

