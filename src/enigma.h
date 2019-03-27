#ifndef ENIGMA

#define ENIGMA

#define MAX_POSITIONS           26
#define MAX_ROTORS              4
#define MAX_ROTOR_NAME          5
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
    char    string[MAX_TEXT];
    
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
int         countLetter             (Enigma* enigma, char letter);



#endif