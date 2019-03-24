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
#include <string.h>     

#include "toolbox.h"
#include "enigma.h"
#include "turing.h"

#define NUMBER_OF_THREADS   1



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
CribCircleSet cribCircleSet[numOfSets]=
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
//char crib[]   ="WETTERVORHERSAGEBISKAYAUNDDONNERWETTER";
char turingCrib[]   ="WETTERVORHERSAGEBISKAYA";
char turingCypher[] ="RPVPZILDGRNOPPLOFZNRUALXKHEXLDMQYCDFAQ";

char testGrundStellung[]="01 17 12";
char testRingStellung []="06 24 03";
char testWaltzen[3][5]  ={"I", "II", "III"};
char testUkw[]          ="UKW B";
char testSteckers[]     ="bq cr di ej kw mt os px uz gh";


#define numOfSets 3
CribCircleSet cribCircleSet[MAX_POSITIONS];


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

LinkedLetters       links[MAX_POSITIONS];

int                 mallocs=0;

CribCircleSet*      theSet;
CribCircle*         theCircle;


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
CribCircle* createCircle()
{
    CribCircle*  circle;
    circle              =malloc(sizeof(CribCircle));
    circle->circleSize  =0;
    
    mallocs++;
    
    return circle;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void freeCircle(CribCircle* circle)
{
    free(circle);
    mallocs--;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

void dumpSets()
{
    int s;
    int c;
    int l;
    s=0;
    while (s<MAX_POSITIONS)
    {
        theSet=&cribCircleSet[s];
        printf("SET %c - %d\n", theSet->startChar, theSet->numOfCircles);
        c=0;
        while (c<theSet->numOfCircles)
        {
            theCircle=&theSet->cribCircles[c];
            if (theCircle->circleSize>0)
            {
                l=0;
                while (l<theCircle->circleSize)
                {
                    printf("%c (%d) ", theCircle->orgChars[l], theCircle->advances[l]);
                    l++;
                }
                printf("...\n");
            }
            c++;
        }
        s++;
    }
}

/**************************************************************************************************\
* 
* Makes an inventory of the links between any two letters in the text and crib
* 
\**************************************************************************************************/
void turingGenerateLetterLinks(char* text, char* crib)
{
    int pos;
    int index1;
    int index2;
    
    // Reset links
    pos=0;
    while (pos<MAX_POSITIONS)
    {
        links[pos].letter       ='A'+pos;
        links[pos].numOfLinks   =0;
        pos++;
    }
    
    pos=0;
    while (pos<strlen(crib))
    {
        index1=text[pos]-'A';
        index2=crib[pos]-'A';
        
        links[index1].links[links[index1].numOfLinks].letter    =crib[pos];
        links[index1].links[links[index1].numOfLinks].position  =pos+1;
        links[index1].numOfLinks++;
        links[index2].links[links[index2].numOfLinks].letter    =text[pos];
        links[index2].links[links[index2].numOfLinks].position  =pos+1;
        links[index2].numOfLinks++;
        pos++;
    }
}


/**************************************************************************************************\
* 
* Recursive function that find all loops starting from one character.
* For longer cribs this method will result in stack overflow.
* TO DO: remove double loops
* 
\**************************************************************************************************/


void followLoop(char startChar, LetterLink* currentLink, int step, CribCircle* circle)
{
    int             l;
    int             c;
    int             found;
    int             circleSize;
    int             nextLetter;
    LinkedLetters*  nextLinks;
    CribCircle*     nextCircle;
    
    if (step==MAX_POSITIONS)
    {
        // Fall back to prevent endless loops
        // End 1: no link found
        printf("Endless loop\n");
    }
    else
    {
        if (currentLink==NULL || circle==NULL)
        {
            // Special case: currentLink==NULL: first character
            nextLetter  =startChar;
            nextLinks   =&links[nextLetter-'A'];
            l=0;
            while (l<nextLinks->numOfLinks)
            {
                // Create the first circle
                nextCircle                      =createCircle();
                circleSize                      =nextCircle->circleSize;
                // Add next link to circle
                nextCircle->advances[circleSize]=nextLinks->links[l].position;
                nextCircle->orgChars[circleSize]=nextLetter;
                nextCircle->circleSize++;
                // Iterate
                followLoop(startChar, &nextLinks->links[l], step+1, nextCircle);
                freeCircle(nextCircle);
                l++;
            }
        }
        else
        {
            nextLetter  =currentLink->letter;
            if (nextLetter==startChar)
            {
                circle->orgChars[circle->circleSize]=startChar;         // finish character string
                circle->orgChars[circle->circleSize+1]='\0';
                theSet      =&cribCircleSet[(int)startChar-'A'];        // Add circle to set
                theCircle   =&theSet->cribCircles[theSet->numOfCircles];
                *theCircle  =*circle; // Copy
                theSet->numOfCircles++;
            }
            else
            {
                nextLinks   =&links[nextLetter-'A'];
                l           =0;
                while (l<nextLinks->numOfLinks)
                {
                    // Make sure the next position not already
                    // occurs in the circle: only allowed once
                    c       =0;
                    found   =0;
                    while (c<circle->circleSize && !found)
                    {
                        if (circle->advances[c]==nextLinks->links[l].position)
                        {
                            found=1;
                        }
                        c++;
                    }
                    if (nextLinks->links[l].position!=currentLink->position && !found)
                    {
                        // Copy circle
                        nextCircle                      =createCircle();
                        *nextCircle                     =*circle;
                        // Add next link to circle
                        circleSize                      =nextCircle->circleSize;
                        nextCircle->advances[circleSize]=nextLinks->links[l].position;
                        nextCircle->orgChars[circleSize]=nextLetter;
                        nextCircle->circleSize++;
                        followLoop(startChar, &nextLinks->links[l], step+1, nextCircle);
                        freeCircle(nextCircle);

                    }

                    l++;
                }
            }
        }
    }

//    printf("Mallocs %d\n", mallocs);
}


/**************************************************************************************************\
* 
* This method transfers a text and a crib into a list of crib circles; assumes the crib length
* is smaller than or equal to text length
* 
\**************************************************************************************************/
void turingFindLoops(char* text, char* crib)
{
    int c;
    
    if (strlen(text)>=strlen(crib))
    {
        turingGenerateLetterLinks(text, crib);
        
        c=0;
        while (c<MAX_POSITIONS)
        {
            cribCircleSet[c].numOfCircles   =0;
            cribCircleSet[c].startChar      ='A'+c;
            followLoop('A'+c, NULL, 0, NULL);
            c++;
        }
    }
    else
    {
        printf("Invalid crib length\n");
    }
}





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
    
    
    turingFindLoops(turingCypher, turingCrib);
    
    
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
    while (set<MAX_POSITIONS)
    {
        circle=0;
        while (circle<cribCircleSet[set].numOfCircles)
        {
            cribCircle=&cribCircleSet[set].cribCircles[circle];
            
            temp[0]=cribCircle->orgChars[0];
            printf("Letter %c, Circle %02i: Input: %s, ", cribCircleSet[set].startChar, circle, temp);
     
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
* Simulates the Turing solution: finds the rotors, RingStellungen and GrundStellungen based
* on a 'crib'
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

    // Parse the rotor permutations
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
           
            // Set the rotor
            placeWaltze(enigma, 1, waltzen[permutation[0]]);
            placeWaltze(enigma, 2, waltzen[permutation[1]]);
            placeWaltze(enigma, 3, waltzen[permutation[2]]);
            
            // GrundStellungen
            for (g1=1; g1<=26; g1++)
            {
                for (g2=1; g2<=26; g2++)
                {
                    for (g3=1; g3<=26; g3++)
                    {
                        // The RingStellung of the 1st ring has no function
                        r1=1;
            
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
                                while (set<MAX_POSITIONS && found)
                                {
                                    theSet=&cribCircleSet[set];
                                    
                                    if (theSet->numOfCircles>0)
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
                                            theSet=&cribCircleSet[set];
                                            while (circle<theSet->numOfCircles && found)
                                            {
                                                cribCircle=&theSet->cribCircles[circle];
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
                                                theSet->foundChar=c+'A';
                                            }
                                            c++;
                                        }
                                    }
                                    set++;
                                }
                                if (found)
                                {
                                    printf("Solution found: R %d %d %d, G %d %d %d\n",
                                           r1, r2, r3, g1, g2, g3);
                                    
                                    set=0;
                                    while (set<MAX_POSITIONS)
                                    {
                                        if (cribCircleSet[set].numOfCircles>0)
                                        {
                                            circle=0;
                                            while (circle</*cribCircleSet[set].numOfCircles*/1)
                                            {
                                                cribCircle=&cribCircleSet[set].cribCircles[circle];
                                                printf("Circle %d: %s - %c - %s \n", 
                                                       circle, 
                                                       cribCircle->orgChars, 
                                                       cribCircleSet[set].foundChar,
                                                       cribCircle->foundChars);
                                                circle++;
                                            }
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
* Implements the Turing Bombe
* cypher        : cypher string, upper case!!
* crib          : crib, upper case. Length must be shorter than cypher and not to long 
*                 to prevent stack overflow
* numOfThreads  : Use multiple threads to use multi processor cores. Rotor combinations are 
*                 split up amongst the threads
* 
\**************************************************************************************************/

void turingBombe(char* cypher, char* crib, int numOfThreads)
{
    int         i; 
    int         numberOfPermutations;

    turingFindLoops(cypher, crib);

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


/**************************************************************************************************\
* 
* An example
* 
\**************************************************************************************************/
void turingExample()
{
    turingBombe(turingCypher, turingCrib, 1);
}
