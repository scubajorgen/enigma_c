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
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "enigma.h"
#include "coincidence.h"
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

/** Defines the method for decrypting the engima cipher */
typedef struct
{
    IocRecipe       recipe;
    LinkedList*     permutations;               // List of rotor permutations
} Operation;

/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

EnigmaSettings      iocSettings;

int                 iocNumberOfResults=0;      
IocResults          iocTopResults[TOP_RESULTS_SIZE];
IocResults          swap;

Operation           operation;
IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
int                 iocNumberOfWorkItems;
int                 iocInitialNumberOfWorkItems;

int                 iocThreadsRunning=0;
int                 iocLastThreadId=-1;

int                 ioctrigramCount[MAX_TRIGRAMS];

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
* Create and initialize a results instance
* 
\**************************************************************************************************/
IocResults* createIocResults()
{
    IocResults* results     =malloc(sizeof(IocResults));
    int i;
    for (i=0;i<MAX_POSITIONS;i++)
    {
        results->steckerTable[i]=0;
    }
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
            if (iocNumberOfResults<TOP_RESULTS_SIZE)
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
void iocDumpTopResults(int number, int withDecode)
{
    int             i;
    int             s1, s2;
    IocResults*     results;
    EnigmaSettings* settings;   
    Enigma*         enigma;

    if (number>TOP_RESULTS_SIZE)
    {
        number=TOP_RESULTS_SIZE;
    }

    printf("Top %d best results of :\n", TOP_RESULTS_SHOW);
    enigma=createEnigmaM3();
    i=0;
    while (i<number)
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
    maxIoc          =-1000000.0;
    
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
                if (operation.recipe.evalSteckers==EVAL_NGRAM)
                {
                    ioc=ngramScore(enigma, operation.recipe.ngramSize);
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
/*
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
    
    results->indexOfCoincidence=maxIoc;

    destroyEnigma(enigma);
    fflush(stdout);
}
*/
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
    char*       cipher;
    int         maxCipherChars;
    char*       timeString;
    int         threadId;


    // The work item
    cipher          =work->cipher;
    maxCipherChars  =work->maxCipherChars;
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
    
    results     =createIocResults();
    
    enigma      =createEnigmaM3(); 

    placeUmkehrWalze(enigma, ukw);
        
    clearSteckerBrett(enigma);

    setText(enigma, cipher);

    // limit number of cipher characters to speed up work
    // 250 will do
    if (enigma->textSize > maxCipherChars)
    {
        enigma->textSize=maxCipherChars;
    }

    count       =0;
    startTime   =time(NULL);

    iocMax      =-1000000.0;
    // Parse the Walzen permutations assigned
    w           =start;
    while (w<=end)
    {
        permutation=(int*)elementAt(permutations, w);

        placeWalze(enigma, 1, walzen[permutation[0]]);
        placeWalze(enigma, 2, walzen[permutation[1]]);
        placeWalze(enigma, 3, walzen[permutation[2]]);

        time(&now);
        timeString=ctime(&now);
        timeString[strlen(timeString)-1]='\0';
        printf("%s: Worker %02d trying permutation %02d (%02d/%02d): %5s %5s %5s %5s R1 %02d R2 %02d-%02d R3 %02d-%02d\n",
          timeString,
          threadId,
          w,
          w-start+1, end-start+1,
          ukw,
          walzen[permutation[0]], 
          walzen[permutation[1]], 
          walzen[permutation[2]], 
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
                            setGrundStellung(enigma, 1, g1);
                            setGrundStellung(enigma, 2, g2);
                            setGrundStellung(enigma, 3, g3);
                            encodeDecode(enigma);

                            if (operation.recipe.evalWalzen==EVAL_NGRAM)
                            {
                                ioc=ngramScore(enigma, operation.recipe.ngramSize);
                            }
                            else
                            {
                                ioc=iocIndexOfCoincidence(enigma);
                            }

                            if (ioc>iocMax)
                            {
                                results->indexOfCoincidence         =ioc;
                                results->settings.numberOfRotors    =3;
                                strncpy(results->settings.cipher, cipher, MAX_TEXT-1);
                                strncpy(results->settings.rotors[0], walzen[permutation[0]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[1], walzen[permutation[1]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.rotors[2], walzen[permutation[2]], MAX_ROTOR_NAME-1);
                                strncpy(results->settings.ukw, ukw, MAX_ROTOR_NAME);
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
        printf("Executed %ld decryptions in %ld minutes, %ld per sec\n", 
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
//            ioc=iocIndexOfCoincidence(enigma);
            if (operation.recipe.evalWalzen==EVAL_NGRAM)
            {
                ioc=ngramScore(enigma, operation.recipe.ngramSize);
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
        printf("Rotor %d: best Ringstellung %2d, best Grundstellung %2d %2d %2d - IoC %f\n", rotor, maxR, maxG1, maxG2, maxG3, maxIoc);
        rotor--;
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

    printf("Worker %02d starting work item: %02d-%02d (%02d/%02d), %s, R1 %02d R2 %02d-%02d R3: %02d-%02d\n", 
            worker, item->startPermutation, item->endPermutation,
            initialNumberOfWorkItems-workItem, initialNumberOfWorkItems, 
            item->ukw, 
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
    Depth_t             method;
    int                 maxSteckers;
    int                 i;

    method                  =operation.recipe.method; 
    maxSteckers             =operation.recipe.maxSteckers;

    // We now have a list of rotor settings sorted on IoC
    iocDumpTopResults(TOP_RESULTS_SHOW, 0);

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
            i=0;
            while (i<iocNumberOfResults)
            {
                // find ringstellung for rotor R2
                printf("Finding ring setting R2 for %d\n", i);
                iocFindRingStellung(&iocTopResults[i], 2, 2);
                i++;
            }
            break;           
        case DEPTH_NONE:
            // Now we have got the Top 10 best results for rotor position and Ringstellung R1
            // Try to find
            // - Ringstellung R2 and R3
            i=0;
            while (i<iocNumberOfResults)
            {
                // find ringstellung for rotor R2 and R3
                printf("Finding ring setting R2 and R3 for %d\n", i);
                iocFindRingStellung(&iocTopResults[i], 2, 3);
                i++;
            }
            break;
    }

    // Lets sort the results
    sortTopResults();
    iocDumpTopResults(TOP_RESULTS_SHOW, 0);

    // Finally we are going to look for the steckers for the best result
    for(i=0;i<TOP_RESULTS_SHOW;i++)
    {
        printf("Finding final steckers for result %d\n", i);
        iocFindSteckeredChars(&iocTopResults[i], maxSteckers);
    }

    // Show the final result 
    printf("FOUND SOLUTION: \n");
    //sortTopResults();
    iocDumpTopResults(TOP_RESULTS_SHOW, 1);

    if (operation.permutations!=NULL)
    {
        destroyLinkedList(operation.permutations);
    }

}

/**************************************************************************************************\
* 
* Set the list of Walze/rotor permutations
* 
\**************************************************************************************************/
void setWalzePermutations(LinkedList* permutations)
{
    operation.permutations   =permutations;
}

/**************************************************************************************************\
* 
* Verbose onm operation method
* 
\**************************************************************************************************/

void reportMethod()
{
    switch (operation.recipe.enigmaType)
    {
        case ENIGMATYPE_M3:
            printf("Enigma machine              : Engima M3 (3 Walzen)");
            break;
        case ENIGMATYPE_M4:
            printf("Enigma machine              : Engima M4 (4 Walzen)");
            break;
    }

    switch (operation.recipe.rotorSet)
    {
        case M3_ENIGMA1_1930:
            printf("Walzen                      : Enigma 1, 3 walzen");
            break;
        case M3_ARMY_1938:
            printf("Walzen                      : Wehrmacht 1938, 5 walzen");
            break;
        case M3_ARMY_1939:
            printf("Walzen                      : Wehrmacht 1939, 8 walzen");
            break;
        case M4_NAVAL_1941:
            printf("Walzen                      : Kriegsmarine Engima M4");
            break;
    }

    switch (operation.recipe.method)
    {
        case DEPTH_NONE:
            printf("Method                      : Keeping R1, R2, R3 fixed (Gillogly)\n");
            break;
        case DEPTH_R3:
            printf("Method                      : Keeping R1, R2 fixed\n");
            break;
        case DEPTH_R2_R3:
            printf("Method                      : Keeping R1 fixed\n");
            break;
    }
    printf("Evaluation for rotor        : ");
    switch (operation.recipe.evalWalzen)
    {
        case EVAL_IOC:
            printf("IoC\n");
            break;
        case EVAL_NGRAM:
            printf("NGRAMs, size %d\n", operation.recipe.ngramSize);
            break;
    }
    printf("Evaluation for stecker      : ");
    switch (operation.recipe.evalWalzen)
    {
        case EVAL_IOC:
            printf("IoC\n");
            break;
        case EVAL_NGRAM:
            printf("NGRAMs, size %d\n", operation.recipe.ngramSize);
            break;
    }

    printf("Max steckers                : %02d\n", operation.recipe.maxSteckers);
    printf("#####################################################################################\n");
}


/**************************************************************************************************\
* 
* Defines the decryption method
* recipe        Defines the operation of the IoC decoding
*   method        The method to use
*   evalWalzen    Evaluation to use for finding walzen, ringstellungen and grundstellungen  
*   evalSteckers  Evaluation to use for finding steckers
*   maxSteckers   The maximum number of Steckers to try
*   ngramSize     n in ngram (2 - bigrams, 3 - trigrams, etc), only used for evaluation=EVAL_NGRAM
*   ngrams        Ngram set to use ('DE', 'EN', 'GC')
* 
\**************************************************************************************************/
void setOperation(IocRecipe recipe)
{
    operation.recipe    = recipe;
    
    // Initialise the NGRAM scoring
    if (recipe.ngramSize>0 && strcmp(recipe.ngramSet, "")!=0)
    {
        printf("Preparing NGRAMs size %d, language %s\n", recipe.ngramSize, recipe.ngramSet);
        prepareNgramScore(recipe.ngramSize, recipe.ngramSet);
    }
}


/**************************************************************************************************\
* 
* Prepare work items for M3
* Currently it assumes 
* - an Enigma M3
* - only Walzen I, II, III, IV and V
* - Umkehr Walzen UKW B and UKW C
* 
\**************************************************************************************************/
void prepareWorkM3(char* cipher, int numOfThreads)
{
    int             i;
    int             length;
    LinkedList*     permutations;

    // Start with 5 Wehrmacht rotors in an M3 Enigma
    // TO DO: create permutations according to operation.recipe.rotorSet
    permutations        =createRotorPermutations(3, 5);
    length              =linkedListLength(permutations);
   
    setWalzePermutations(permutations);
    
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads*2;

    dispatcherClearWorkItems();
    i=0;
    while (i<numOfThreads)
    {
        iocWorkItems[i*2].cipher                =cipher;
        iocWorkItems[i*2].permutations          =permutations;
        iocWorkItems[i*2].startPermutation      =i*length/numOfThreads;
        iocWorkItems[i*2].endPermutation        =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2].R1                    =1;
        iocWorkItems[i*2].startR2               =1;
        if (operation.recipe.method==DEPTH_R2_R3)
        {
            iocWorkItems[i*2].endR2             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2].endR2             =1;
        }
        iocWorkItems[i*2].startR3               =1;
        if (operation.recipe.method==DEPTH_R2_R3 || operation.recipe.method==DEPTH_R3)
        {
            iocWorkItems[i*2].endR3             =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2].endR3             =1;
        }
        iocWorkItems[i*2].maxCipherChars        =MAX_TEXT;
        strncpy(iocWorkItems[i*2].ukw, "UKW B", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i*2]);

        
        iocWorkItems[i*2+1].cipher              =cipher;
        iocWorkItems[i*2+1].permutations        =permutations;
        iocWorkItems[i*2+1].startPermutation    =i*length/numOfThreads;
        iocWorkItems[i*2+1].endPermutation      =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2+1].R1                  =1;
        iocWorkItems[i*2+1].startR2             =1;
        if (operation.recipe.method==DEPTH_R2_R3)
        {
            iocWorkItems[i*2+1].endR2           =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2+1].endR2           =1;
        }
        iocWorkItems[i*2+1].startR3             =1;
        if (operation.recipe.method==DEPTH_R2_R3 || operation.recipe.method==DEPTH_R3)
        {
            iocWorkItems[i*2+1].endR3           =MAX_POSITIONS;
        }
        else
        {
            iocWorkItems[i*2+1].endR3           =1;
        }
        iocWorkItems[i*2+1].maxCipherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i*2+1].ukw, "UKW C", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i*2+1]);
        i++;
    }
}

/**************************************************************************************************\
* 
* Prepare work items for M4
* 
\**************************************************************************************************/
void prepareWorkM4(char* cipher, int numOfThreads)
{
    printf("Not supported... yet\n");
    exit(0);
}

/**************************************************************************************************\
* 
* Tries to decode a cipher only text. 
* 
\**************************************************************************************************/
void iocDecodeText(char* cipher, int numOfThreads)
{
    
    reportMethod();

    if (operation.recipe.enigmaType==ENIGMATYPE_M3)
    {
        prepareWorkM3(cipher, numOfThreads);
    }
    else if (operation.recipe.enigmaType==ENIGMATYPE_M3)
    {
        prepareWorkM4(cipher, numOfThreads);
    }
    else
    {
        printf("Error: non existing Enigma\n");
        exit(0);
    }
    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL);
}


