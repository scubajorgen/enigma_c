
/**************************************************************************************************\
* 
* This file implements the method turing used to crack enigma encoded messages. It assumes a crib:
* a cipher text with corresponding plain text. Both are transformed into a 'menu' containing
* 'crib circles' or 'loops'. Crib circles are used validate hypotheses regarding the steckered
* letters: a the hypothesis proves right if the steckered value used as input generates 
* the same value as output when traversing the loop.
* Explained in detail on http://www.ellsbury.com/bombe1.htm
* 
\**************************************************************************************************/

#define MAX_CIRCLES         1024
#define MAX_CIRCLE_SIZE     26 
#define MAX_CRIB_SIZE       30
#define TURING_MAX_RESULTS  1024

typedef struct
{
    char*       cipher;             // Cipher as string
    char*       crib;               // Crib as string
    int         cribPosition;       // Position of the start of the Crib in the Cipher; -1 if not known
    int         numberOfThreads;    // Number of threads to use
    int         R1;                 // Value or Ringstellung 1, has no use  
    int         startR2;            // Min R3 to use, usually A
    int         endR2;              // Max R3 to use, usually A gives good result
    int         startR3;            // Min R2 to use, usually A
    int         endR3;              // Max R2 to use, usually Z
    Enigma_t    enigmaType;         // Type of Enigma
    WalzeSet_t  walzeSet;           // Walze set to choose Walzen from
    LinkedList* customPermutations; // Custom Walze/UKW permutations; NULL for automatic generation of permutations
} TuringRecipe;

typedef struct
{
    EnigmaSettings  settings;
    char            decoded[MAX_TEXT];
    float           score;
} TuringResult;

typedef struct
{
    char        letter;                     // Linked letter
    int         position;                   // Rotor positions of the link (advances from start of the cipher)
} LetterLink;


// Defines all the linked letters to one letter
// The maximum number of letters is MAX_CRIB_SIZE
typedef struct
{
    char        letter;                     // The letter
    int         numOfLinks;                 // Number of linked letters
    LetterLink  links[MAX_CRIB_SIZE];       // Linked letters
} LinkedLetters;


// Crib circle definition.
typedef struct CribCircle
{
    int                 circleSize;                         // Number of steps in the circle
    int                 advances    [MAX_CIRCLE_SIZE];      // Advances of the Walzen per step
    char                orgChars    [MAX_CIRCLE_SIZE+4];    // Original chars as in the text/crib
//    char                foundChars  [MAX_CIRCLE_SIZE+4];    // Steckered chars 
} CribCircle;


// Set of crib circles or loops that all start and end with the same letter
typedef struct
{
    int             numOfCircles;
    CribCircle      cribCircles[MAX_CIRCLES];
    char            startChar;
//    char            foundChar;
} CribCircleSet;

typedef struct
{
    char            startChar;
    char            foundChar;
} SteckeredChars;

extern LinkedLetters       menu[MAX_POSITIONS];
extern CribCircleSet       cribCircleSet[MAX_POSITIONS];

void            dumpSets                        ();
void            dumpMenu                        ();
SteckeredChars* createSteckeredChars            ();
int             turingValidateHypotheses        (Enigma* enigma, int g1, int g2, int g3, SteckeredChars* chars);
int             turingValidateTheSteckeredValues(SteckeredChars* chars);

// Public functions
void            turingGenerateLetterLinks       (char* cipher, char* crib, int cribStartPosition);
void            turingFindLoops                 (char* cipher, char* crib, int cribStartPosition);
void            turingBombe                     (TuringRecipe recipe, LinkedList* results);
TuringRecipe*   createDefaultTuringRecipe       (char* cipher, char* crib, int cribPosition, int numberOfThreads);
void            destroyTuringRecipe             (TuringRecipe* recipe);
LinkedList*     turingCribFit                   (char crib[], char cipher[]);
