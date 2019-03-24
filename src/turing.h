
#define MAX_CIRCLES         50
#define MAX_CIRCLE_SIZE     26
#define MAX_CRIB_SIZE       50

typedef struct
{
    char        letter;                     // Linked letter
    int         position;                   // Rotor positions of the link (advances from start)
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
    int                 circleSize;                     // Number of steps in the circle
    int                 advances    [MAX_POSITIONS];    // Advances of the rotors per step
    char                orgChars    [MAX_POSITIONS];    // Original chars as in the text/crib
    char                foundChars  [MAX_POSITIONS];    // Steckered chars 
} CribCircle;


// Set of crib circles or loops that all start and end with the same letter
typedef struct
{
    int             numOfCircles;
    CribCircle      cribCircles[MAX_CIRCLES];
    char            startChar;
    char            foundChar;
} CribCircleSet;

extern LinkedLetters       links[MAX_POSITIONS];
extern CribCircleSet       cribCircleSet[MAX_POSITIONS];


void    turingGenerateLetterLinks   (char* text, char* crib);
void    turingFindLoops             (char* text, char* crib);
void    turingProve                 ();
void    turingBombe                 (char* cypher, char* crib, int numOfThreads);
void    turingExample               ();