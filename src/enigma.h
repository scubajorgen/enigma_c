/**************************************************************************************************\
* 
* This module implents the Enigma machine. It is optimized for performance required for cracking
* enigma cyphers. All settings (A-Z, a-z) are translated to positions from 0-25 in tables, 
* encoding/decoding consists merely of lookup in tables
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
#define MAX_ROTOR_NAME          6
#define MAX_STECKER_STRING      41
#define MAX_REFLECTIONS         13
#define MAX_TEXT                1024
#define MAX_NOTCHES             2

typedef struct
{
    int     numberOfRotors;
    char    rotors[MAX_ROTORS][MAX_ROTOR_NAME];
    char    ukw[MAX_ROTOR_NAME];
    int     ringStellungen[MAX_ROTORS];
    int     grundStellungen[MAX_ROTORS];
    char    steckers[MAX_STECKER_STRING];
    char    cypher[MAX_TEXT];
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
    int     umkehrWaltzeFunction[MAX_POSITIONS];
    int     steckerBrett[MAX_POSITIONS];

    int     textSize;
    int     text[MAX_TEXT];
    int     conversion[MAX_TEXT];
    char    string[MAX_TEXT+1]; // add 1 for '\0'
    
} Enigma;


// Functions that come in handy for cracking codes
void        advances                (Enigma* enigma, int steps);
int         encodeCharacter         (Enigma* enigma, int theCharacter);

// Main functions for encoding/decoding
Enigma*     createEnigmaM3          ();
Enigma*     createEnigmaM4          ();
void        destroyEnigma           (Enigma* enigma);
void        setText                 (Enigma* engima, char text[]);
void        placeWaltze             (Enigma* enigma, int waltze, char rotorName[]);
void        setRingStellung         (Enigma* enigma, int waltze, int ringStellung);
void        setRingStellungen       (Enigma* engima, char* ringStellungen);
void        setGrundStellung        (Enigma* enigma, int waltze, int grundStellung);
void        setGrundStellungen      (Enigma* enigma, char* grundStellungen);
void        placeUmkehrWaltze       (Enigma* engima, char name[]);
void        clearSteckerBrett       (Enigma* engima);
void        placeSteckers           (Enigma* engima, char steckers[]);
void        encodeDecode            (Enigma* enigma);
char*       toString                (Enigma* enigma);
void        setEnigma               (Enigma* enigma, EnigmaSettings* settings);
void        dumpDecoded             (EnigmaSettings* settings);

int         countLetter             (Enigma* enigma, char letter);
int         countConvertedChar      (Enigma* enigma, int letter);
int         countTrigram            (Enigma* enigma, char* trigram);
int         countNgram              (Enigma* enigma, char* ngram, int n);

#endif