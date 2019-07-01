#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "ngramscore.h"
#include "enigma.h"

#define MAX_NGRAM           3
#define MAX_NGRAM_SIZE      17576

typedef enum {NGRAM_CHANGE, NGRAM_FREQUENCY} NgramFileFormat;

typedef struct
{
    char    ngram[MAX_NGRAM];
    int     value;
    int     frequency;
    float   chance;
    float   logChance;
} NgramFrequency;

NgramFrequency  ngramFrequencies[MAX_NGRAM_SIZE];
int             ngrams;


char fileLine[128];




/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void readNgramFile(char* fileName, int n, NgramFileFormat format)
{
    FILE *fp;
    int  i;
    int  line;
    int sum;
 
    fp = fopen(fileName, "r"); // read mode
 
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    } 
 
    line=0;
    sum=0;
    // Read line
    while ( fgets(fileLine, sizeof fileLine, fp)!=NULL)
    {
        ngramFrequencies[line].value=0;
        i=0;
        while ((i<n) && (fileLine[i]!=' '))
        {
            ngramFrequencies[line].ngram[i]=fileLine[i];
            ngramFrequencies[line].value<<=5;
            ngramFrequencies[line].value+=ngramFrequencies[line].ngram[i]-'A';
            i++;
        }
        if (format==NGRAM_CHANGE)
        {
            ngramFrequencies[line].chance=atof(fileLine+i+1);
            ngramFrequencies[line].logChance=log(ngramFrequencies[line].chance);
        }
        else
        {
            ngramFrequencies[line].frequency=atoi(fileLine+i+1);
            sum+=ngramFrequencies[line].frequency;
        }        
        
        line++;
    }
    
    ngrams=line;
    
    if (format==NGRAM_FREQUENCY)
    {
        i=0;
        while (i<ngrams)
        {
            ngramFrequencies[i].chance      =(float)ngramFrequencies[i].frequency/(float)sum;
            ngramFrequencies[i].logChance   =log(ngramFrequencies[i].chance);
/*
            printf("%s (%d) - %d - %f - %f\n", 
                                ngramFrequencies[i].ngram, 
                                ngramFrequencies[i].value,
                                ngramFrequencies[i].frequency,
                                ngramFrequencies[i].chance,
                                ngramFrequencies[i].logChance);
*/
            i++;
        }
    } 
    fclose(fp);    
}


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void prepareNgramScore(int n, char* language)
{
    if (strncmp(language, "DE", 4)==0)
    {
        if (n==1)
        {
            readNgramFile("ngrams/german_monograms.txt", 3, NGRAM_FREQUENCY);
        }
        else if (n==3)
        {
            readNgramFile("ngrams/german_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
    }
    else if (strncmp(language, "GB", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/english_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
    }
    else if (strncmp(language, "GC", 4)==0)
    {
        if (n==3)
        {
            readNgramFile("ngrams/geocaching_trigrams.txt", 3, NGRAM_FREQUENCY);
        }
    }
}




/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
float ngramScore(Enigma* engima, int n)
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
    while (c<engima->textSize-n+1)
    {
        // Construct the ngram value
        ngram           =0;
        ngramValue      =0;
        while (ngram<n)
        {
            ngramValue<<=5;
            ngramValue+=engima->conversion[c+ngram];
            ngram++;
        }
        
        ngram       =ngrams>>1;
        ngramInc    =ngram>>1;
        found       =0;
        exit        =0;
        last        =2;
        while (!found && !exit)
        {
//            printf("%d %d - %d %d +- %d\n", ngramValue, ngramFrequencies[ngram].value, ngrams, ngram, ngramInc);
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
            score+=ngramFrequencies[ngrams-1].logChance;
        }
        c++;
    }
    return score;
}
