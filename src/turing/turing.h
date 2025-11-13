
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
void            turingBombe                     (char* cipher, char* crib, int cribStartPosition, int numOfThreads);
