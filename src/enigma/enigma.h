/**************************************************************************************************\
* 
* This module implements the Enigma machine. It is optimized for performance required for cracking
* enigma ciphers. All settings (A-Z, a-z) are translated to positions from 0-25 in tables, 
* encoding/decoding consists merely of lookup in Walze tables
*
* Position : UKW   2       1     0
* Enigma M3: UKW Walze1 Walze2 Walze3
*
* Position : UKW   3      2       1     0
* Enigma M4: UKW Walze1 Walze2 Walze3 Walze4
*
\**************************************************************************************************/
#ifndef ENIGMA

#define ENIGMA

#include "linkedList.h"


#define MAX_POSITIONS           26
#define MAX_WALZEN              4
#define MAX_WALZE_NAME          10
#define MAX_STECKER_STRING      41
#define MAX_REFLECTIONS         13
#define MAX_TEXT                4096
#define MAX_NOTCHES             2
#define MAX_WALZE_SETS          4
#define WALZEN                  10
#define UMKEHR_WALZEN           5


typedef enum
{
    ENIGMATYPE_M3,
    ENIGMATYPE_M4
} Enigma_t;

typedef enum WalzeSet_t
{
    M3_ENIGMA1_1930     =0, // 3 Walzen + 2 UKWs
    M3_ARMY_1938        =1, // 5 Walzen + 2 UKWs
    M3_ARMY_1939        =2, // 8 Walzen + 2 UKWs
    M4_NAVAL_1941       =3  // 8 Walzen + 2 4th Walzen + 2 thin UKWs
} WalzeSet_t;

typedef enum
{
    MESSAGEFORMAT_TEXT,
    MESSAGEFORMAT_KRIEGSMARINE,
    MESSAGEFORMAT_WEHRMACHT
} MessageFormat_t;

extern char walzeNames      [WALZEN]        [MAX_WALZE_NAME];  // All Walzen
extern char umkehrWalzeNames[UMKEHR_WALZEN] [MAX_WALZE_NAME];   // All UKWs
extern int  walzeSets       [MAX_WALZE_SETS][WALZEN];           // Walze selections for the 3 Walzen
extern int  fourthWalzeSets [MAX_WALZE_SETS][WALZEN];           // Walze selections for the 4th Walze
extern int  ukwSets         [MAX_WALZE_SETS][UMKEHR_WALZEN];    // UKW selections

typedef struct
{
    int     numberOfWalzen;
    char    walzen[MAX_WALZEN][MAX_WALZE_NAME];
    char    ukw[MAX_WALZE_NAME];
    int     ringStellungen[MAX_WALZEN];
    int     grundStellungen[MAX_WALZEN];
    char    steckers[MAX_STECKER_STRING];
    char    cipher[MAX_TEXT];
} EnigmaSettings;

typedef struct 
{
    int     numberOfWalzen;
    int     walzeFunction[MAX_WALZEN][MAX_POSITIONS];
    int     walzeInverseFunction[MAX_WALZEN][MAX_POSITIONS];
    int     grundStellung[MAX_WALZEN];
    int     ringStellung[MAX_WALZEN];
    int     notches[MAX_WALZEN][MAX_NOTCHES];
    int     numberOfNotches[MAX_WALZEN]; 
    int     umkehrWalzeFunction[MAX_POSITIONS];
    int     steckerBrett[MAX_POSITIONS];

    int     textSize;
    int     text[MAX_TEXT];         // text as numbers 0-25
    int     conversion[MAX_TEXT];   // encoded/decoded text as numbers 0-25
    char    string[MAX_TEXT+1];     // encoded/decoded text as String of chars A-Z, add 1 for trailing '\0'
    
} Enigma;


// Functions that come in handy for cracking codes
void                advances                    (Enigma* enigma, int steps);
int                 encodeCharacter             (Enigma* enigma, int theCharacter);

// Main functions for encoding/decoding
Enigma*             createEnigmaM3              ();
Enigma*             createEnigmaM4              ();
void                destroyEnigma               (Enigma* enigma);
void                setText                     (Enigma* engima, char text[]);
void                placeWalze                  (Enigma* enigma, int walze, char walzeName[]);
void                setRingStellung             (Enigma* enigma, int walze, int ringStellung);
int                 getRingStellung             (Enigma* enigma, int walze);
void                setRingStellungen           (Enigma* engima, char* ringStellungen);
void                setGrundStellung            (Enigma* enigma, int walze, int grundStellung);
int                 getGrundStellung            (Enigma* enigma, int walze);
void                setGrundStellungen          (Enigma* enigma, char* grundStellungen);
LinkedList*         getWalzenPermutations       (Enigma_t enigmaType, WalzeSet_t walzeSet);
void                placeUmkehrWalze            (Enigma* engima, char name[]);
LinkedList*         getUkwPermutations          (Enigma_t enigmaType, WalzeSet_t walzeSet);
void                clearSteckerBrett           (Enigma* engima);
void                placeSteckers               (Enigma* engima, char steckers[]);
int                 steckersToSteckerbrettTable (char steckers[], int* steckerBrettTable);
void                steckerbrettTableToSteckers (int *steckerBrettTable, char steckers[]);
void                encodeDecode                (Enigma* enigma);
char*               toString                    (Enigma* enigma);
void                setEnigma                   (Enigma* enigma, EnigmaSettings* settings);
void                printEnigmaSettings         (EnigmaSettings* settings, char* title);
void                dumpDecoded                 (EnigmaSettings* settings);
void                displayEnigmaMessage        (char* message, MessageFormat_t type);

EnigmaSettings*     createRandomSettings        (Enigma* enigma, WalzeSet_t walzeSet, int numberOfSteckers);
void                destroyEnigmaSettings       (EnigmaSettings* settings);

int                 countLetter                 (Enigma* enigma, char letter);
void                countConvertedChars         (Enigma* enigma, int counts[MAX_POSITIONS]);
int                 countTrigram                (Enigma* enigma, char* trigram);
int                 countNgram                  (Enigma* enigma, char* ngram, int n);

#endif