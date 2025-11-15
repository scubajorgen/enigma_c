/**************************************************************************************************\
* 
* ngramScore.c
* Functions for calculating NGRAM scores
* 
\**************************************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "ngramScore.h"
#include "log.h"
#include "enigma.h"
#include "toolbox.h"

typedef enum {NGRAM_CHANCE, NGRAM_FREQUENCY} NgramFileFormat;

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
* Chance:
* [ABC] [0.123]
* [ABC] is the ngram, [0.123] the relative chance of occurences
*
* In both cases the list must be alphabetically sorted in ascending order
* FILE MUST CONTAIN ALL POSSIBLE BI OR TRIGRAMS, SORTED AND IN UPPER CASE!
*
* In the array ngramFrequences each NGRAM is placed on the right position, whether it is in the
* file or not, resulting in e.g.
* 0 "AAA" ...
* 1 "AAB" ... 
* 2 ""    ...
* 3 "AAD" ...
* ...
* So the NGRAM is in fact the index ABC means A*26^2+B*26^1+C*26^0
\**************************************************************************************************/
void readNgramFile(char* fileName, int n, NgramFileFormat format)
{
    logDebug("Preparing NGRAMS: n=%d, file: %s, start", n, fileName);

    // Clear the NGRAM frequencies by setting ngram to ""
    int ngramArraySize=ipow(26,n);
    for (int i=0; i<ngramArraySize; i++)
    {
        strncpy(ngramFrequencies[i].ngram, "***", MAX_NGRAM_STRING);
        ngramFrequencies[i].frequency=0;
    }

    FILE* fp = fopen(fileName, "r"); // read mode
 
    if (fp == NULL)
    {
        logFatal("Error while opening the file %s", fileName);
        exit(EXIT_FAILURE);
    } 
 
    int  line   =0;
    long sum    =0;
    minFrequency=INT_MAX;
    minChance   =0.0f;
    // Read line
    while ( fgets(fileLine, sizeof fileLine, fp)!=NULL)
    {
        int     index=0;
        int     i=0;
        bool    ngramFound=true;
        char    ngram[MAX_NGRAM+1];
        while (i<n && ngramFound)
        {
            char c=fileLine[i];
            if ((c=='\0') || (c=='\n'))
            {
                // line does not contain NGRAM
                ngramFound=false;
            }
            else if (c!=' ')
            {
                ngram[i]=c;
                index*=26;
                index+=c-'A';
                i++;
            }
        }
        ngram[i]='\0'; 

        if (ngramFound)
        {
            strncpy(ngramFrequencies[index].ngram, ngram, MAX_NGRAM_STRING);
            if (format==NGRAM_CHANCE)
            {
                ngramFrequencies[index].chance      =atof(fileLine+i+1);
                ngramFrequencies[index].logChance   =log(ngramFrequencies[index].chance);
                if (ngramFrequencies[index].chance<minChance)
                {
                    minChance                       =ngramFrequencies[index].chance;
                }
            }
            else
            {
                ngramFrequencies[index].frequency   =strtol(fileLine+i+1, NULL, 10);
                sum+=ngramFrequencies[index].frequency;
                if (ngramFrequencies[index].frequency<minFrequency)
                {
                    minFrequency                    =ngramFrequencies[index].frequency;
                }
            }
            line++;
        }
        else
        {
            logWarning("Empty line at line %d in NGRAM file %s", line, fileName);
        }
    }
    ngrams=line;

    logDebug("Preparing NGRAMs: n=%d, file: %s: %d NGRAMS read", n, fileName, line);
    logDebug("NGRAM occurence sum: %ld", sum);
    // If frequencies were read, calculate the change of occurrence
    if (format==NGRAM_FREQUENCY)
    {
        for (int i=0; i<ngramArraySize; i++)
        {
            if (ngramFrequencies[i].ngram[0]!='*')
            {
                ngramFrequencies[i].chance      =(float)ngramFrequencies[i].frequency/(float)sum;
                ngramFrequencies[i].logChance   =log(ngramFrequencies[i].chance);
            }
        }
        minChance=(float)minFrequency/(float)sum;
    }
    minLogChance=log(minChance);
    logDebug("NGRAM minimum freq %ld, min chance %f min Logchange %f\n", minFrequency, minChance, minLogChance);
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
            logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
            ngramSize=0;
        }
    }
    else if (strncmp(language, "GB", 4)==0)
    {
        if (n==2)
        {
            readNgramFile("ngrams/GB_bigrams.txt", 2, NGRAM_FREQUENCY);
        }
        if (n==3)
        {
            readNgramFile("ngrams/GB_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
            logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
        }
    }
    else if (strncmp(language, "NL", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/dutch_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
        else
        {
            ngramSize=0;
            logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
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
            logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
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
            logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
        }
    }
    else
    {
        ngramSize=0;
        logFatal("Invalid NGRAM configuration/file: n=%d, language: %s", n, language);
    }
}

/**************************************************************************************************\
* 
* Calculate the ngram score. It assumes the array of ngrams is sorted alphabetically
* About 51% faster than previous ngramScore()
* 
\**************************************************************************************************/
float ngramScore(Enigma* enigma)
{
    float score=0;
    for (int i=0; i<enigma->textSize-ngramSize+1; i++)
    {
        int index=0;
        for (int j=0; j<ngramSize; j++)
        {
            index+=enigma->conversion[i+j];
            if (j<ngramSize-1)
            {
                index*=26;
            }
        }
        if (ngramFrequencies[index].ngram[0]=='*')
        {
            // If an NGRAM is not in the list, assume lowest probability
            score+=minLogChance;
        }
        else
        {
            // consistency check
            /*
            bool error=false;
            for (int j=0; j<ngramSize;j++)
            {
                if (ngramFrequencies[index].ngram[j]-'A'!=enigma->conversion[i+j])
                {
                    logFatal("Inconsistent NGRAM list");
                }
            }
            */
            score+=ngramFrequencies[index].logChance;
        }
    }
    return score;
}