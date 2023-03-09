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

#define MAX_THREADS     16
#define MAX_WORKITEMS   32
#define MAX_UKW_STRING  20


// This defines a work item for the thread
typedef struct
{
    int     permutationStart;       // Start permutation in the permutations array
    int     permutationEnd;         // End permutation (not included)
    char    ukw[MAX_UKW_STRING];    // UKW to use
} ThreadWork;

// This defines the parameters passed to the thread
typedef struct
{
    int         start;              // Start work item in work array
    int         end;                // End work item (not included)
    pthread_t   threadId;           // ID of the thread
} ThreadParam;


int                 threadsRunning;

char                waltzenString[80];

// Thread stuff
ThreadWork          work[MAX_WORKITEMS];
pthread_mutex_t     mutex;
ThreadParam         params[MAX_THREADS];

time_t              startTime;

// Defines the links between letters
LinkedLetters       menu[MAX_POSITIONS];

// Defines the crib circle loops
CribCircleSet       cribCircleSet[MAX_POSITIONS];

LinkedList* permutations;

// Consistency parameters
int                 mallocs=0;
int                 stepMax=0;

char*               turingBombeCypher;


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
void destroyCircle(CribCircle* circle)
{
    free(circle);
    mallocs--;
}

/**************************************************************************************************\
* 
* Debugging: show letter links
* 
\**************************************************************************************************/
void dumpMenu()
{
    int c;
    int l;
    c=0;
    while (c<MAX_POSITIONS)
    {
        printf("Link %c: %d - ", menu[c].letter, menu[c].numOfLinks);
        l=0;
        while (l<menu[c].numOfLinks)
        {
            printf("%c (%d) ", menu[c].links[l].letter, menu[c].links[l].position);
            l++;
        }
        printf("\n");
        c++;
    }
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void dumpCircle(CribCircle* circle)
{
    int c;
    
    c=0;
    while (c<circle->circleSize)
    {
        printf("%c (%d) ", circle->orgChars[c], circle->advances[c]);
        c++;
    }
    printf("%c\n", circle->orgChars[0]);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void dumpSets()
{
    int                 s;
    int                 c;
    CribCircleSet*      theSet;
    CribCircle*         theCircle;    

    s=0;
    while (s<MAX_POSITIONS)
    {
        theSet=&cribCircleSet[s];
        printf("SET %c - %d\n", theSet->startChar, theSet->numOfCircles);
        c=0;
        while (c<theSet->numOfCircles)
        {
            theCircle=&theSet->cribCircles[c];
            dumpCircle(theCircle);
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
        menu[pos].letter       ='A'+pos;
        menu[pos].numOfLinks   =0;
        pos++;
    }
    
    pos=0;
    while (pos<strlen(crib))
    {
        index1=text[pos]-'A';
        index2=crib[pos]-'A';
        
        menu[index1].links[menu[index1].numOfLinks].letter      =crib[pos];
        menu[index1].links[menu[index1].numOfLinks].position    =pos+1;
        menu[index1].numOfLinks++;
        menu[index2].links[menu[index2].numOfLinks].letter      =text[pos];
        menu[index2].links[menu[index2].numOfLinks].position    =pos+1;
        menu[index2].numOfLinks++;
        pos++;
    }
}


/**************************************************************************************************\
* 
* Recursive function that find all loops starting from one character.
* For longer cribs this method will result in stack overflow.
* TO DO: remove double loops, prevent endless loops
* 
\**************************************************************************************************/
void followLoop(char startChar, LetterLink* currentLink, CribCircle* circle, int step)
{
    int             l;
    int             c;
    int             found;
    int             circleSize;
    int             nextLetter;
    LinkedLetters*  nextLinks;
    CribCircle*     nextCircle;
    CribCircleSet*  theSet;
    CribCircle*     theCircle;
    
    // Recursion depth
    if (step>stepMax)
    {
        stepMax=step;
    }
    
    if (step>MAX_POSITIONS)
    {
        // Fall back to prevent endless loops
        // Should never happen, unfortunatelly it does
        printf("Endless loop\n");
    }
    else if (cribCircleSet[(int)startChar-'A'].numOfCircles>=MAX_CIRCLES)
    {
        printf("Maximum number of crib circles %d reached for %c\n", MAX_CIRCLES, startChar);
    }
    else
    {
        // On the 1st call the currentLink and circle are set to NULL
        if (currentLink==NULL || circle==NULL)
        {
            // Special case: currentLink==NULL: use the start character 
            // as starting point and add all its links
            nextLetter  =startChar;
            nextLinks   =&menu[nextLetter-'A'];
            l=0;
            while (l<nextLinks->numOfLinks)
            {
                // Create the first circle
                nextCircle                      =createCircle();
                circleSize                      =nextCircle->circleSize; // =0
                // Add next link to circle
                nextCircle->advances[circleSize]=nextLinks->links[l].position;
                nextCircle->orgChars[circleSize]=nextLetter;
                nextCircle->circleSize++;
                // Iterate
                followLoop(startChar, &nextLinks->links[l], nextCircle, step+1);
                destroyCircle(nextCircle);
                l++;
            }
        }
        else
        {
            nextLetter  =currentLink->letter;
            if (nextLetter==startChar)
            {
                // Yes! we found a loop
                circle->orgChars[circle->circleSize]=startChar;         // finish character string
                circle->orgChars[circle->circleSize+1]='\0';
                theSet      =&cribCircleSet[(int)startChar-'A'];        // Add circle to set
                theCircle   =&theSet->cribCircles[theSet->numOfCircles];
                *theCircle  =*circle; // Copy
                theSet->numOfCircles++;
            }
            else
            {
                nextLinks   =&menu[nextLetter-'A'];
                l           =0;
                while (l<nextLinks->numOfLinks)
                {
                    // Make sure the next position not already
                    // occurs in the circle: only allowed once
                    c       =0;
                    found   =0;
                    while (c<circle->circleSize && !found)
                    {
                        // This one allows only simple loops, like a - b - c - a 
                        // Limits the number of loops; advantageous for large crib sizes
//                        if (circle->advances[c]==nextLinks->links[l].position)

                        // This one allows also complicated loops, like a - b - c - b - a
                        // increasing the number of loops exponentially; advantageous for 
                        // not to large crib sizes (up to ~25 chars)
                        if (circle->orgChars[c]==nextLetter)
                        {
                            found=1;
                        }
                        c++;
                    }
                    // Do not loop back :-)
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
                        followLoop(startChar, &nextLinks->links[l], nextCircle, step+1);
                        destroyCircle(nextCircle);

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
        // create the menu (the letter links)
        turingGenerateLetterLinks(text, crib);
       
        stepMax=0;
        c=0;
        while (c<MAX_POSITIONS)
        {
            // reset the circle set for this letter
            cribCircleSet[c].numOfCircles   =0;
            cribCircleSet[c].startChar      ='A'+c;
            
            // recursivel generate the crib loops
            followLoop('A'+c, NULL, NULL, 0);
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
* Helper: for a given setting of the engima, test all loop hypotheses, i.e. check if for each 
* subset of loops there is a steckered character that when following the loops results in the 
* same output char; the subset is characterized that they start and end with the same 
* unsteckered character 
*
* TO DO: it is possible that more than one character fullfills a set of loops (fc>1); 
* in that case only the last found value is stored. In that case the method
* turingValidateTheSteckeredValues() may reject the solution and the right solution may not
* be seen. 
* Work-around: sufficiently long crib to generate enough loops in the set reducing the chance
* on multiple chars fulfilling the set...
* 
\**************************************************************************************************/
int turingValidateHypotheses(Enigma* enigma, int g1, int g2, int g3, SteckeredChars* chars)
{
    int             theFound;
    int             found;
    int             c;
    int             fc;
    int             e;
    int             set;
    int             circle;
    CribCircle*     cribCircle;
    int             theChar;    
    CribCircleSet*  theSet;
    
    // We check if for each set of crib circles 
    // there is a character (hypothesis) that fullfills 
    // all circles in the set. 
    // Parse the sets
    found=1;
    theFound=1;
    set=0;
    while (set<MAX_POSITIONS && theFound)
    {
        theSet=&cribCircleSet[set];
        
        if (theSet->numOfCircles>2)
        {
            // Choose a character c and check whether
            // the output of each circle in th set results 
            // in the same character c (hypothesis)
            c       =0;
            fc      =0;
            found   =0;
            theFound=0;
            while (c<MAX_POSITIONS/* && !found*/)
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
                        setGrundStellung(enigma, 1, g1);
                        setGrundStellung(enigma, 2, g2);
                        setGrundStellung(enigma, 3, g3);
                        
                        advances(enigma, cribCircle->advances[e]);
                        theChar=encodeCharacter(enigma, theChar);
                        e++;
                    }
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
                    chars[set].foundChar=c+'A';
                    fc++;
                    if (fc>1)
                    {
                        printf("Found multiple %d - %c->%c circles %d\n", fc, theSet->startChar, c+'A', theSet->numOfCircles);
                    }
                    theFound=1;
                }
                c++;
            }
        }
        set++;
    }
    
    return theFound;
}


/**************************************************************************************************\
* 
* Helper: For a found set of steckered values, check if the set is consistent
* 
\**************************************************************************************************/
int turingValidateTheSteckeredValues(SteckeredChars* chars)
{
    int found;
    int set;
    int set2;
    
    found=1;
    // Now we found a potential engima settings
    // Check for inconsistencies in the steckered letters.
    // This means: steckered letters may not appear more than once
    // unless mutual
    set=0;
    while (set<MAX_POSITIONS && found)
    {
        set2=set+1;
        while (set2<MAX_POSITIONS && found)
        {
            if ((chars[set].foundChar==chars[set2].startChar) &&
                (chars[set2].foundChar!=chars[set].startChar) &&
                (chars[set2].foundChar!='?'))                                             
                 
            {
                found=0;
            }
            if ((chars[set].foundChar!='?') &&
                (chars[set].startChar==chars[set2].foundChar) &&
                (chars[set].foundChar!=chars[set2].startChar))
            {
                found=0;
            }
            if ((chars[set].foundChar!='?') &&
                (chars[set2].foundChar!='?') &&
                (chars[set].foundChar==chars[set2].foundChar))
            {
                found=0;
            }
            set2++;
        }
        set++;
    }
    return found;
}

/**************************************************************************************************\
* 
* Helper: Print a found solution
* 
\**************************************************************************************************/
void turingPrintSolution(EnigmaSettings* settings)
{
    printf("Solution found: %s - %s %s %s R %d %d %d, G %d %d %d, %s\n",
           settings->ukw,
           settings->rotors[0], settings->rotors[1], settings->rotors[2],
           settings->ringStellungen[0], settings->ringStellungen[1], settings->ringStellungen[2],
           settings->grundStellungen[0], settings->grundStellungen[1], settings->grundStellungen[2],
           settings->steckers);


    fflush(stdout);
    
    dumpDecoded(settings);
}

/**************************************************************************************************\
* 
* Helper: Create array of steckered chars (per thread)
* 
\**************************************************************************************************/
SteckeredChars* createSteckeredChars()
{
    int                 c;
    SteckeredChars*     chars;
    
    chars=malloc(sizeof(SteckeredChars)*MAX_POSITIONS);
    c=0;
    while (c<MAX_POSITIONS)
    {
        chars[c].startChar='A'+c;
        chars[c].foundChar='?';
        c++;
    }
    return chars;
    
}

/**************************************************************************************************\
* 
* Helper: Create array of steckered chars (per thread)
* 
\**************************************************************************************************/
void convertSteckeredCharsToString(SteckeredChars* chars, char* string)
{
    int c;
    int c2;
    int found;
    int s;
    
    c=0;
    s=0;
    while (c<MAX_POSITIONS)
    {
        if (chars[c].foundChar!=chars[c].startChar && 
            chars[c].foundChar!='?')
        {
            found=0;
            c2=0;
            while (c2<s*3 && !found)
            {
                if (string[c2]==chars[c].foundChar)
                {
                    found=1;
                }
                c2++;
            }
            if (!found)
            {
                string[s*3]  =chars[c].startChar; 
                string[s*3+1]=chars[c].foundChar; 
                string[s*3+2]=' '; 
                s++;
            }
        }
        c++;
    }
    string[s*3-1]='\0';
    
}


/**************************************************************************************************\
* 
* Simulates the Turing solution: finds the rotors, RingStellungen and GrundStellungen based
* on a 'crib'
* 
\**************************************************************************************************/
void turingFind(int permutationStart, int permutationEnd, char* ukw)
{

    long            counting;
    long            prevCounting;
    long            currentTime;
    long            diffTime;
    long            prevTime;
    long            convPerSec;

    Enigma*         enigma;
    char*           w1;
    char*           w2;
    char*           w3;
    int             g1, g2, g3, r1, r2, r3;
    SteckeredChars* steckeredChars; 
    int*            permutation;
    int             w;
    int             found;
    EnigmaSettings  settings; // TO DO: put on heap iso. stack

    
    
    printf("Processing %d-%d, %s\n", permutationStart, permutationEnd, ukw);

    // Create a set of steckered values (for this thread)
    steckeredChars=createSteckeredChars();

    enigma=createEnigmaM3(); 
    clearSteckerBrett(enigma);
    placeUmkehrWaltze(enigma, ukw);

    counting        =0;
    prevTime        =startTime;
    prevCounting    =0;
    w               =permutationStart;

    // Parse the rotor permutations
    while (w<permutationEnd)
    {
        permutation=(int*)elementAt(permutations, w);
        
        if (permutation!=NULL)
        {
            w1=waltzen[permutation[0]];
            w2=waltzen[permutation[1]];
            w3=waltzen[permutation[2]];
            sprintf(waltzenString,"%s - %s %s %s", ukw, w1, w2, w3);
            
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
            
            printf("Processing waltzen permutation (%3d, %3d, %3d) %3d:%15s @ systemtime %ld seconds, %ld settings per sec \n", 
                   permutation[0], permutation[1], permutation[2], w, waltzenString, currentTime, convPerSec);
            fflush(stdout);
           
            // Set the rotor
            placeWaltze(enigma, 1, w1);
            placeWaltze(enigma, 2, w2);
            placeWaltze(enigma, 3, w3);
            
            // GrundStellungen
            for (g1=1; g1<=26; g1++)
            {
                for (g2=1; g2<=26; g2++)
                {
                    for (g3=1; g3<=26; g3++)
                    {
                        // The RingStellung of the 1st ring has no function
                        // In theory the 2nd ring should be taken into account. However
                        // For short cribs most of the times taking one value is sufficient
                        // It must be taken into account when the if the notch position of
                        // the second rotor is reached after a few rotations...
                        r1=1; r2=3;
/*            
                        for (r2=1; r2<=26; r2++)
                        {
*/
                            for (r3=1; r3<=26; r3++)
                            {

                                setRingStellung(enigma, 1, r1);
                                setRingStellung(enigma, 2, r2);
                                setRingStellung(enigma, 3, r3);

                                found=turingValidateHypotheses(enigma, g1, g2, g3, steckeredChars);
                                
                                if (found)
                                {
                                    found=turingValidateTheSteckeredValues(steckeredChars);
                                }                                    
                                
                                if (found)
                                {
                                    settings.numberOfRotors     =3;
                                    strncpy(settings.cypher, turingBombeCypher, MAX_TEXT-1);
                                    strncpy(settings.rotors[0], w1, MAX_ROTOR_NAME);
                                    strncpy(settings.rotors[1], w2, MAX_ROTOR_NAME);
                                    strncpy(settings.rotors[2], w3, MAX_ROTOR_NAME);
                                    settings.ringStellungen[0]  =r1;
                                    settings.ringStellungen[1]  =r2;
                                    settings.ringStellungen[2]  =r3;
                                    settings.grundStellungen[0] =g1;
                                    settings.grundStellungen[1] =g2;
                                    settings.grundStellungen[2] =g3;
                                    strncpy(settings.ukw, ukw, MAX_ROTOR_NAME-1);
                                    
                                    convertSteckeredCharsToString(steckeredChars, settings.steckers);
                                    turingPrintSolution(&settings);
                                            
                                }

                                counting++;
                                
                            }
/*
                        }
*/
                    }

                }
            }
             
        }
        else
        {
            printf("Error!! Permutation is NULL");
            fflush(stdout);
        }
        w++;
    }
    
    destroyEnigma(enigma);
    
    free(steckeredChars);
}   

/**************************************************************************************************\
* 
* Thread function
* 
\**************************************************************************************************/
void *threadFunction(void *vargp) 
{ 
    int     workStart;
    int     workEnd;
    int     w;
    int     p;
    long    threadId;
    
    ThreadParam* params;

    // Increase the number of threads running
    pthread_mutex_lock(&mutex);
    threadsRunning++;
    pthread_mutex_unlock(&mutex);  

    // Store the value argument passed to this thread 
    params=(ThreadParam*)vargp;

    threadId    =(long)params->threadId;
    workStart   =params->start;
    workEnd     =params->end;

    printf("Starting thread %ld processing work items %2d to %2d\n", threadId, workStart, workEnd);
  
    w=workStart;
    while (w<workEnd)
    {
        printf("Thread %ld starting work item %d\n", threadId, w);
        turingFind(work[w].permutationStart, work[w].permutationEnd, work[w].ukw);
        w++;
    }
    
    // Decrease the number of running threads
    // If this is the last one, clean up the linked list
    pthread_mutex_lock(&mutex);
    threadsRunning--;
    if (threadsRunning==0)
    {
        p=0;
        while (p<linkedListLength(permutations))
        {
            free(elementAt(permutations, p));
            p++;
        }            
        destroyLinkedList(permutations);
    }
    pthread_mutex_unlock(&mutex); 



    return NULL;
} 
   
/**************************************************************************************************\
* 
* Implements the Turing Bombe
* cypher        : cypher string, upper case!!
* crib          : crib, upper case. Length must be shorter than cypher and not to long 
*                 to prevent stack overflow
* numOfThreads  : Use multiple threads to use multi processor cores. Rotor combinations are 
*                 split up amongst the threads. Number of permutations should be divisable by 
*                 this number
* 
\**************************************************************************************************/
void turingBombe(char* cypher, char* crib, int numOfThreads)
{
    int         i; 
    int         w;
    int         numberOfPermutations;
    int         workItems;

    turingBombeCypher=cypher;

    turingFindLoops(cypher, crib);

    // Choose from the 5 wehrmacht waltzen   
    permutations=createRotorPermutations(3, 5);
    
    numberOfPermutations=linkedListLength(permutations);
    printf("Waltzen permutations %d\n", numberOfPermutations);

    workItems=numOfThreads*2;
    
    w=0;
    while (w<workItems)
    {
        work[w  ].permutationStart  =(w/2)*numberOfPermutations/numOfThreads;
        work[w  ].permutationEnd    =(w/2+1)*numberOfPermutations/numOfThreads;
        strncpy(work[w  ].ukw, "UKW B", MAX_UKW_STRING);
        
        work[w+1].permutationStart  =(w/2)*numberOfPermutations/numOfThreads;
        work[w+1].permutationEnd    =(w/2+1)*numberOfPermutations/numOfThreads;
        strncpy(work[w+1].ukw, "UKW C", MAX_UKW_STRING);
        w+=2;
    }

    threadsRunning=0;
    startTime=time(NULL);  
    // Let us create three threads 
    for (i = 0; i < numOfThreads; i++) 
    {
        
        params[i].start =i*2;
        params[i].end   =(i+1)*2;

        pthread_create(&(params[i].threadId), NULL, threadFunction, (void *)(params+i)); 
    }
    
    if (i*(numberOfPermutations/numOfThreads)!=numberOfPermutations)
    {
        printf("Oops! Permutations not divisible by number of threads\n");
    }
    
    pthread_exit(NULL); 
}

