/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software tries all rotor settings, 
* RingStellungen included.
*
\**************************************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "enigma.h"
#include "coincidence.h"
#include "toolbox.h"
#include "ngrams.h"
#include "ngramscore.h"

/**************************************************************************************************\
* DEFINES
\**************************************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_TRIGRAMS        54
#define MAX_NGRAMSETSIZE    5

/** Defines the method for decrypting the engima cypher */
typedef struct
{
    Method_t    method;                     // The method
    int         maxSteckers;                // Maximum number of Steckers to use
    int         maxSteckersIoc;             // METHOD_NGRAMS: the number of Steckers used for IOC; beyond this number NGRAMS
    int         ngramSize;                  // n in ngram (2-bigrams, 3-trigrams)
    char        ngramSet[MAX_NGRAMSETSIZE]; // The set to use ('DE' - german, 'EN' - english, 'GC' - NL, geocaching)
} EvaluationMethod;

typedef struct
{
    pthread_t           threadId;
    LinkedList*         permutations;
    EvaluationMethod*   evaluationMethod;
} IocThreadParams;

/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

EnigmaSettings      iocSettings;

int                 iocNumberOfResults=0;      
IocResults          iocTopResults[TOP_RESULTS_SIZE];
IocResults          swap;

EvaluationMethod    evaluationMethod;
IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
int                 iocNumberOfWorkItems;
int                 iocInitialNumberOfWorkItems;

pthread_mutex_t     iocMutex;

IocThreadParams     threadParams[MAX_THREADS];

int                 iocThreadsRunning=0;
int                 iocLastThreadId=-1;

int                 ioctrigramCount[MAX_TRIGRAMS];

long                evaluationStartTime;
long                evaluationEndTime;


/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/
/**************************************************************************************************\
* 
* Calculates the index of coincidence (Gillogly)
* TODO: transform to int calculation for speed
* 
\**************************************************************************************************/
float iocIndexOfCoincidence(Enigma* enigma)
{
    int     c;
    float   ioc;
    int     iocCharCount[MAX_POSITIONS];
    
    c=0;
    while (c<MAX_POSITIONS)
    {
        iocCharCount[c]=countConvertedChar(enigma, c);
        c++;
    }
    
    ioc=0.0;
    c=0;
    while (c<MAX_POSITIONS)
    {
        ioc+=iocCharCount[c]*(iocCharCount[c]-1);
        c++;
    }
    ioc/=enigma->textSize*(enigma->textSize-1);
    
    return ioc;
}

/**************************************************************************************************\
* 
* Store the results in the top 10 of best results
* Returns the lowest index of coincidence value in the list
* 
\**************************************************************************************************/
float iocStoreResults(IocResults* results)
{
    int i;
    int index;
    
    // Find the position to fit the results in, if any
    // Check if the ioc surpasses any ioc in the list
    i       =0;
    index   =-1;
    while (i<iocNumberOfResults && index<0)
    {
        if (results->indexOfCoincidence > iocTopResults[i].indexOfCoincidence)
        {
            index=i;
        }
        i++;
    }
    // If not and the list is not full yet add it
    if ((index<0) && (i<TOP_RESULTS_SIZE))
    {
        index=i;
    }
    
    if (index>=0)
    {
        // Shift all top 10 entries below
        i=iocNumberOfResults;
        while (i>index)
        {
            if (i<TOP_RESULTS_SIZE)
            {
                iocTopResults[i]=iocTopResults[i-1];
            }
            i--;
        } 
        // Add the new record
        iocTopResults[index]=*results;

        if (iocNumberOfResults<TOP_RESULTS_SIZE)
        {
            iocNumberOfResults++;
        }   
    }
    return iocTopResults[iocNumberOfResults-1].indexOfCoincidence;
}

/**************************************************************************************************\
* 
* After processing the top 10 results may no longer be sorted. This sorts the list.
* 
\**************************************************************************************************/
void sortTopResults()
{
    int     i;
    int     j;

    i=0;
    while (i<TOP_RESULTS_SIZE-1)
    {
        j=i+1;
        while (j<TOP_RESULTS_SIZE)
        {
            if (iocTopResults[j].indexOfCoincidence>iocTopResults[i].indexOfCoincidence)
            {
                swap            =iocTopResults[i];
                iocTopResults[i]=iocTopResults[j];
                iocTopResults[j]=swap;
            }
            j++;
        }        
        i++;
    }
}

/**************************************************************************************************\
* 
* Show the top results. En passant, translate the found steckertable into stecker pairs
* 
\**************************************************************************************************/
void iocDumpTopResults(int withDecode)
{
    int             i;
    int             s1, s2;
    IocResults*     results;
    EnigmaSettings* settings;   
    Enigma*         enigma;

    printf("Top %d best results:\n", TOP_RESULTS_SIZE);
    enigma=createEnigmaM3();
    i=0;
    while (i<iocNumberOfResults)
    {
        results=&iocTopResults[i];
        settings=&results->settings;

        // Convert the stecker positions to a stecker brett string of pairs
        s1=0;
        s2=0;
        while (s1<MAX_POSITIONS)
        {
            if (results->steckerTable[s1]>s1)
            {
                if (s2>0)
                {
                    results->settings.steckers[3*s2-1]=' ';
                }
                results->settings.steckers[3*s2]=s1+'A';
                results->settings.steckers[3*s2+1]=results->steckerTable[s1]+'A';
                results->settings.steckers[3*s2+2]='\0';
                s2++;
            }
            s1++;
        }
     
        printf("%2d: %s %3s %3s %3s R %2d %2d %2d G %2d %2d %2d - %s - %f\n",
               i+1,
               settings->ukw,
               settings->rotors[0], settings->rotors[1], settings->rotors[2],
               settings->ringStellungen[0], settings->ringStellungen[1], settings->ringStellungen[2],
               settings->grundStellungen[0], settings->grundStellungen[1], settings->grundStellungen[2],
               settings->steckers,
               results->indexOfCoincidence);
        if (withDecode)
        {
            setEnigma(enigma, settings);
            encodeDecode(enigma);
            printf("%s\n", toString(enigma));
        }

        i++;
    }
    destroyEnigma(enigma);

    fflush(stdout);
}

/**************************************************************************************************\
* 
* Helper: converts the engima stecker string to the stecker table
* The stecker string is like: AG HI PQ CV
* 
\**************************************************************************************************/
int steckersToSteckerTable(char* steckers, int* steckerTable)
{
    int count;
    int s;

    // Initialise stecker brett table: no steckers
    s=0;
    while (s<MAX_POSITIONS)
    {
        steckerTable[s]=s;
        s++;
    }
  
    // Convert the steckers from the Enigma to the stecker table
    count=0;

    s=0;
    while (s<strlen(steckers))
    {
        steckerTable[steckers[s+0]-'A']=steckers[s+1]-'A';
        steckerTable[steckers[s+1]-'A']=steckers[s+0]-'A';
        count++;
        s+=3;
    }  
    return count;
}

/**************************************************************************************************\
* 
* Helper: converts the stecker table to a engima stecker string
* 
\**************************************************************************************************/
void steckerTableToSteckers(int *steckerTable, char* steckers)
{
    int s1;
    int s2;
    // Convert the stecker positions to a stecker brett string
    s1=0;
    s2=0;
    while (s1<MAX_POSITIONS)
    {
        if (steckerTable[s1]>s1)
        {
            if (s2>0)
            {
                steckers[3*s2-1]=' ';
            }
            steckers[3*s2]=s1+'A';
            steckers[3*s2+1]=steckerTable[s1]+'A';
            steckers[3*s2+2]='\0';
            s2++;
        }
        s1++;
    }
}

/**************************************************************************************************\
* 
* Helper: print the steckertable as letters
* 
\**************************************************************************************************/
void printSteckerTable(int* steckerTable)
{
    int i;
    printf("Stecker table: ");
    i=0;
    while (i<26)
    {
      printf("%c", steckerTable[i]+'A');
      i++;
    }
    printf("\n");  
}

/**************************************************************************************************\
* 
* Helper: print found letters in a nifty string
* 
\**************************************************************************************************/
void printFoundLetters(int s1Max, int s2Max, int s1aMax, int s2aMax, float maxIoc)
{
    printf("Found steckered chars %c-%c ", s1Max+'A', s2Max+'A');
    printf("(");

    if (s1aMax>=0)
    {
        printf("%c-%c", s1aMax+'A', s1Max+'A');
    }
    else
    {
        printf(" - ");
    }
    if (s2aMax>=0)
    {
        printf(" %c-%c", s2Max+'A', s2aMax+'A');
    }
    else
    {
        printf("  - ");
    }

    printf(") ");
    printf("ioc %f\n", maxIoc);
    fflush(stdout);  
}

/**************************************************************************************************\
* 
* After the rotor settings and Ringstellungs have been found, this method finds the steckers. 
* It uses the Index of Coincidence as measure of plain text fitness
* It uses a 'hill climbing algorithm': try a stecker on each pair of unsteckered letters and
* calculate de IoC. Place the one with the highest IoC. Proceed to the next round until the
* requested number of steckers has been placed or no better IoC can be found.
* This function is meant to be called after the rotor settings and Ringstellungs have been established 
* (Gillogly method)
*
* Steckering: if on a location a stecker is already present, this stecker is removed
* Example: if we want to swap the P and Q position:
*           ....PQ.... -> ....QP....
*           ..P.NQ.... -> ..N.QP....
*           ....PS.Q.. -> ....QP.S..
*           ..P.NS.Q.. -> ..N.QP.S..             
* 
\**************************************************************************************************/
void iocFindSteckeredChars(IocResults* results, int maxNumOfSteckers)
{
    Enigma* enigma;
    int     sCount;
    int     s1;
    int     s2;
    int     s1a;
    int     s2a;
    int     s1Max;
    int     s2Max;
    int     s1aMax;
    int     s2aMax;
    int     steckerTable[MAX_POSITIONS];
    float   maxIoc;
    float   ioc;
    int     found;

    // Initialise stecker brett table
    sCount=steckersToSteckerTable(results->settings.steckers, steckerTable);

    // Setup the enigma
    enigma=createEnigmaM3();
    setEnigma(enigma, &results->settings);
    
    // The IoC found by just moving the rotors, no steckers
    maxIoc          =results->indexOfCoincidence;
    
    found=1;
    while (sCount<maxNumOfSteckers && found)
    {
        s1Max=-1;
        s2Max=-1;
        s1aMax=-1;
        s2aMax=-1;
        found=0;
        s1=0;
        while (s1<MAX_POSITIONS-1)
        {
            s2=s1+1;
            while (s2<MAX_POSITIONS)
            {
                if (steckerTable[s1]==s1 && steckerTable[s2]==s2)
                {
                    // Place stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s1a=-1;
                    s2a=-1;
                }
                else if (steckerTable[s1]!=s1 && steckerTable[s2]==s2)
                {
                    // swap back: s1=s1, s1a=s1a
                    s1a=steckerTable[s1];
                    steckerTable[s1a]=s1a;
                    // place new stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s2a=-1;
                }
                else if (steckerTable[s1]==s1 && steckerTable[s2]!=s2)
                {
                    // swap back: s2=s2, s2a=s2a
                    s2a=steckerTable[s2];
                    steckerTable[s2a]=s2a;
                    // place new stecker
                    steckerTable[s1]=s2;  
                    steckerTable[s2]=s1;
                    s1a=-1;
                }
                else
                {
                    // swap back: s1=s1, s1a=s1a, s2=s2, s2a=s2a
                    s1a=steckerTable[s1];
                    s2a=steckerTable[s2];
                    steckerTable[s1a]=s1a;
                    steckerTable[s2a]=s2a;
                    // place new stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                }

                // Quick and dirty way to set up the steckers
                memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                
                // reset the enigma Grundstellung
                setGrundStellung(enigma, 1, results->settings.grundStellungen[0]);
                setGrundStellung(enigma, 2, results->settings.grundStellungen[1]);
                setGrundStellung(enigma, 3, results->settings.grundStellungen[2]);
                
                encodeDecode(enigma);
                ioc=iocIndexOfCoincidence(enigma);

                if (ioc>maxIoc)
                {
                    maxIoc=ioc;
                    s1Max=s1;
                    s2Max=s2;
                    s1aMax=s1a;
                    s1aMax=s2a;
                    found=1;
                }
                // Remove stecker
                if (s1a>=0)
                {
                    steckerTable[s1]  =s1a;
                    steckerTable[s1a] =s1;
                }
                else
                {
                    steckerTable[s1]  =s1;
                }
                if (s2a>=0)
                {
                    steckerTable[s2]  =s2a;
                    steckerTable[s2a] =s2;
                }
                else
                {
                    steckerTable[s2]  =s2;
                }
              
                s2++;
            }
            s1++;
        }
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            printFoundLetters(s1Max, s2Max, s1aMax, s2aMax, maxIoc);
            if (s1aMax>=0)
            {
              steckerTable[s1aMax]=s1aMax;
              sCount--;
            }
            if (s2aMax>=0)
            {
              steckerTable[s2aMax]=s2aMax;
              sCount--;
            }
            steckerTable[s1Max]=s2Max;
            steckerTable[s2Max]=s1Max;
            sCount++;
        }
        else
        {
            printf("No improvement found\n");
        }
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));

    // Convert the stecker positions to a stecker brett string
    steckerTableToSteckers(steckerTable, results->settings.steckers);

    results->indexOfCoincidence=maxIoc;
    
    destroyEnigma(enigma);
    fflush(stdout);
}

/**************************************************************************************************\
* 
* After the rotor settings have been found, this method finds the steckers.
* It uses ngrams and scores the ngrams in the decoded message
* Improved method: also swap back existing letters
* 
\**************************************************************************************************/
void iocFindSteckeredCharsNgram(IocResults* results, int maxNumOfSteckers, int ngramSize)
{

    Enigma* enigma;
    int     sCount;
    int     s1;
    int     s2;
    int     s1a;
    int     s2a;
    int     s1Max;
    int     s2Max;
    int     s1aMax;
    int     s2aMax;
    int     steckerTable[MAX_POSITIONS];
    float   maxIoc;
    float   ioc;
    int     found;

    sCount=steckersToSteckerTable(results->settings.steckers, steckerTable);

    enigma=createEnigmaM3();

    // Initialise maxIoc based on given settings
    setEnigma(enigma, &results->settings);
    encodeDecode(enigma);
    maxIoc          =ngramScore(enigma, ngramSize);

    found=1;
    while (sCount<maxNumOfSteckers && found)
    {
        s1Max=-1;
        s2Max=-1;
        s1aMax=-1;
        s2aMax=-1;
        found=0;
        s1=0;
        while (s1<MAX_POSITIONS-1)
        {
            s2=s1+1;
            while (s2<MAX_POSITIONS)
            {
                if (steckerTable[s1]==s1 && steckerTable[s2]==s2)
                {
                    // Letters have not been steckered: Place stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s1a=-1;
                    s2a=-1;
                }
                else if (steckerTable[s1]!=s1 && steckerTable[s2]==s2)
                {
                    // s1 has been steckered before
                    // swap back
                    s1a=steckerTable[s1];
                    steckerTable[s1a]=s1a;
                    // place new stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s2a=-1;
                }
                else if (steckerTable[s1]==s1 && steckerTable[s2]!=s2)
                {
                    // s2 has been steckered before
                    // swap back
                    s2a=steckerTable[s2];
                    steckerTable[s2a]=s2a;
                    // place new stecker
                    steckerTable[s1]=s2;  
                    steckerTable[s2]=s1;
                    s1a=-1;
                }
                else //both letters have been steckered before
                {
                    s1a=steckerTable[s1];
                    s2a=steckerTable[s2];
                    // Now we have two cases: 
                    // - both letters have been swapped with other letters ..P.NS.Q.. 
                    //   In this case: swap back and place new stecker
                    // - both letters have been swapped interchangeably    ....QP....
                    //   In this case: just swap them back and see if it improves
                    if (s1a!=s2 && s2a!=s1)
                    {
                        // swap back
                        steckerTable[s1a]=s1a;
                        steckerTable[s2a]=s2a;
                        // place stecker
                        steckerTable[s1]=s2;
                        steckerTable[s2]=s1;
                    }
                    else
                    {
                        // swap back
                        steckerTable[s1]=s1;
                        steckerTable[s2]=s2;
                    }  
                }

                // Quick and dirty way to set up the steckers
                memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                
                setGrundStellung(enigma, 1, results->settings.grundStellungen[0]);
                setGrundStellung(enigma, 2, results->settings.grundStellungen[1]);
                setGrundStellung(enigma, 3, results->settings.grundStellungen[2]);
                
                encodeDecode(enigma);
                ioc=ngramScore(enigma, ngramSize);

                if (ioc>maxIoc)
                {
                    maxIoc=ioc;
                    s1Max=s1;
                    s2Max=s2;
                    s1aMax=s1a;
                    s2aMax=s2a;
                    found=1;
                }
                // Remove stecker
                if (s1a>=0)
                {
                    steckerTable[s1]=s1a;
                    steckerTable[s1a]=s1;
                }
                else
                {
                    steckerTable[s1]=s1;
                }
                if (s2a>=0)
                {
                    steckerTable[s2]=s2a;
                    steckerTable[s2a]=s2;
                }
                else
                {
                    steckerTable[s2]=s2;
                }

                s2++;
            }
            s1++;
        }
                   
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            printFoundLetters(s1Max, s2Max, s1aMax, s2aMax, maxIoc);

            if (steckerTable[s1Max]==s1Max && steckerTable[s2Max]==s2Max)
            {
                // Place stecker
                steckerTable[s1Max]=s2Max;
                steckerTable[s2Max]=s1Max;
                sCount++;
            }
            else if (steckerTable[s1Max]!=s1Max && steckerTable[s2Max]==s2Max)
            {
                // swap back
                steckerTable[s1aMax]=s1aMax;
                // place new stecker
                steckerTable[s1Max]=s2Max;
                steckerTable[s2Max]=s1Max;
            }
            else if (steckerTable[s1Max]==s1Max && steckerTable[s2Max]!=s2Max)
            {
                // swap back
                steckerTable[s2aMax]=s2aMax;
                // place new stecker
                steckerTable[s1Max]=s2Max;  
                steckerTable[s2Max]=s1Max;

            }
            else //both letters have been swapped before
            {

                // place new stecker, unless s1 and s2 had been swapped 
                // interchangeably; in the latter case: leave them swapped back
                if (s1aMax!=s2Max && s2aMax!=s1Max)
                {
                    // swap back
                    steckerTable[s1aMax]=s1aMax;
                    steckerTable[s2aMax]=s2aMax;
                    steckerTable[s1Max]=s2Max;
                    steckerTable[s2Max]=s1Max;
                }
                else
                {
                    steckerTable[s1Max]=s1Max;
                    steckerTable[s2Max]=s2Max;
                }  
                sCount--;
            }
        }
        else
        {
            printf("No more improvement found\n");
        }
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));

    // Convert the stecker positions to a stecker brett string
    steckerTableToSteckers(steckerTable, results->settings.steckers);
    
    destroyEnigma(enigma);
    fflush(stdout);
}

/**************************************************************************************************\
* 
* This method is meant to try to find best stecker settings for each rotor setting. 
* To be used inline
* 
\**************************************************************************************************/
void iocFindSteckeredCharsInline(Enigma* enigma, IocResults* results, int g1, int g2, int g3, int maxSteckers)
{
    int     sCount;
    int     s1;
    int     s2;
    int     s1a;
    int     s2a;
    int     s1Max;
    int     s2Max;
    int     s1aMax;
    int     s2aMax;
    int     steckerTable[MAX_POSITIONS];
    float   maxIoc;
    float   ioc;
    int     found;

    // Initialise stecker brett table: no steckers
    s1=0;
    while (s1<MAX_POSITIONS)
    {
        steckerTable[s1]=s1;
        s1++;
    }
    
    maxIoc          =-1000000.0;
    
    sCount=0;
    found=1;
    while (sCount<maxSteckers && found)
    {
        s1Max=-1;
        s2Max=-1;
        s1aMax=-1;
        s2aMax=-1;
        found=0;
        s1=0;
        while (s1<MAX_POSITIONS-1)
        {
            s2=s1+1;
            while (s2<MAX_POSITIONS)
            {
                if (steckerTable[s1]==s1 && steckerTable[s2]==s2)
                {
                    // Place stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s1a=-1;
                    s2a=-1;
                }
                else if (steckerTable[s1]!=s1 && steckerTable[s2]==s2)
                {
                    // swap back
                    s1a=steckerTable[s1];
                    steckerTable[s1a]=s1a;
                    // place new stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                    s2a=-1;
                }
                else if (steckerTable[s1]==s1 && steckerTable[s2]!=s2)
                {
                    // swap back
                    s2a=steckerTable[s2];
                    steckerTable[s2a]=s2a;
                    // place new stecker
                    steckerTable[s1]=s2;  
                    steckerTable[s2]=s1;
                    s1a=-1;
                }
                else
                {
                    // swap back
                    s1a=steckerTable[s1];
                    s2a=steckerTable[s2];
                    steckerTable[s1a]=s1a;
                    steckerTable[s2a]=s2a;
                    // place new stecker
                    steckerTable[s1]=s2;
                    steckerTable[s2]=s1;
                }
                    
                // Quick and dirty way to set up the steckers
                memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                
                setGrundStellung(enigma, 1, g1);
                setGrundStellung(enigma, 2, g2);
                setGrundStellung(enigma, 3, g3);
                
                encodeDecode(enigma);

                ioc=iocIndexOfCoincidence(enigma);

                if (ioc>maxIoc)
                {
                    maxIoc=ioc;
                    s1Max=s1;
                    s2Max=s2;
                    s1aMax=s1a;
                    s2aMax=s2a;
                    found=1;
                }
                
                // Remove stecker
                if (s1a>=0)
                {
                    steckerTable[s1]=s1a;
                    steckerTable[s1a]=s1;
                }
                else
                {
                    steckerTable[s1]=s1;
                }
                if (s2a>=0)
                {
                    steckerTable[s2]=s2a;
                    steckerTable[s2a]=s2;
                }
                else
                {
                    steckerTable[s2]=s2;
                }

                s2++;
            }
            s1++;
            
        }
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            if (s1aMax>=0)
            {
              steckerTable[s1aMax]=s1aMax;
              sCount--;
            }
            if (s2aMax>=0)
            {
              steckerTable[s2aMax]=s2aMax;
              sCount--;
            }
            steckerTable[s1Max]=s2Max;
            steckerTable[s2Max]=s1Max;
            sCount++;
        }
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));
   
    results->indexOfCoincidence=maxIoc;
        
}

/**************************************************************************************************\
* 
* This method follows the James Gillogly method.
* This method processes the indicated Waltzen permutations with UKW. It looks for the setting with
* the best index of coincidence. The best solutions are stored in the Top 10. 
* Whereas the original James Gillogly method fixed the R2 setting and tries to find the 
* best R2 setting after the rotor settings have been found, this function varies
* also the R2 setting. Takes more time, but all settings are tried.
* The parameter maxSteckers parameter defines the number of steckers the routine will look for.
*
* Set to 0 to just try Waltzen positions and use Index of Coincidence (original Gillogly method).
*
* Set to >0 to try to find the best steckers for each Waltzen position.
* For each rotor setting the steckers are tried, which makes this very slow.
* Best approach is to fix R2 to one setting, and try to find the best result. Then finalize 
* by trying all R2 for the found rotor setting.
* Works for short messages and >5 steckers
* 
\**************************************************************************************************/

void iocEvaluateEngimaSettings(IocWorkItem* work, int maxSteckers)
{
    Enigma*     enigma;
    int         r1, r2, r3;
    int         g1, g2, g3;
    int*        permutation;
    int         w;
    float       ioc;
    float       iocMax;
    IocResults* results;
    int         start;
    int         end;
    int         R1;
    int         startR2;
    int         endR2;
    int         startR3;
    int         endR3;
    char*       ukw;
    long        startTime;
    long        timeDiff;
    long        count;
    time_t      now;
    LinkedList* permutations;
    char*       cypher;
    int         maxCypherChars;
    char*       timeString;
    int         threadId;


    // The work item
    cypher          =work->cypher;
    maxCypherChars  =work->maxCypherChars;
    permutations    =work->permutations;
    start           =work->startPermutation;
    end             =work->endPermutation;
    ukw             =work->ukw;
    R1              =work->R1;
    startR2         =work->startR2;
    endR2           =work->endR2;
    startR3         =work->startR3;
    endR3           =work->endR3;
    threadId        =work->threadId;
    
    results     =malloc(sizeof(IocResults));
    
    enigma      =createEnigmaM3(); 

    placeUmkehrWaltze(enigma, ukw);
        
    clearSteckerBrett(enigma);

    setText(enigma, cypher);

    // limit number of cypher characters to speed up work
    // 250 will do
    if (enigma->textSize > maxCypherChars)
    {
        enigma->textSize=maxCypherChars;
    }

    count       =0;
    startTime   =time(NULL);

    iocMax      =-1000000.0;
    // Parse the Waltzen permutations assigned
    w           =start;
    while (w<=end)
    {
        permutation=(int*)elementAt(permutations, w);

        placeWaltze(enigma, 1, waltzen[permutation[0]]);
        placeWaltze(enigma, 2, waltzen[permutation[1]]);
        placeWaltze(enigma, 3, waltzen[permutation[2]]);

        time(&now);
        timeString=ctime(&now);
        timeString[strlen(timeString)-1]='\0';
        printf("%s: Thread %d trying permutation %02d (%d/%d): %s %s %s %s R1 %02d R2 %02d-%02d R3 %02d-%02d\n",
          timeString,
          threadId,
          w,
          w-start+1, end-start+1,
          ukw,
          waltzen[permutation[0]], 
          waltzen[permutation[1]], 
          waltzen[permutation[2]], 
          R1,
          startR2, endR2, 
          startR3, endR3);
        fflush(stdout);

        // The Ringstellung of the 1st Waltze has no meaning
        r1=R1;

        r2=startR2;
        while (r2<=endR2)
        {
            r3=startR3;
            while (r3<=endR3)
            {
                g1=1;
                while (g1<=MAX_POSITIONS)
                {
                    g2=1;
                    while (g2<=MAX_POSITIONS)
                    {
                        g3=1;
                        while (g3<=MAX_POSITIONS)
                        {
                            
                            setRingStellung(enigma, 1, r1);
                            setRingStellung(enigma, 2, r2);
                            setRingStellung(enigma, 3, r3);

                            if (maxSteckers>0)
                            {
                                iocFindSteckeredCharsInline(enigma, results, g1, g2, g3, maxSteckers);
                                ioc=results->indexOfCoincidence;
                            }
                            else
                            {
                                setGrundStellung(enigma, 1, g1);
                                setGrundStellung(enigma, 2, g2);
                                setGrundStellung(enigma, 3, g3);
                                encodeDecode(enigma);
                                ioc=iocIndexOfCoincidence(enigma);
                            }
                            if (ioc>iocMax)
                            {
                                results->indexOfCoincidence         =ioc;
                                results->settings.numberOfRotors    =3;
                                strncpy(results->settings.cypher, cypher, MAX_TEXT-1);
                                strncpy(results->settings.rotors[0], waltzen[permutation[0]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[1], waltzen[permutation[1]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[2], waltzen[permutation[2]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.ukw, ukw, MAX_ROTOR_NAME);
                                results->settings.ringStellungen[0] =r1;
                                results->settings.ringStellungen[1] =r2;
                                results->settings.ringStellungen[2] =r3;
                                results->settings.grundStellungen[0]=g1;
                                results->settings.grundStellungen[1]=g2;
                                results->settings.grundStellungen[2]=g3;
                        
                                pthread_mutex_lock(&iocMutex);
                                iocMax=iocStoreResults(results);
                                pthread_mutex_unlock(&iocMutex);
                            }
                            count++;
                            g3++;
                        }
                        g2++;
                    }
                    g1++;
                }
                r3++;
            }
            r2++;
        }
        w++;
    }
    destroyEnigma(enigma);  

    free(results);
    
    timeDiff=time(NULL)-startTime;
    if (timeDiff>0)
    {
        printf("Executed %ld decryptions in %ld minutes, %ld per sec\n", 
                count,
                timeDiff/60, 
                count/timeDiff);
    }
    pthread_mutex_lock(&iocMutex);
    pthread_mutex_unlock(&iocMutex);     
}

/**************************************************************************************************\
* 
* In the original Gillogly method, rotors are found assuming Ringstellung 1-1-1. After finding
* the rotors and Grundstellung, the Ringstellung is found by varying the Ringstellungen and 
* calculating the index of coincidence. First the 26 positions for R3 are tried. The best 
* solution is selected. Then the 26 positions of R2 are tried. The best solution is selected.
* 
* However, this frequently does not result in the optimal solutions, since it does not take into 
* account allowed Waltzen combinations, allowed by the inner working of the Enigma. 
* This improved version simulates the allowed Walzen combinations.
* 
\**************************************************************************************************/
void findRingStellung(IocResults*  results, int startRotor, int endRotor)
{
    int             rotor;
    int             inc;
    int             steps;
    int             ring;
    int             g1;
    int             g2;
    int             g3;
    float           ioc;
    float           maxIoc;
    int             maxR;
    int             maxG1;
    int             maxG2;
    int             maxG3;
    Enigma*         enigma;
    EnigmaSettings* settings;

    settings    =&results->settings;
    enigma      =createEnigmaM3(); 
    
    rotor       =endRotor;
    while (rotor>=startRotor)
    {
        maxR    =settings->ringStellungen[rotor-1];
        maxG1   =settings->grundStellungen[0];
        maxG2   =settings->grundStellungen[1];
        maxG3   =settings->grundStellungen[2];
        maxIoc  =results->indexOfCoincidence;
        inc    =-MAX_POSITIONS+1;
        while (inc<MAX_POSITIONS)
        {
            ring=1+inc;
            if (ring<1)
            {
                ring+=MAX_POSITIONS;
            }            
            setEnigma(enigma, settings);
            setRingStellung(enigma, rotor, ring);
            
            steps=ipow(MAX_POSITIONS, enigma->numberOfRotors-rotor)*inc;
            advances(enigma, steps);

            g1=getGrundStellung(enigma, 1);
            g2=getGrundStellung(enigma, 2);
            g3=getGrundStellung(enigma, 3);
            encodeDecode(enigma);
            ioc=iocIndexOfCoincidence(enigma);

            if (ioc>maxIoc)
            {
                maxIoc  =ioc;
                maxR    =ring;
                maxG1   =g1;
                maxG2   =g2;
                maxG3   =g3;
            }
            inc++;
        }

        settings->grundStellungen[0]        =maxG1;
        settings->grundStellungen[1]        =maxG2;
        settings->grundStellungen[2]        =maxG3;
        settings->ringStellungen[rotor-1]   =maxR;
        results->indexOfCoincidence         =maxIoc;
        printf("Rotor %d: best Ringstellung %2d, best Grundstellung %2d %2d %2d - IoC %f\n", rotor, maxR, maxG1, maxG2, maxG3, maxIoc);
        rotor--;
    }
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Thread function. Executes the next available work item at the end of the list
* 
\**************************************************************************************************/
void *iocThreadFunction(void *vargp) 
{
    int                 i;
    int                 done;
    int                 lastManStanding;
    IocWorkItem*        item;
    LinkedList*         permutations;
    IocThreadParams*    params;
    long                id;
    
    EvaluationMethod*   evalMethod;
    Method_t            method;
    int                 maxSteckers;
    int                 maxSteckersIoc;
    int                 ngramSize;
    int                 initialNumberOfWorkItems;
    int                 number;
    long                timeDiff;
    
    pthread_mutex_lock(&iocMutex);
    initialNumberOfWorkItems=iocInitialNumberOfWorkItems;
    iocLastThreadId++;
    id=iocLastThreadId;
    pthread_mutex_unlock(&iocMutex);
    
    params      =(IocThreadParams*)vargp;
    permutations=params->permutations;
    
    done=0;
    // Increase the number of threads running
    pthread_mutex_lock(&iocMutex);

    evalMethod    =params->evaluationMethod;
    method        =evalMethod->method;
    maxSteckers   =evalMethod->maxSteckers;
    maxSteckersIoc=evalMethod->maxSteckersIoc;
    ngramSize     =evalMethod->ngramSize;

    iocThreadsRunning++;
    pthread_mutex_unlock(&iocMutex);     
    
    while (!done)
    {
        // Pop the next item from the stack
        pthread_mutex_lock(&iocMutex);
        if (iocNumberOfWorkItems>0)
        {
            iocNumberOfWorkItems--;
            item=&iocWorkItems[iocNumberOfWorkItems];
            item->threadId=id;
        }
        else
        {
            done=1;
        }
        number=iocNumberOfWorkItems;       
        pthread_mutex_unlock(&iocMutex);     

        // PHASE 1
        // process the item
        if (!done)
        {
            printf("Thread %ld starting work item: %02d-%02d (%d/%d), %s, R1 %02d R2 %02d-%02d R3: %02d-%02d\n", 
                   id, item->startPermutation, item->endPermutation,
                   initialNumberOfWorkItems-number, initialNumberOfWorkItems, 
                   item->ukw, 
                   item->R1,
                   item->startR2, item->endR2, 
                   item->startR3, item->endR3);
            fflush(stdout);
            switch (method)
            {
                case METHOD_IOC:
                case METHOD_IOC_R2R3:
                case METHOD_IOC_R3:
                  iocEvaluateEngimaSettings(item, 0);
                  break;
                case METHOD_IOC_DEEP:
                  iocEvaluateEngimaSettings(item, maxSteckers);
                  break;
                case METHOD_IOC_NGRAM:
                  iocEvaluateEngimaSettings(item, maxSteckersIoc);
                  break;
            }
        }
    }
    
    // Decrease the number of threads running
    pthread_mutex_lock(&iocMutex);
    iocThreadsRunning--;
    if (iocThreadsRunning==0)
    {
        lastManStanding=1;
    }
    else
    {
        lastManStanding=0;
    }
    pthread_mutex_unlock(&iocMutex);     
    
    
    // PHASE 2
    // If this is the last thread, finish the job
    if (lastManStanding)
    {
        printf("Last man standing: %ld\n", id);
        iocDumpTopResults(0);
        switch (method)
        {
            case METHOD_IOC_R2R3:
                // Now we have got the Top 10 best results for rotor position and Ringstellung R1 R2 R3
                // Try to find
                // - Stecker Positions for each of them (only for the not deep method)
                i=0;
                while (i<iocNumberOfResults)
                {
                    printf("Finding steckers for %d\n", i);
                    iocFindSteckeredChars(&iocTopResults[i], maxSteckers);
                    i++;
                }
              break;
            case METHOD_IOC_R3:
                // Now we have got the Top 10 best results for rotor position and Ringstellung R1 R3
                // Try to find
                // - Ringstellung R2
                // - Stecker Positions for each of them (only for the not deep method)
                i=0;
                while (i<iocNumberOfResults)
                {
                    // find ringstellung for rotor R2
                    printf("Finding ring setting R2 for %d\n", i);
                    findRingStellung(&iocTopResults[i], 2, 2);
                    printf("Finding steckers for %d\n", i);
                    iocFindSteckeredChars(&iocTopResults[i], maxSteckers);
                    i++;
                }
              break;           
            case METHOD_IOC:
                // Now we have got the Top 10 best results for rotor position and Ringstellung R1
                // Try to find
                // - Ringstellung R2 and R3
                // - Stecker Positions for each of them (only for the not deep method)
                i=0;
                while (i<iocNumberOfResults)
                {
                    // find ringstellung for rotor R2 and R3
                    printf("Finding ring setting R2 and R3 for %d\n", i);
                    findRingStellung(&iocTopResults[i], 2, 3);
                    printf("Finding steckers for %d\n", i);
                    iocFindSteckeredChars(&iocTopResults[i], maxSteckers);
                    i++;
                }
              break;
            case METHOD_IOC_DEEP:
              break;
            case METHOD_IOC_NGRAM:            
                i=0;
                while (i<iocNumberOfResults)
                {
                    printf("Finding final steckers for %d\n", i);
                    iocFindSteckeredCharsNgram(&iocTopResults[i], maxSteckers, ngramSize);
                    i++;
                }
              break;
        }
        
        // Show the final result 
        printf("FOUND SOLUTIONS: \n");
        sortTopResults();
        iocDumpTopResults(1);

        if (permutations!=NULL)
        {
            destroyLinkedList(permutations);
        }
        evaluationEndTime=time(NULL);
        timeDiff=evaluationEndTime-evaluationStartTime;
        printf("Time elapsed: %ld'%02ld\"\n", timeDiff/60, timeDiff%60);
    }
    fflush(stdout);
    return NULL;
}

/**************************************************************************************************\
* 
* Tries to decode a cypher only text. Two methods are supported by means of isDeep.
* isDeep=0: First the rotor positions are detected, then the steckered positions. Works op to 
*           5-6 steckers
* isDeep=1: For each rotor position the steckers are tried
*
* Assumes the work items and work items number have been defined
* Pass pointer to permutations if the permutations must be cleaned up after finishing. Leave NULL
* if not
* 
\**************************************************************************************************/
void iocExecuteWorkItems (int numOfThreads, LinkedList* permutations)
{
    int             i;

    // create the threads
    iocInitialNumberOfWorkItems=iocNumberOfWorkItems;
    i=0;
    while (i<numOfThreads)
    {
        threadParams[i].permutations        =permutations;
        threadParams[i].evaluationMethod    =&evaluationMethod;

        pthread_create(&(threadParams[i].threadId), 
                       NULL, iocThreadFunction, 
                       (void *)(threadParams+i)); 
        i++;
    }
    pthread_exit(NULL);     
}

/**************************************************************************************************\
* 
* Defines the decryption method
* method          The method to use
* maxSteckers     The maximum number of Steckers to try
* maxSteckersIoc  METHOD_IOC_NGRAM: the max number of Steckers to use with ioc, rest will be ngram
* ngramSize       n in ngram (2 - bigrams, 3 - trigrams, etc)
* ngrams          Ngram set to use ('DE', 'EN', 'GC')
* 
\**************************************************************************************************/
void setEvaluationMethod(Method_t method, int maxSteckers, int maxSteckersIoc, int ngramSize, char* ngrams)
{
    evaluationMethod.method         =method;
    evaluationMethod.maxSteckers    =maxSteckers;
    evaluationMethod.maxSteckersIoc =maxSteckersIoc;
    evaluationMethod.ngramSize      =ngramSize;

    if (ngrams!=NULL)
    {
        strncpy(evaluationMethod.ngramSet, ngrams, MAX_NGRAMSETSIZE-1);
    }
    else
    {
        evaluationMethod.ngramSet[0]='\0';
    }
    
    // Initialise the NGRAM scoring
    if (ngramSize>0 && ngrams!=NULL)
    {
        prepareNgramScore(ngramSize, ngrams);
    }
}

/**************************************************************************************************\
* 
* Tries to decode a cypher only text. Two methods are supported by means of isDeep.
* isDeep=0: First the rotor positions are detected, then the steckered positions. Works op to 
*           5-6 steckers
* isDeep=1: For each rotor position the steckers are tried
* 
\**************************************************************************************************/
void iocDecodeText(char* cypher, int numOfThreads)
{
    int             i;
    LinkedList*     permutations;
    int             length;
    
    // Start with 5 Wehrmacht rotors
    permutations        =createRotorPermutations(3, 5);
    length              =linkedListLength(permutations);
   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads*2;

    i=0;
    while (i<numOfThreads)
    {
        iocWorkItems[i*2].cypher                =cypher;
        iocWorkItems[i*2].permutations          =permutations;
        iocWorkItems[i*2].startPermutation      =i*length/numOfThreads;
        iocWorkItems[i*2].endPermutation        =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2].R1                    =1;
        iocWorkItems[i*2].startR2               =1;
        if (evaluationMethod.method==METHOD_IOC_R2R3)
        {
            iocWorkItems[i*2].endR2             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2].endR2             =1;
        }
        iocWorkItems[i*2].startR3               =1;
        if (evaluationMethod.method==METHOD_IOC_R2R3 || evaluationMethod.method==METHOD_IOC_R3)
        {
            iocWorkItems[i*2].endR3             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2].endR3             =1;
        }
        iocWorkItems[i*2].maxCypherChars        =MAX_TEXT;
        strncpy(iocWorkItems[i*2].ukw, "UKW B", MAX_ROTOR_NAME);

        
        iocWorkItems[i*2+1].cypher              =cypher;
        iocWorkItems[i*2+1].permutations        =permutations;
        iocWorkItems[i*2+1].startPermutation    =i*length/numOfThreads;
        iocWorkItems[i*2+1].endPermutation      =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2+1].R1                  =1;
        iocWorkItems[i*2+1].startR2             =1;
        if (evaluationMethod.method==METHOD_IOC_R2R3)
        {
            iocWorkItems[i*2+1].endR2           =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2+1].endR2           =1;
        }
        iocWorkItems[i*2+1].startR3             =1;
        if (evaluationMethod.method==METHOD_IOC_R2R3 || evaluationMethod.method==METHOD_IOC_R3)
        {
            iocWorkItems[i*2+1].endR3           =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2+1].endR3           =1;
        }
        iocWorkItems[i*2+1].maxCypherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i*2+1].ukw, "UKW C", MAX_ROTOR_NAME);
        i++;
    }

    evaluationStartTime=time(NULL);
    iocExecuteWorkItems(numOfThreads, permutations);
}


