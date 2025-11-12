/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software can also try *all* rotor settings, 
* RingStellungen included.
*
\**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "enigma.h"
#include "coincidence.h"
#include "log.h"
#include "toolbox.h"
#include "ngrams.h"
#include "ngramscore.h"
#include "workDispatcher.h"

/**************************************************************************************************\
* DEFINES
\**************************************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_TRIGRAMS        54
#define MIN_IOC             -100000.0;

/** Defines the method for decrypting the engima cipher */
typedef struct
{
    IocRecipe       recipe;
    LinkedList*     permutations;               // List of rotor permutations
    int             numOfRotors;                // The number of Walzen (3 or 4)
    int             knownSteckerBrett[MAX_POSITIONS];
    int             numberOfKnownSteckers;
} Operation;

/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

EnigmaSettings      iocSettings;

int                 iocNumberOfResults=0;      
IocResults*         iocHighScores;
IocResults          swap;

Operation           operation;
IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
int                 iocInitialNumberOfWorkItems;

int                 iocThreadsRunning=0;
int                 iocLastThreadId=-1;

int                 ioctrigramCount[MAX_TRIGRAMS];

char                printBuffer[100];

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
    int     iocCharCount[MAX_POSITIONS];
    for (int c=0; c<MAX_POSITIONS; c++)
    {
        iocCharCount[c]=countConvertedChar(enigma, c);
    }
    
    float ioc=0.0;
    for (int c=0; c<MAX_POSITIONS; c++)
    {
        ioc+=iocCharCount[c]*(iocCharCount[c]-1);
    }
    ioc/=enigma->textSize*(enigma->textSize-1);
    return ioc;
}

/**************************************************************************************************\
* 
* Create and initialize a results instance
* 
\**************************************************************************************************/
IocResults* createIocResults()
{
    IocResults* results=malloc(sizeof(IocResults));
    return results;
}

/**************************************************************************************************\
* 
* Create a results instance
* 
\**************************************************************************************************/
void destroyIocResults(IocResults* results)
{
    free(results);
}

/**************************************************************************************************\
* 
* Create a highscore list and initialize it. Destroy it after use
* 
\**************************************************************************************************/
void initializeHighScoreList()
{
    iocHighScores=malloc(operation.recipe.scoreListSize*sizeof(IocResults));
    for(int i=0; i<operation.recipe.scoreListSize; i++)
    {
        iocHighScores[i].indexOfCoincidence=MIN_IOC;
    }
}

/**************************************************************************************************\
* 
* Destroy the highscore list
* 
\**************************************************************************************************/
void destroyHighScoreList()
{
    free(iocHighScores);
}

/**************************************************************************************************\
* 
* Store the results in the top 10 of best results
* Returns the lowest index of coincidence value in the list
* 
\**************************************************************************************************/
float iocStoreResults(IocResults* results)
{
    // Find the position to fit the results in, if any
    // Check if the ioc surpasses any ioc in the list
    int index   =-1;
    int i;
    for (i=0; i<iocNumberOfResults && index<0; i++)
    {
        if (results->indexOfCoincidence > iocHighScores[i].indexOfCoincidence)
        {
            index=i;
        }
    }
    // If not and the list is not full yet add it
    if ((index<0) && (i<operation.recipe.scoreListSize))
    {
        index=i;
    }
    
    if (index>=0)
    {
        // Shift all top 10 entries below
        for (i=iocNumberOfResults; i>index; i--)
        {
            if (i<operation.recipe.scoreListSize)
            {
                iocHighScores[i]=iocHighScores[i-1];
            }
        } 
        // Add the new record
        iocHighScores[index]=*results;

        if (iocNumberOfResults<operation.recipe.scoreListSize)
        {
            iocNumberOfResults++;
        }   
    }
    return iocHighScores[iocNumberOfResults-1].indexOfCoincidence;
}

/**************************************************************************************************\
* 
* After processing the top results list may no longer be sorted. This sorts the list.
* 
\**************************************************************************************************/
void sortTopResults()
{
    for (int i=0; i<operation.recipe.scoreListSize-1; i++)
    {
        for (int j=i+1; j<operation.recipe.scoreListSize; j++)
        {
            if (iocHighScores[j].indexOfCoincidence>iocHighScores[i].indexOfCoincidence)
            {
                swap            =iocHighScores[i];
                iocHighScores[i]=iocHighScores[j];
                iocHighScores[j]=swap;
            }
        }        
    }
}

/**************************************************************************************************\
* 
* Show the top results. En passant, translate the found steckertable into stecker pairs
* 
\**************************************************************************************************/
void iocDumpHighScores(int number, bool withDecode)
{
    int             i;
    int             s1, s2;
    IocResults*     results;
    EnigmaSettings* settings;   
    Enigma*         enigma;

    printf("Top %d best results of :\n", number);
    enigma=createEnigmaM3();
    i=0;
    while (i<number)
    {
        results =&iocHighScores[i];
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
            displayEnigmaMessage(toString(enigma), operation.recipe.displayFormat);
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
    char c1, c2, c3, c4;
    if (s1aMax>=0)
    {
        c1=s1aMax+'A';
        c2=s1Max +'A';
    }
    else
    {
        c1=' ';
        c2=' ';
    }
    if (s2aMax>=0)
    {
        c3=s2Max+'A';
        c4=s2aMax +'A';
    }
    else
    {
        c3=' ';
        c4=' ';
    }
    logInfo("Found steckered chars %c-%c (%c-%c %c-%c)", s1Max+'A', s2Max+'A', c1, c2, c3, c4);
}

/**************************************************************************************************\
* 
* After the rotor settings and Ringstellungs have been found, this method finds the steckers. 
* It uses the Index of Coincidence or NGRAM as measure of plain text fitness
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
    int     steckerTable[MAX_POSITIONS];

    // Initialise stecker brett table
    int sCount=steckersToSteckerTable(results->settings.steckers, steckerTable);

    // Setup the enigma
    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, &results->settings);
    
    // The IoC found by just moving the rotors, no steckers
    //float maxIoc          =results->indexOfCoincidence;
    // Just reset the max; needed when switching from IoC to NGRAMs 
    float maxIoc          =MIN_IOC;
    
    bool found=true;
    while (sCount<maxNumOfSteckers && found)
    {
        float   ioc;
        int     s1a;
        int     s2a;
        int     s1Max=-1;
        int     s2Max=-1;
        int     s1aMax=-1;
        int     s2aMax=-1;
        found=false;
        for (int s1=0; s1<MAX_POSITIONS-1;s1++)
        {
            for (int s2=s1+1; s2<MAX_POSITIONS; s2++)
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
                if (operation.recipe.evalSteckers==EVAL_NGRAM)
                {
                    ioc=ngramScore(enigma);
                }
                else
                {
                    ioc=iocIndexOfCoincidence(enigma);
                }

                if (ioc>maxIoc)
                {
                    maxIoc=ioc;
                    s1Max=s1;
                    s2Max=s2;
                    s1aMax=s1a;
                    s1aMax=s2a;
                    found=true;
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
            }
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
* This method is meant to try to find best stecker settings for each rotor setting. 
* To be used inline
* 
\**************************************************************************************************/
void iocFindSteckeredCharsInline(Enigma* enigma, IocResults* results, int g1, int g2, int g3, int maxSteckers)
{
    int     steckerTable[MAX_POSITIONS];

    // Initialise stecker brett table: initialize and place any known steckers
    int     sCount=steckersToSteckerTable(operation.recipe.knownSteckers, steckerTable);
    float   maxIoc          =MIN_IOC;
    bool    found           =true;

    // In the case we already have our steckers, we need a maxIoC
    if (sCount==maxSteckers)
    {
        // Quick and dirty way to set up the steckers
        memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
        
        setGrundStellung(enigma, 1, g1);
        setGrundStellung(enigma, 2, g2);
        setGrundStellung(enigma, 3, g3);
        
        encodeDecode(enigma);
        maxIoc=iocIndexOfCoincidence(enigma);
        maxIoc=ngramScore(enigma);
    }
    // else
    while (sCount<maxSteckers && found)
    {
        float   ioc;
        int     s1;
        int     s2;
        int     s1a;
        int     s2a;
        int     s1Max   =-1;
        int     s2Max   =-1;
        int     s1aMax  =-1;
        int     s2aMax  =-1;
        found           =false;
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
                if (operation.recipe.evalSteckers==EVAL_IOC)
                {
                    ioc=ngramScore(enigma);
                }
                else
                {
                    ioc=iocIndexOfCoincidence(enigma);
                }

                ioc=iocIndexOfCoincidence(enigma);

                if (ioc>maxIoc)
                {
                    maxIoc  =ioc;
                    s1Max   =s1;
                    s2Max   =s2;
                    s1aMax  =s1a;
                    s2aMax  =s2a;
                    found   =true;
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
    // Update the results: Store the raw stecker table and highes IoC
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));
    results->indexOfCoincidence=maxIoc;
}


/**************************************************************************************************\
* 
* This method follows the James Gillogly method.
* This method processes the indicated Walzen permutations with UKW. It looks for the setting with
* the best index of coincidence or ngram score. The best solutions are stored in the Top 10. 
* Whereas the original James Gillogly method fixed the R1, R2 and R3 setting and tries to find the 
* best R2 and R3 setting after the rotor settings have been found, this function might also vary
* the R2 and R3 setting. Takes more time, but all settings are tried.
*
* Set to 0 to just try Walzen positions and use Index of Coincidence (original Gillogly method).
*
* Set to >0 to try to find the best steckers for each Walzen position.
* For each rotor setting the steckers are tried, which makes this very slow.
* Best approach is to fix R2 to one setting, and try to find the best result. Then finalize 
* by trying all R2 for the found rotor setting.
* Works for short messages and >5 steckers
* 
\**************************************************************************************************/
void iocEvaluateEngimaSettings(IocWorkItem* work)
{
    int         r1, r2, r3;
    int         g1, g2, g3;
    int*        permutation;
    int         w;
    float       ioc;
    float       iocMax;
    int         start;
    int         end;
    int         R1;
    int         startR2;
    int         endR2;
    int         startR3;
    int         endR3;
    long        startTime;
    long        timeDiff;
    long        count;
    time_t      now;
    LinkedList* permutations;
    char*       cipher;
    int         maxCipherChars;
    char*       timeString;
    int         threadId;


    // The work item
    cipher              =operation.recipe.cipher;
    maxCipherChars      =work->maxCipherChars;
    permutations        =operation.permutations;
    start               =work->startPermutation;
    end                 =work->endPermutation;
    R1                  =work->R1;
    startR2             =work->startR2;
    endR2               =work->endR2;
    startR3             =work->startR3;
    endR3               =work->endR3;
    threadId            =work->threadId;
    
    IocResults* results =createIocResults();
    
    Enigma*     enigma  =createEnigmaM3(); 

    // Quick and dirty way to set up the steckers
    memcpy(enigma->steckerBrett , operation.knownSteckerBrett, MAX_POSITIONS*sizeof(int));
    memcpy(results->steckerTable, operation.knownSteckerBrett, MAX_POSITIONS*sizeof(int));

    setText(enigma, cipher);

    // limit number of cipher characters to speed up work
    // 250 will do
    if (enigma->textSize > maxCipherChars)
    {
        enigma->textSize=maxCipherChars;
    }

    count       =0;
    startTime   =time(NULL);

    iocMax      =MIN_IOC;
    // Parse the Walzen permutations assigned
    w           =start;
    while (w<=end)
    {
        permutation=(int*)elementAt(permutations, w);
        placeUmkehrWalze(enigma, umkehrWalzeNames[permutation[0]]);

        placeWalze(enigma, 1, rotorNames[permutation[1]]);
        placeWalze(enigma, 2, rotorNames[permutation[2]]);
        placeWalze(enigma, 3, rotorNames[permutation[3]]);
        placeUmkehrWalze(enigma, umkehrWalzeNames[permutation[0]]);

        time(&now);
        timeString=ctime(&now);
        timeString[strlen(timeString)-1]='\0';
        logInfo("%s: Worker %02d trying permutation %03d (%02d/%02d): %5s %5s %5s %5s R1 %02d R2 %02d-%02d R3 %02d-%02d",
          timeString,
          threadId,
          w,
          w-start+1, end-start+1,
          umkehrWalzeNames[permutation[0]],
          walzen[permutation[1]], 
          walzen[permutation[2]], 
          walzen[permutation[3]], 
          R1,
          startR2, endR2, 
          startR3, endR3);
        fflush(stdout);

        // The Ringstellung of the 1st Walze has no meaning
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

                            if (operation.recipe.maxSteckersInline>0)
                            {
                                // This updates the indexOfCoincidence and steckerTable in the results object
                                iocFindSteckeredCharsInline(enigma, results, g1, g2, g3, operation.recipe.maxSteckersInline);
                                ioc=results->indexOfCoincidence;
                            }
                            else
                            {
                                setGrundStellung(enigma, 1, g1);
                                setGrundStellung(enigma, 2, g2);
                                setGrundStellung(enigma, 3, g3);
                                encodeDecode(enigma);

                                if (operation.recipe.evalWalzen==EVAL_NGRAM)
                                {
                                    ioc=ngramScore(enigma);
                                }
                                else
                                {
                                    ioc=iocIndexOfCoincidence(enigma);
                                }
                            }

                            if (ioc>iocMax)
                            {
                                results->indexOfCoincidence         =ioc;
                                results->settings.numberOfRotors    =3;
                                strncpy(results->settings.cipher, cipher, MAX_TEXT-1);
                                strncpy(results->settings.rotors[0], rotorNames[permutation[1]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[1], rotorNames[permutation[2]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[2], rotorNames[permutation[3]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.ukw, umkehrWalzeNames[permutation[0]], MAX_ROTOR_NAME-1);
                                results->settings.ringStellungen[0] =r1;
                                results->settings.ringStellungen[1] =r2;
                                results->settings.ringStellungen[2] =r3;
                                results->settings.grundStellungen[0]=g1;
                                results->settings.grundStellungen[1]=g2;
                                results->settings.grundStellungen[2]=g3;
                                mutexLock();
                                iocMax=iocStoreResults(results);
                                mutexUnlock();
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

    destroyIocResults(results);
    
    timeDiff=time(NULL)-startTime;
    if (timeDiff>0)
    {
        logInfo("Executed %ld decryptions in %ld minutes, %ld per sec", 
                count,
                timeDiff/60, 
                count/timeDiff);
    }
}

/**************************************************************************************************\
* 
* In the original Gillogly method, rotors are found assuming Ringstellung 1-1-1. After finding
* the rotors and Grundstellung, the Ringstellung is found by varying the Ringstellungen and 
* calculating the index of coincidence. First the 26 positions for R3 are tried. The best 
* solution is selected. Then the 26 positions of R2 are tried. The best solution is selected.
* 
* However, this frequently does not result in the optimal solutions, since it does not take into 
* account allowed Walzen combinations, allowed by the inner working of the Enigma. 
* This improved version simulates the allowed Walzen combinations.
* 
\**************************************************************************************************/
void iocFindRingStellung(IocResults*  results, int startRotor, int endRotor)
{
    int             steps;
    int             ring;
    int             g1;
    int             g2;
    int             g3;
    float           ioc;

    EnigmaSettings* settings    =&results->settings;
    Enigma*         enigma      =createEnigmaM3();
    for (int rotor=endRotor;rotor>=startRotor; rotor--)
    {
        int   maxR      =settings->ringStellungen[rotor-1];
        int   maxG1     =settings->grundStellungen[0];
        int   maxG2     =settings->grundStellungen[1];
        int   maxG3     =settings->grundStellungen[2];
        float maxIoc    =results->indexOfCoincidence;
        int inc         =-MAX_POSITIONS+1;
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

            if (operation.recipe.evalWalzen==EVAL_NGRAM)
            {
                ioc=ngramScore(enigma);
            }
            else
            {
                ioc=iocIndexOfCoincidence(enigma);
            }

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
        logInfo("Rotor %d: best Ringstellung %2d, best Grundstellung %2d %2d %2d - IoC %f", rotor, maxR, maxG1, maxG2, maxG3, maxIoc);
    }
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Worker function. Processes the work item parsed. Basically, the proper Walzen settings are 
* looked for
* 
\**************************************************************************************************/
void iocWorkerFunction(int worker, int workItem, void* params)
{
    IocWorkItem*        item;
    int                 initialNumberOfWorkItems;

    item=(IocWorkItem*)params;
    item->threadId=worker;

    initialNumberOfWorkItems=dispatcherGetTotalWorkItems();

    logInfo("Worker %02d starting work item: %02d-%02d (%02d/%02d), R1 %02d R2 %02d-%02d R3: %02d-%02d", 
            worker, item->startPermutation, item->endPermutation,
            initialNumberOfWorkItems-workItem, initialNumberOfWorkItems, 
            item->R1,
            item->startR2, item->endR2, 
            item->startR3, item->endR3);
    fflush(stdout);

    iocEvaluateEngimaSettings(item); 
}


/**************************************************************************************************\
* 
* Finish function. Executed after the work is done
* 
\**************************************************************************************************/
void iocFinishFunction(void* params)
{
    Depth_t method  =operation.recipe.method; 
    int maxSteckers =operation.recipe.maxSteckers;

    // We now have a list of rotor settings sorted on IoC
    iocDumpHighScores(operation.recipe.scoreListSize, true);

    // First see if there are any ringstellungen left to find
    switch (method)
    {
        case DEPTH_R2_R3:
            // Now we have got the Top 10 best results for rotor position and Ringstellung R1 R2 R3
            // Nothing to be done here
            break;
        case DEPTH_R3:
            // Now we have got the Top 10 best results for rotor position and Ringstellung R1 R3
            // Try to find
            // - Ringstellung R2
            for (int i=0; i<iocNumberOfResults; i++)
            {
                // find ringstellung for rotor R2
                logInfo("Finding ring setting R2 for %d", i);
                iocFindRingStellung(&iocHighScores[i], 2, 2);
            }
            break;           
        case DEPTH_NONE:
            // Now we have got the Top 10 best results for rotor position and Ringstellung R1
            // Try to find
            // - Ringstellung R2 and R3
            for (int i=0; i<iocNumberOfResults; i++)
            {
                // find ringstellung for rotor R2 and R3
                logInfo("Finding ring setting R2 and R3 for %d", i);
                iocFindRingStellung(&iocHighScores[i], 2, 3);
            }
            break;
    }

    // Lets sort the results
    sortTopResults();
    iocDumpHighScores(operation.recipe.numberOfSolutions, false);

    // Finally we are going to look for the steckers for the best result
    for(int i=0;i<operation.recipe.numberOfSolutions; i++)
    {
        logInfo("Finding final steckers for result %d", i);
        iocFindSteckeredChars(&iocHighScores[i], maxSteckers);
    }
}

/**************************************************************************************************\
* 
* Creates a list of permutations of UKW/Walzen based on the recipe. 
* The object contains 4 or 5 ints:
* M3:
* int 0: the UKW index
* int 1: Walze 1
* int 2: Walze 2
* int 3: Walze 3
*
* M4:
* int 0: the UKW index
* int 1: Walze 1
* int 2: Walze 2
* int 3: Walze 3
* int 4: Walze 4
* The index is the index in rotorNames resp. umkerhWalzenNames!
* 
\**************************************************************************************************/
LinkedList* generateWalzePermutations(IocRecipe recipe)
{
    LinkedList* ukwPermutations         =NULL;
    LinkedList* walzenPermutations      =NULL;
    LinkedList* permutations            =NULL;

    walzenPermutations  =getWalzenPermutations(recipe.enigmaType, recipe.rotorSet);
    ukwPermutations     =getUkwPermutations   (recipe.enigmaType, recipe.rotorSet);
    if (recipe.enigmaType==ENIGMATYPE_M4)
    {
        operation.numOfRotors=4;
    }
    else
    {
        operation.numOfRotors=3;
    }
    permutations=combinePermutations(ukwPermutations, 1, walzenPermutations, operation.numOfRotors);
    destroyPermutations(walzenPermutations);
    destroyPermutations(ukwPermutations);
    return permutations;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void iocSetCustomWalzePermutations(LinkedList* permutations)
{
    operation.permutations   =permutations;
}

/**************************************************************************************************\
* 
* Verbose onm operation method
* 
\**************************************************************************************************/

void iocReportMethod()
{
    printf("#####################################################################################\n");
    switch (operation.recipe.enigmaType)
    {
        case ENIGMATYPE_M3:
            printf("# Enigma machine              : Engima M3 (3 Walzen)\n");
            break;
        case ENIGMATYPE_M4:
            printf("# Enigma machine              : Engima M4 (4 Walzen)\n");
            break;
    }

    switch (operation.recipe.rotorSet)
    {
        case M3_ENIGMA1_1930:
            printf("# Walzen                      : Enigma 1, 3 walzen\n");
            break;
        case M3_ARMY_1938:
            printf("# Walzen                      : Wehrmacht 1938, 5 walzen\n");
            break;
        case M3_ARMY_1939:
            printf("# Walzen                      : Wehrmacht 1939, 8 walzen\n");
            break;
        case M4_NAVAL_1941:
            printf("# Walzen                      : Kriegsmarine Engima M4\n");
            break;
    }

    switch (operation.recipe.method)
    {
        case DEPTH_NONE:
            printf("# Method                      : Keeping R1, R2, R3 fixed (Gillogly)\n");
            break;
        case DEPTH_R3:
            printf("# Method                      : Keeping R1, R2 fixed\n");
            break;
        case DEPTH_R2_R3:
            printf("# Method                      : Keeping R1 fixed\n");
            break;
    }
    printf("# Evaluation for rotor        : ");
    switch (operation.recipe.evalWalzen)
    {
        case EVAL_IOC:
            printf("IoC\n");
            break;
        case EVAL_NGRAM:
            printf("NGRAMs, size %d\n", operation.recipe.ngramSize);
            break;
    }
    printf("# Evaluation for stecker      : ");
    switch (operation.recipe.evalWalzen)
    {
        case EVAL_IOC:
            printf("IoC\n");
            break;
        case EVAL_NGRAM:
            printf("NGRAMs, size %d\n", operation.recipe.ngramSize);
            break;
    }

    printf("# Max steckers                : %02d\n", operation.recipe.maxSteckers);
    printf("# Max steckers inline         : %02d\n", operation.recipe.maxSteckersInline);
    printf("# Result list size (Walzen)   : %d\n"  , operation.recipe.scoreListSize);
    printf("# Number to show (Steckers)   : %d\n"  , operation.recipe.numberOfSolutions);
    printf("# Number of threads to use    : %d\n"  , operation.recipe.numberOfThreads);
    printf("# Cipher string length        : %d\n"  , (int)strlen(operation.recipe.cipher));
    printf("# Cipher size for decoding    : %d\n"  , operation.recipe.cipherSize);
    printf("# Cipher                      : \n");
    printCipher(operation.recipe.cipher);
    printf("#####################################################################################\n");
}

/**************************************************************************************************\
* 
* Initialize the IoC solver by setting the recipe and Walze permutations to use
* 
\**************************************************************************************************/
void iocInitialize(IocRecipe recipe, LinkedList* permutations)
{
    // Sanity checks
    if (recipe.scoreListSize<1)
    {
        logFatal("Illegal score list size, should be larger than 0");
    }
    if (recipe.numberOfSolutions>recipe.scoreListSize)
    {
        logFatal("Illegal number of solutions %d requested, should be smaller that score list size %d", 
                 recipe.numberOfSolutions,
                 recipe.scoreListSize);
    }

    // Initialise the NGRAM scoring
    if (recipe.ngramSize>0 && strcmp(recipe.ngramSet, "")!=0)
    {
        logInfo("Preparing NGRAMs size %d, language %s", recipe.ngramSize, recipe.ngramSet);
        prepareNgramScore(recipe.ngramSize, recipe.ngramSet);
    }

    // Fill a Steckerbrett with the known steckers
    operation.numberOfKnownSteckers=steckersToSteckerTable(recipe.knownSteckers, operation.knownSteckerBrett); 

    // Store recipe and permutations
    operation.recipe        =recipe;
    operation.permutations  =permutations;

    // Create the highscore list
    initializeHighScoreList();
}


/**************************************************************************************************\
* 
* Prepare work items for M3
* Currently it assumes 
* - an Enigma M3
* 
\**************************************************************************************************/
void prepareWorkM3()
{
    // Create the stack of work for the trheads
    int length              =linkedListLength(operation.permutations);
    int numOfThreads        =operation.recipe.numberOfThreads;

    dispatcherClearWorkItems();
    for (int i=0;i<numOfThreads; i++)
    {
        iocWorkItems[i].startPermutation      =i*length/numOfThreads;
        iocWorkItems[i].endPermutation        =(i+1)*length/numOfThreads-1;
        iocWorkItems[i].R1                    =1;
        iocWorkItems[i].startR2               =1;
        if (operation.recipe.method==DEPTH_R2_R3)
        {
            iocWorkItems[i].endR2             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i].endR2             =1;
        }
        iocWorkItems[i].startR3               =1;
        if (operation.recipe.method==DEPTH_R2_R3 || operation.recipe.method==DEPTH_R3)
        {
            iocWorkItems[i].endR3             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i].endR3             =1;
        }
        iocWorkItems[i].maxCipherChars        =operation.recipe.cipherSize;
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i]);
    }
}

/**************************************************************************************************\
* 
* Prepare work items for M4
* 
\**************************************************************************************************/
void prepareWorkM4()
{
    logFatal("Not supported... yet\n");
}

/**************************************************************************************************\
* 
* Tries to decode a cipher only text. Recipe defines how decryption takes. If you do not define
* customPermutations, the software calculates the Walzen permutations based on the enigma (M3 or M4)
* and the used Walzen set. If you have knowledge on the Walzen that have been used, you can provide
* your own Walzen permutation.
* recipe: Recipe to configure the decryption
* customPermutations: Custom walzen permutation or NULL if all permutations should be tried.
* 
\**************************************************************************************************/
void iocDecodeText(IocRecipe recipe, LinkedList* customPermutations)
{
    LinkedList* permutations;
    if (customPermutations==NULL)
    {
        permutations=generateWalzePermutations(recipe);
    }
    else
    {
        permutations=customPermutations;
    }
    iocInitialize(recipe, permutations);

    iocReportMethod();
    if (operation.recipe.enigmaType==ENIGMATYPE_M3)
    {
        prepareWorkM3();
    }
    else if (operation.recipe.enigmaType==ENIGMATYPE_M4)
    {
        prepareWorkM4();
    }
    else
    {
        logFatal("Error: non existing Enigma\n");
    }
    // Start work, distribute it over the threads. 
    // This function returns if all threads finished their work
    // and the iocFinishFunction has been executed.
    dispatcherStartWork(operation.recipe.numberOfThreads, iocFinishFunction, NULL, true);

    // Show the final result 
    printf("FOUND SOLUTION: \n");
    iocDumpHighScores(operation.recipe.numberOfSolutions, true);

    if (operation.permutations!=NULL)
    {
        destroyLinkedList(operation.permutations);
    }
    destroyHighScoreList();
}

/**************************************************************************************************\
* 
* Creates and returns a default recipe with plausible values. Of course you can modify it.
* Destroy after use!
* 
\**************************************************************************************************/
IocRecipe* createDefaultRecipe(char* cipher, int numberOfThreads)
{
    IocRecipe* recipe=malloc(sizeof(IocRecipe));
    recipe->enigmaType          =ENIGMATYPE_M3;
    recipe->rotorSet            =M3_ARMY_1938;
    recipe->method              =DEPTH_NONE;
    recipe->evalWalzen          =EVAL_IOC;
    recipe->evalSteckers        =EVAL_IOC;
    recipe->maxSteckers         =10;
    recipe->maxSteckersInline   =0;
    recipe->knownSteckers[0]    ='\0';
    recipe->ngramSize           =0;
    recipe->ngramSet[0]         ='\0';
    recipe->scoreListSize       =25;
    recipe->numberOfSolutions   =5;
    recipe->numberOfThreads     =numberOfThreads;
    recipe->cipher              =cipher;
    recipe->cipherSize          =MAX_TEXT;
    recipe->displayFormat       =MESSAGEFORMAT_TEXT;
    return recipe;
}

/**************************************************************************************************\
* 
* Destroy the recipe
* 
\**************************************************************************************************/
void destroyRecipe(IocRecipe* recipe)
{
    free(recipe);
}