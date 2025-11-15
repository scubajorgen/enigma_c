
#include "enigma.h"

#define MAX_NGRAM           3       // Support for mono, bi and trigrams
#define MAX_NGRAM_STRING    4       // Include trailing \0
#define MAX_NGRAM_SIZE      17576   // Max for trigrams = 26x26x26

typedef struct
{
    char    ngram[MAX_NGRAM+1];     // the ngram
    long    frequency;              // the frequency of the ngram in some reference text
    float   chance;                 // the chance of occurence of the ngram
    float   logChance;              // natural log of the chance
} NgramFrequency;

extern int            ngrams;
extern NgramFrequency ngramFrequencies[MAX_NGRAM_SIZE];

void            prepareNgramScore       (int n, char* language);
float           ngramScore              (Enigma* engima);
