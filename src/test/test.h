#ifndef TEST_H
#define TEST_H

#include "linkedList.h"

extern char testTextGerman[];
extern char testTextEnglish[];
extern char testTextMax[];
extern char testTextRandom[];
extern char testTextEnigma[];

// Helpers
char*   createSampleText       (char* dest, char* source, int size);

void    testToolbox            ();
void    testLinkedList         ();
void    testWorkDispatcher     ();
void    testWalze              ();
void    testUkw                ();
void    testSteckerbrett       ();
void    testEnigma             ();
void    testCoincidence        ();
void    testNgram              ();
void    testTuring             ();
void    testTuringTryOut       ();

void    testEnigmaPerformance  ();
void    testIocPerformance     ();
void    testTuringPerformance  ();

#endif