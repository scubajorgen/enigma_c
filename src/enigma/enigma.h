/**************************************************************************************************\
* 
* This module implements the Enigma machine. It is optimized for performance required for cracking
* enigma ciphers. All settings (A-Z, a-z) are translated to positions from 0-25 in tables, 
* encoding/decoding consists merely of lookup in rotor tables
*
* Position : UKW   2       1     0
* Enigma M3: UKW rotor1 rotor2 rotor3
*
* Position : UKW   3      2       1     0
* Enigma M4: UKW rotor1 rotor2 rotor3 rotor4
*
\**************************************************************************************************/
#ifndef ENIGMA

#define ENIGMA

#define MAX_POSITIONS           26
#define MAX_ROTORS              4
#define MAX_ROTOR_NAME          10
#define MAX_STECKER_STRING      41
#define MAX_REFLECTIONS         13
#define MAX_TEXT                4096
#define MAX_NOTCHES             2
#define MAX_ROTOR_SETS          4
#define ROTORS                  10
#define MAX_ROTORNAME           10
#define UMKEHR_WALZEN           5


typedef enum
{
    ENIGMATYPE_M3,
    ENIGMATYPE_M4
} Enigma_t;

typedef enum RotorSet_t
{
    M3_ENIGMA1_1930     =0, // 3 rotors + 2 UKWs
    M3_ARMY_1938        =1, // 5 rotors + 2 UKWs
    M3_ARMY_1939        =2, // 8 rotors + 2 UKWs
    M4_NAVAL_1941       =3  // 8 rotors + 2 4th rotors + 2 thin UKWs
} RotorSet_t;

extern int  rotorSets[MAX_ROTOR_SETS][MAX_ROTORNAME];
extern int  fourthRotorSets[MAX_ROTOR_SETS][ROTORS];
extern int  forthRotorSets[MAX_ROTOR_SETS][MAX_ROTORNAME];
extern int  ukwSets[MAX_ROTOR_SETS][UMKEHR_WALZEN];
extern char umkehrWalzeNames[UMKEHR_WALZEN][MAX_ROTOR_NAME];
extern char rotorNames[ROTORS][MAX_ROTORNAME];

typedef struct
{
    int     numberOfRotors;
    char    rotors[MAX_ROTORS][MAX_ROTOR_NAME];
    char    ukw[MAX_ROTOR_NAME];
    int     ringStellungen[MAX_ROTORS];
    int     grundStellungen[MAX_ROTORS];
    char    steckers[MAX_STECKER_STRING];
    char    cipher[MAX_TEXT];
} EnigmaSettings;

typedef struct 
{
    int     numberOfRotors;
    int     rotorFunction[MAX_ROTORS][MAX_POSITIONS];
    int     rotorInverseFunction[MAX_ROTORS][MAX_POSITIONS];
    int     grundStellung[MAX_ROTORS];
    int     ringStellung[MAX_ROTORS];
    int     notches[MAX_ROTORS][MAX_NOTCHES];
    int     numberOfNotches[MAX_ROTORS]; 
    int     umkehrWalzeFunction[MAX_POSITIONS];
    int     steckerBrett[MAX_POSITIONS];

    int     textSize;
    int     text[MAX_TEXT];         // text as numbers 0-25
    int     conversion[MAX_TEXT];   // encoded/decoded text as numbers 0-25
    char    string[MAX_TEXT+1];     // encoded/decoded text as String of chars A-Z, add 1 for trailing '\0'
    
} Enigma;


// Functions that come in handy for cracking codes
void                advances                (Enigma* enigma, int steps);
int                 encodeCharacter         (Enigma* enigma, int theCharacter);

// Main functions for encoding/decoding
Enigma*             createEnigmaM3          ();
Enigma*             createEnigmaM4          ();
void                destroyEnigma           (Enigma* enigma);
void                setText                 (Enigma* engima, char text[]);
void                placeWalze              (Enigma* enigma, int walze, char rotorName[]);
void                setRingStellung         (Enigma* enigma, int walze, int ringStellung);
int                 getRingStellung         (Enigma* enigma, int walze);
void                setRingStellungen       (Enigma* engima, char* ringStellungen);
void                setGrundStellung        (Enigma* enigma, int walze, int grundStellung);
int                 getGrundStellung        (Enigma* enigma, int walze);
void                setGrundStellungen      (Enigma* enigma, char* grundStellungen);
void                placeUmkehrWalze        (Enigma* engima, char name[]);
void                clearSteckerBrett       (Enigma* engima);
void                placeSteckers           (Enigma* engima, char steckers[]);
void                encodeDecode            (Enigma* enigma);
char*               toString                (Enigma* enigma);
void                setEnigma               (Enigma* enigma, EnigmaSettings* settings);
void                printEnigmaSettings     (EnigmaSettings* settings, char* title);
void                dumpDecoded             (EnigmaSettings* settings);

EnigmaSettings*     createRandomSettings    (Enigma* enigma, RotorSet_t rotorSet, int numberOfSteckers);
void                destroyEnigmaSettings   (EnigmaSettings* settings);

int                 countLetter             (Enigma* enigma, char letter);
int                 countConvertedChar      (Enigma* enigma, int  letter);
int                 countTrigram            (Enigma* enigma, char* trigram);
int                 countNgram              (Enigma* enigma, char* ngram, int n);

#endif