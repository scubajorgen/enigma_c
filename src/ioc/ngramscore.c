#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "ngramscore.h"
#include "enigma.h"

#define MAX_NGRAM           3
#define MAX_NGRAM_SIZE      17576

typedef enum {NGRAM_CHANCE, NGRAM_FREQUENCY} NgramFileFormat;

typedef struct
{
    char    ngram[MAX_NGRAM];   // the ngram
    int     value;              // the ngram represented as value: each letter takes 5 bits
    long    frequency;          // the frequency of the ngram
    float   chance;             // the chance of the ngram
    float   logChance;          // natural log of the chance
} NgramFrequency;

int             ngramSize=0;
NgramFrequency  ngramFrequencies[MAX_NGRAM_SIZE];
int             ngrams;
long            minFrequency;
float           minChance;
float           minLogChance;

char fileLine[128];


/**************************************************************************************************\
* 
* Reads the ngram file. Supports two types: frequency and chance
*
* Frequency:
* [ABC] [123]
* [ABC] is the ngram, [123] the number of occurences (e.g. in some text)
*
* Chance
* [ABC] [0.123]
* [ABC] is the ngram, [0.123] the relative chance of occurences
*
* In both cases the list must be alphabetically sorted in ascending order
* 
\**************************************************************************************************/
void readNgramFile(char* fileName, int n, NgramFileFormat format)
{
    FILE *fp;
    int  i;
    int  line;
    long sum;
 
    fp = fopen(fileName, "r"); // read mode
 
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    } 
 
    line=0;
    sum=0;
    minFrequency=INT_MAX;
    minChance=1.0f;
    // Read line
    while ( fgets(fileLine, sizeof fileLine, fp)!=NULL)
    {
        ngramFrequencies[line].value=0;
        i=0;
        while ((i<n) && (fileLine[i]!=' '))
        {
            ngramFrequencies[line].ngram[i]     =fileLine[i];
            ngramFrequencies[line].value        <<=5;
            ngramFrequencies[line].value        +=ngramFrequencies[line].ngram[i]-'A';
            i++;
        }
        if (format==NGRAM_CHANCE)
        {
            ngramFrequencies[line].chance       =atof(fileLine+i+1);
            ngramFrequencies[line].logChance    =log(ngramFrequencies[line].chance);
            if (ngramFrequencies[line].chance<minChance)
            {
                minChance                       =ngramFrequencies[line].chance;
            }
        }
        else
        {
            ngramFrequencies[line].frequency    =strtol(fileLine+i+1, NULL, 10);
            sum+=ngramFrequencies[line].frequency;
            if (ngramFrequencies[line].frequency<minFrequency)
            {
               minFrequency                     =ngramFrequencies[line].frequency;
            }
        }        
        
        line++;
    }
    
    ngrams=line;
    
    // If frequencies were read, calculate the change of occurrence
    if (format==NGRAM_FREQUENCY)
    {
        i=0;
        while (i<ngrams)
        {
            ngramFrequencies[i].chance      =(float)ngramFrequencies[i].frequency/(float)sum;
            ngramFrequencies[i].logChance   =log(ngramFrequencies[i].chance);
/*
            printf("%s (%d) - %ld - %f - %f\n", 
                                ngramFrequencies[i].ngram, 
                                ngramFrequencies[i].value,
                                ngramFrequencies[i].frequency,
                                ngramFrequencies[i].chance,
                                ngramFrequencies[i].logChance);
*/ 
            i++;
        }
        minChance=(float)minFrequency/(float)sum;
    }
    minLogChance=log(minChance);

    fclose(fp);    
}


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void prepareNgramScore(int n, char* language)
{
    ngramSize=n;
    if (strncmp(language, "DE", 4)==0)
    {
        if (n==1)
        {
            readNgramFile("ngrams/german_monograms.txt" , 1, NGRAM_FREQUENCY);
        }
        else if (n==2)
        {
            readNgramFile("ngrams/german_bigrams.txt"   , 2, NGRAM_FREQUENCY);
        }
        else if (n==3)
        {
            readNgramFile("ngrams/german_trigrams.txt"  , 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
        }
    }
    else if (strncmp(language, "GB", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/english_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
        }
    }
    else if (strncmp(language, "GC", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/geocaching_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
        }
    }
    else if (strncmp(language, "GC2", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/geocachingDe_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
        }
    }
    else
    {
        ngramSize=0;
    }
}




/**************************************************************************************************\
* 
* Calculate the ngram score. It assumes the array of ngrams is sorted alphabetically
* 
\**************************************************************************************************/
float ngramScore(Enigma* engima)
{
    int     c;
    int     ngram;
    int     ngramInc;
    int     found;
    int     exit;
    int     ngramValue;
    float   score;
    int     last;
    
    score   =0;
    c       =0;
    while (c<engima->textSize-ngramSize+1)
    {
        // Construct the ngram value
        ngram           =0;
        ngramValue      =0;
        while (ngram<ngramSize)
        {
            ngramValue<<=5;
            ngramValue+=engima->conversion[c+ngram];
            ngram++;
        }
       
        ngram       =ngrams>>1;
        ngramInc    =ngram>>1;
        found       =0;
        exit        =0;
        last        =4;         // TODO there must be a better solution for this...
        while (!found && !exit)
        {
            if (ngramValue==ngramFrequencies[ngram].value)
            {
                score+=ngramFrequencies[ngram].logChance;
                found=1;
            }
            else if (ngramValue>ngramFrequencies[ngram].value)
            {
                ngram+=ngramInc;
                if (ngramInc>1)
                {
                    ngramInc>>=1;
                }
                else if (ngramInc==1)
                {
                    if (last==0)
                    {
                        exit=1;
                    }
                    last--;
                }
            }
            else
            {
                ngram-=ngramInc;
                if (ngramInc>1)
                {
                    ngramInc>>=1;
                }
                else if (ngramInc==1)
                {
                    if (last==0)
                    {
                        exit=1;
                    }
                    last--;
                }
            }
        }
        // For non found ngrams, add the score of the last 
        // ngram
        if (!found)
        {
            score+=minLogChance;
        }
        c++;
    }
    return score;
}
