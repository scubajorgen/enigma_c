/**************************************************************************************************\
* 
* This file implements the method turing used to crack enigma encoded messages. It assumes a crib:
* a cypher text with corresponding plain text. Both are transformed into a 'menu' containing
* 'crib circles' or 'loops'. Crib circles are used validate hypotheses regarding the steckered
* letters: a the hypothesis proves right if the steckered value used as input generates 
* the same value as output when traversing the loop.
* Explained in detail on http://www.ellsbury.com/bombe1.htm
* 
\**************************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

#include "toolbox.h"
#include "enigma.h"

#define NUMBER_OF_THREADS   1
#define MAX_CIRCLE          12
#define MAX_CIRCLE_SIZE     12

// Crib circle definition.
typedef struct
{
    int     circleSize;                     // Number of steps in the circle
    int     advances[MAX_CIRCLE_SIZE];      // Advances of the rotors per step
    char    originalChar[MAX_CIRCLE_SIZE];  // Entry char
    char    foundChars[MAX_CIRCLE_SIZE];    // Steckered chars 
}
CribCircle;


// Set of crib circles or loops that start and end with the same letter
typedef struct
{
    int             numOfCircles;
    CribCircle      cribCircles[MAX_CIRCLE];
    char            startChar;
    char            foundChar;
} CribCirlceSet;




// TEST CASE 1
/*
char decoded[]="wettervorhersagebiskayaundonnerwetter";
char cypher[] ="gzsuxqbbefsahpolvbmckzqsvahqmbkepbbrp";

char testRingStellung []="01 13 01";
char testGrundStellung[]="02 02 03";
char testWaltzen[3][5]  ={"I", "II", "III"};
char testUkw[]          ="UKW B";
char testSteckers[]     ="bq cr di ej kw mt os px uz gh";


#define numOfSets 2
CribCirlceSet cribCircleSet[numOfSets]=
{
    {
        4, 
        {
            {5, {30, 8, 15, 1, 32}, "EBOGWE", ""},  // eb bo og gw we
            {4, {30, 34, 3, 11},    "EBTSE" , ""},  // eb bt ts se
            {4, {30, 35, 3, 11},    "EBTSE" , ""},  // eb bt ts se
            {2, {36, 9},            "ERE"   , ""}   // er re
        },
        'E'
    },
    {
        4, 
        {
            {2, {36, 9},            "RER"   , ""},  // er re
            {3, {6, 23, 12},        "RQAR"  , ""},  // rq qa ar
            {3, {12, 21, 31},       "RAKR"  , ""},  // ra ak kr
            {3, {37, 14, 12},       "RPAR"  , ""},  // rp pa ar        
        },
        'R'
    }
};
*/
// TEST CASE 2
char decoded[]="wettervorhersagebiskayaunddonnerwetter";
char cypher[] ="rpvpzildgrnopplofznrualxkhexldmqycdfaq";

char testGrundStellung[]="01 17 12";
char testRingStellung []="06 24 03";
char testWaltzen[3][5]  ={"I", "II", "III"};
char testUkw[]          ="UKW B";
char testSteckers[]     ="bq cr di ej kw mt os px uz gh";


#define numOfSets 3
CribCirlceSet cribCircleSet[numOfSets]=
{
    {
        5, 
        {
            {9, { 5, 18,  6,  9, 15, 7,  3,  4,  2} , "EZIRGLVTPE"   , ""},
            {5, {16, 12,  6, 18, 5}                 , "EBTSE"        , ""},
            {4, {11, 19, 13,  2}                    , "ENSPE"        , ""},
            {8, { 5, 18,  6,  9, 15, 23, 14, 2}     , "EZIRGLAPE"    , ""},
            {6, { 5, 18,  6, 20, 25, 11}            , "EZIRKNE"      , ""},
        },
        'E'
    },
    {
        4, 
        {
            {5, {14, 23,  7,  3, 4}                 , "PALVTP"      , ""},
            {8, { 2, 16, 12,  9, 15,  7,  3,  4 }   , "PEORGLVT"    , ""},
            {6, {14, 21, 24, 28, 16,  2 }           , "PAUXOEP"     , ""},
            {4, {13, 19, 11,  2}                    , "PSNEP"       , ""} 
        },
        'P'
    },
    {
        3, 
        {
            {4, {26, 10, 12, 8}                     , "DHROD"       , ""},
            {7, {26, 10,  6, 18,  5, 16, 8}         , "DHRIZEOD"    , ""},
            {3, { 8, 16, 27}                        , "DOED"        , ""} 
        },
        'D'
    },
    

};



// THE REAL THING

typedef struct
{
    int         start;
    int         end;
    pthread_t   threadId;
} ThreadParam;


int                 waltzenIndices[8]  ={0, 1, 2, 3, 4, 5, 6, 7};
char                waltzen[8][4]      ={"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};

char                waltzenString[80];

LinkedList*         permutations;
int                 threadsRunning;

pthread_mutex_t     mutex;

ThreadParam         params[4];

time_t              startTime;


/**************************************************************************************************\
* 
* Just a test to validate the crib cricle sets, with knowledge of the settings of the Enigma
* that generated the crib
* 
\**************************************************************************************************/
void turingProve()
{
    Enigma*     enigma;
    char*       decoded;
    char        temp[3];
    int         circle;
    int         step;
    int         set;
    CribCircle*     cribCircle;
    
    enigma=createEnigmaM3();
    
    placeWaltze         (enigma, 1, testWaltzen[0]);
    placeWaltze         (enigma, 2, testWaltzen[1]);
    placeWaltze         (enigma, 3, testWaltzen[2]);
    setRingStellungen   (enigma, testRingStellung);
    
    placeSteckers       (enigma, testSteckers);
    placeUmkehrWaltze   (enigma, testUkw);
    
    temp[1]='\0';

    decoded=NULL;
    
    set=0;
    while (set<numOfSets)
    {
        circle=0;
        while (circle<cribCircleSet[set].numOfCircles)
        {
            cribCircle=&cribCircleSet[set].cribCircles[circle];
            
            temp[0]=cribCircle->originalChar[0];
            printf("Circle %i: Input: %s, ", circle, temp);
     
            step=0;
            while (step<cribCircle->circleSize)
            {
                setText(enigma, temp);
                setGrundStellungen  (enigma, testGrundStellung);
                advances(enigma, cribCircle->advances[step]-1);

                encodeDecode        (enigma);
                decoded=toString    (enigma);
                temp[0]=decoded[0];
                step++;
            }
            printf("output: %s\n", decoded);
            circle++;
        }
        set++;
    }

   
    destroyEnigma(enigma);    
}




/**************************************************************************************************\
* 
* Simulates the Turing solution by 
* 
\**************************************************************************************************/
void turingFind(int permutationStart, int permutationEnd)
{
    Enigma*         enigma;
    int             g1, g2, g3, r1, r2, r3;
    long            counting;
    long            prevCounting;
    long            currentTime;
    long            diffTime;
    long            prevTime;
    long            convPerSec;


    int*            permutation;
    int             w;
    int             c;
    int             e;
    int             theChar;
    int             found;
    int             circle;
    int             set;
    CribCircle*     cribCircle;
    
    pthread_mutex_lock(&mutex);
    threadsRunning++;
    pthread_mutex_unlock(&mutex);    

    enigma=createEnigmaM3(); 

    clearSteckerBrett(enigma);
    placeUmkehrWaltze(enigma, "UKW B");

    counting        =0;
    prevTime        =startTime;
    prevCounting    =0;
    w               =permutationStart;

    while (w<permutationEnd)
    {
        pthread_mutex_lock(&mutex);
        permutation=(int*)elementAt(permutations, w);
        pthread_mutex_unlock(&mutex);        
        
        if (permutation!=NULL)
        {
            pthread_mutex_lock(&mutex);
            sprintf(waltzenString,"%s %s %s", waltzen[permutation[0]], waltzen[permutation[1]], waltzen[permutation[2]]);
            pthread_mutex_unlock(&mutex);        
            
            currentTime=(long)time(NULL)-(long)startTime;
            diffTime=currentTime-prevTime;
            if (diffTime>0)
            {
                convPerSec=(counting-prevCounting)/diffTime;
            }
            else
            {
                convPerSec=0;
            }
            prevTime        =currentTime;
            prevCounting    =counting;
            
            printf("Processing waltzen permutation (%3d, %3d, %3d) %3d:%15s @ systemtime %ld seconds, %ld conv per sec \n", 
                   permutation[0], permutation[1], permutation[2], w, waltzenString, currentTime, convPerSec);
            fflush(stdout);
           
            
            placeWaltze(enigma, 1, waltzen[permutation[0]]);
            placeWaltze(enigma, 2, waltzen[permutation[1]]);
            placeWaltze(enigma, 3, waltzen[permutation[2]]);
            
            for (g1=1; g1<=26; g1++)
            {
                for (g2=1; g2<=26; g2++)
                {
                    for (g3=1; g3<=26; g3++)
                    {
                        // The RingStellung of the 1st ring has no function
                        r1=1; r2=1; r3=1;
            
                        for (r2=1; r2<=26; r2++)
                        {
                            for (r3=1; r3<=26; r3++)
                            {

                                setRingStellung(enigma, 1, r1);
                                setRingStellung(enigma, 2, r2);
                                setRingStellung(enigma, 3, r3);

                                // We check if for each set of crib circles 
                                // there is a character (hypothesis) that fullfills 
                                // all circles in the set. 
                                // Parse the sets
                                found=1;
                                set=0;
                                while (set<numOfSets && found)
                                {
                                    // Choose a character c and check whether
                                    // the output of each circle in th set results 
                                    // in the same character c (hypothesis)
                                    c       =0;
                                    found   =0;
                                    while (c<MAX_POSITIONS && !found)
                                    {
                                        // Try the circles. If one circle fails
                                        // hypothesis is rejected, proceed to next char
                                        circle  =0;
                                        found   =1;
                                        while (circle<cribCircleSet[set].numOfCircles && found)
                                        {
                                            cribCircle=&cribCircleSet[set].cribCircles[circle];
                                            theChar =c;
                                            e       =0;
                                            while (e<cribCircle->circleSize)
                                            {
                                                cribCircle->foundChars[e]=theChar+'A';
                                                setGrundStellung(enigma, 1, g1);
                                                setGrundStellung(enigma, 2, g2);
                                                setGrundStellung(enigma, 3, g3);
                                                
                                                advances(enigma, cribCircle->advances[e]);
                                                theChar=encodeCharacter(enigma, theChar);
                                                e++;
                                            }
                                            cribCircle->foundChars[e]=theChar+'A';
                                            cribCircle->foundChars[e+1]='\0';
                                            if (theChar!=c)
                                            {
                                                found=0;
                                            }
                                            
                                            circle++;
                                        }
                                        // Store the character c that works
                                        // for all circles in the set
                                        if (found)
                                        {
                                            cribCircleSet[set].foundChar=c+'A';
                                        }
                                        c++;
                                    }
                                    set++;
                                }
                                if (found)
                                {
                                    printf("Solution found: R %d %d %d, G %d %d %d\n",
                                           r1, r2, r3, g1, g2, g3);
                                    set=0;
                                    while (set<numOfSets)
                                    {
                                        circle=0;
                                        while (circle<cribCircleSet[set].numOfCircles)
                                        {
                                            cribCircle=&cribCircleSet[set].cribCircles[circle];
                                            printf("Circle %d: %s - %c - %s \n", 
                                                   circle, 
                                                   cribCircle->originalChar, 
                                                   cribCircleSet[set].foundChar,
                                                   cribCircle->foundChars);
                                            circle++;
                                        }
                                        set++;
                                    }
                                    fflush(stdout);
                                }

                                counting++;
                                
                            }
                        }

                    }

                }
            }
             
            free(permutation);
        }
        else
        {
            printf("Error!! Permutation is NULL");
            fflush(stdout);
        }
        w++;
    }
    
    destroyEnigma(enigma);
    
    pthread_mutex_lock(&mutex);
    threadsRunning--;
    if (threadsRunning==0)
    {
        destroyLinkedList(permutations);
    }
    pthread_mutex_unlock(&mutex);    
    
}   

/**************************************************************************************************\
* 
* Thread function
* 
\**************************************************************************************************/

void *threadFunction(void *vargp) 
{ 
    ThreadParam* params;
    // Store the value argument passed to this thread 


    params=(ThreadParam*)vargp;

    printf("Starting thread %ld processing pemutations %3d to %3d\n", (long)params->threadId, params->start, params->end);
  
    turingFind(params->start, params->end);
    
    return NULL;
} 
   
/**************************************************************************************************\
* 
* Brute force crack using threads
* 
\**************************************************************************************************/

void turingCrack()
{
    int         i; 
    int         numberOfPermutations;


    permutations        =createLinkedList();
    
    // Use first 5 Waltzen
    permute(permutations, waltzenIndices, 5, 3, 0);
    
    numberOfPermutations=linkedListLength(permutations);
    printf("Waltzen permutations %d\n", numberOfPermutations);

    threadsRunning=0;
    startTime=time(NULL);  
    // Let us create three threads 
    for (i = 0; i < NUMBER_OF_THREADS; i++) 
    {
        
        params[i].start =i*(numberOfPermutations/NUMBER_OF_THREADS);
        params[i].end   =(i+1)*(numberOfPermutations/NUMBER_OF_THREADS);

        pthread_create(&(params[i].threadId), NULL, threadFunction, (void *)(params+i)); 
    }
    
    if (i*(numberOfPermutations/NUMBER_OF_THREADS)!=numberOfPermutations)
    {
        printf("Oops! Permutations not divisible by number of threads\n");
    }
    
    pthread_exit(NULL); 
  
}

