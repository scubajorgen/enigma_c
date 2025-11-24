/**************************************************************************************************\
* 
* This file implements the method turing used to crack enigma encoded messages. It assumes a crib:
* a cipher text with corresponding plain text. Both are transformed into a 'menu' containing
* 'crib circles' or 'loops'. Crib circles are used validate hypotheses regarding the steckered
* letters: a the hypothesis proves right if the steckered value used as input generates 
* the same value as output when traversing the loop.
* Explained in detail on http://www.ellsbury.com/bombe1.htm
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <sys/time.h>

#include "log.h"
#include "enigma.h"
#include "coincidence.h"
#include "toolbox.h"
#include "enigma.h"
#include "turing.h"
#include "workDispatcher.h"

#define MAX_WORKITEMS   32


// This defines a work item for the thread
typedef struct
{
    int     permutationStart;       // Start permutation in the permutations array
    int     permutationEnd;         // End permutation (not included)
} ThreadWork;

char                walzenString[80];

// Thread stuff
ThreadWork          work[MAX_WORKITEMS];

time_t              startTime;

// Defines the links between letters
LinkedLetters       menu[MAX_POSITIONS];

// Defines the crib circle loops
CribCircleSet       cribCircleSet[MAX_POSITIONS];

LinkedList*         tPermutations;

// Consistency parameters
int                 mallocs=0;
int                 stepMax=0;

// The solution
TuringRecipe        theRecipe;
LinkedList*         theResults;



/**************************************************************************************************\
* 
* Creates a CribCircle instance
* 
\**************************************************************************************************/
CribCircle* createCircle()
{
    CribCircle*  circle =malloc(sizeof(CribCircle));
    circle->circleSize  =0;
    mallocs++;
    return circle;
}

/**************************************************************************************************\
* 
* Destroys a CribCricle instance
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
    for (int c=0; c<MAX_POSITIONS; c++)
    {
        printf("Link %c: %d - ", menu[c].letter, menu[c].numOfLinks);
        for (int l=0; l<menu[c].numOfLinks; l++)
        {
            printf("%c (%d) ", menu[c].links[l].letter, menu[c].links[l].position);
        }
        printf("\n");
    }
}

/**************************************************************************************************\
* 
* Debugging: show a CribCircle
* 
\**************************************************************************************************/
void dumpCircle(CribCircle* circle)
{
    for (int c=0; c<circle->circleSize; c++)
    {
        printf("%c (%3d) ", circle->orgChars[c], circle->advances[c]);
    }
    printf("%c\n", circle->orgChars[0]);
}

/**************************************************************************************************\
* 
* Debugging: show all CribCricles in the set
* 
\**************************************************************************************************/
void dumpSets()
{
    for (int s=0; s<MAX_POSITIONS; s++)
    {
        CribCircleSet* theSet=&cribCircleSet[s];
        printf("SET %c - %3d\n", theSet->startChar, theSet->numOfCircles);
        for (int c=0; c<theSet->numOfCircles; c++)
        {
            CribCircle* theCircle=&theSet->cribCircles[c];
            dumpCircle(theCircle);
        }
    }
}

/**************************************************************************************************\
* 
* Makes an inventory of the links between any two letters in the text and crib
* text: cipher text
* crib: piece of plain text that corresponds with the cipher
* cribStartPosition: array position of the crib with respect to the cipher
* 
\**************************************************************************************************/
void turingGenerateLetterLinks(char* text, char* crib, int cribStartPosition)
{
    if (strlen(text)>=cribStartPosition+strlen(crib))
    {
        // Reset links
        for (int pos=0; pos<MAX_POSITIONS; pos++)
        {
            menu[pos].letter       ='A'+pos;
            menu[pos].numOfLinks   =0;
        }
        
        // Build the menu
        for (int pos=0; pos<strlen(crib); pos++)
        {
            int index1=text[pos+cribStartPosition]-'A';
            int index2=crib[pos]-'A';
            
            menu[index1].links[menu[index1].numOfLinks].letter      =crib[pos];
            menu[index1].links[menu[index1].numOfLinks].position    =pos+cribStartPosition+1;
            menu[index1].numOfLinks++;
            menu[index2].links[menu[index2].numOfLinks].letter      =text[pos+cribStartPosition];
            menu[index2].links[menu[index2].numOfLinks].position    =pos+cribStartPosition+1;
            menu[index2].numOfLinks++;
        }
    }
    else
    {
        logFatal("Crib exceeds text; not allowed\n");
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
    // Recursion depth
    if (step>stepMax)
    {
        stepMax=step;
    }
    
    if (step>MAX_POSITIONS)
    {
        // Fall back to prevent endless loops
        // Should never happen, unfortunatelly it does
        logFatal("Endless loop\n");
    }
    else if (cribCircleSet[(int)startChar-'A'].numOfCircles>=MAX_CIRCLES)
    {
        logFatal("Maximum number of crib circles %d reached for %c\n", MAX_CIRCLES, startChar);
    }
    else
    {
        // On the 1st call the currentLink and circle are set to NULL
        if (currentLink==NULL || circle==NULL)
        {
            // Special case: currentLink==NULL: use the start character 
            // as starting point and add all its links
            int nextLetter          =startChar;
            LinkedLetters* nextLinks=&menu[nextLetter-'A'];
            for (int l=0; l<nextLinks->numOfLinks; l++)
            {
                // Create the first circle
                CribCircle* nextCircle          =createCircle();
                int circleSize                  =nextCircle->circleSize; // =0
                // Add next link to circle
                nextCircle->advances[circleSize]=nextLinks->links[l].position;
                nextCircle->orgChars[circleSize]=nextLetter;
                nextCircle->circleSize++;
                // Iterate
                followLoop(startChar, &nextLinks->links[l], nextCircle, step+1);
                destroyCircle(nextCircle);
            }
        }
        else
        {
            int nextLetter  =currentLink->letter;
            if (nextLetter==startChar)
            {
                // Yes! we found a loop
                circle->orgChars[circle->circleSize]=startChar;         // finish character string
                circle->orgChars[circle->circleSize+1]='\0';
                CribCircleSet* theSet   =&cribCircleSet[(int)startChar-'A'];        // Add circle to set
                CribCircle*    theCircle=&theSet->cribCircles[theSet->numOfCircles];
                *theCircle              =*circle; // Copy
                theSet->numOfCircles++;
            }
            else
            {
                LinkedLetters* nextLinks=&menu[nextLetter-'A'];
                for (int l=0; l<nextLinks->numOfLinks; l++)
                {
                    // Make sure the next position not already
                    // occurs in the circle: only allowed once
                    bool found   =false;
                    for (int c=0; c<circle->circleSize && !found; c++)
                    {
                        // This one allows only simple loops, like a - b - c - a 
                        // Limits the number of loops; advantageous for large crib sizes
//                        if (circle->advances[c]==nextLinks->links[l].position)

                        // This one allows also complicated loops, like a - b - c - b - a
                        // increasing the number of loops exponentially; advantageous for 
                        // not to large crib sizes (up to ~25 chars)
                        if (circle->orgChars[c]==nextLetter)
                        {
                            found=true;
                        }
                    }
                    // Do not loop back :-)
                    if (nextLinks->links[l].position!=currentLink->position && !found)
                    {
                        // Copy circle
                        CribCircle* nextCircle          =createCircle();
                        *nextCircle                     =*circle;
                        // Add next link to circle
                        int circleSize                  =nextCircle->circleSize;
                        nextCircle->advances[circleSize]=nextLinks->links[l].position;
                        nextCircle->orgChars[circleSize]=nextLetter;
                        nextCircle->circleSize++;
                        followLoop(startChar, &nextLinks->links[l], nextCircle, step+1);
                        destroyCircle(nextCircle);

                    }
                }
            }
        }
    }
}


/**************************************************************************************************\
* 
* This method transfers a text and a crib into a list of crib circles; assumes the crib length
* is smaller than or equal to text length
* 
\**************************************************************************************************/
void turingFindLoops(char* text, char* crib, int cribStartPosition)
{
    if (strlen(text)>=strlen(crib))
    {
        // create the menu (the letter links)
        turingGenerateLetterLinks(text, crib, cribStartPosition);
       
        stepMax=0;
        for (int c=0; c<MAX_POSITIONS; c++)
        {
            // reset the circle set for this letter
            cribCircleSet[c].numOfCircles   =0;
            cribCircleSet[c].startChar      ='A'+c;
            
            // recursivel generate the crib loops
            followLoop('A'+c, NULL, NULL, 0);
        }
    }
    else
    {
        logFatal("Invalid crib length\n");
    }
}

/**************************************************************************************************\
* 
* When we find a character that fulfills all cribs in the set, we know the counterpart in the 
* crib: this is the one the character is steckered to. This not only holds for the character 
* found, but also to all intermediate characters that are encountered when processing the crib:
* they are steckered to the counterparts in the crib. This function find all steckers that can 
* be derived in this way.
* enigma        : enigma to use
* g1, g2, g3    : grundstellung to start with
* theSet        : the set of crib circles to process
* theChar       : the character found
* chars         : the array of steckered chars to update 
* 
\**************************************************************************************************/
void processIntermediateChars(Enigma* enigma, int g1, int g2, int g3, CribCircleSet* theSet, int theChar, SteckeredChars* chars)
{
    for (int circle=0; circle<theSet->numOfCircles; circle++)
    {
        CribCircle* cribCircle  =&theSet->cribCircles[circle];
        for (int e=0; e<cribCircle->circleSize; e++)
        {
            int encodedC=cribCircle->orgChars[e]-'A';
            chars[encodedC].foundChar=theChar+'A';
            setGrundStellung(enigma, 1, g1);
            setGrundStellung(enigma, 2, g2);
            setGrundStellung(enigma, 3, g3);
            
            advances(enigma, cribCircle->advances[e]);
            theChar=encodeCharacter(enigma, theChar);
        }
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
* Performance improvement: Instead of setting up the enigma for each hypohesis character, we 
* set it up once and pass all 26 characters at once. Abort when no char is found for a circle
* 3.0 times as fast as previous version :-)
*
\**************************************************************************************************/
int turingValidateHypotheses(Enigma* enigma, int g1, int g2, int g3, SteckeredChars* chars)
{
    // We check if for each set of crib circles for each letter
    // there is a character (hypothesis) that fullfills 
    // all circles in the set. 
    // Parse the sets
    bool found      =true;
    for (int set=0; set<MAX_POSITIONS && found; set++)
    {
        CribCircleSet*  theSet=&cribCircleSet[set];
        
        if (theSet->numOfCircles>0)
        {
            // Try the circles. If one circle fails
            // hypothesis is rejected, proceed to next char

            // We start with an array of all characters and are going to pass them all through the circles
            // We are going to look for characters that fulfill all circles
            // As long as there is at least one character (found=true), we continue
            int theChars[MAX_POSITIONS];
            for (int i=0; i<MAX_POSITIONS; i++)
            {
                theChars[i]=i;
            }
            found=true;
            for (int circle=0; circle<theSet->numOfCircles && found; circle++)
            {
                CribCircle* cribCircle  =&theSet->cribCircles[circle];
                for (int e=0; e<cribCircle->circleSize; e++)
                {
                    setGrundStellung(enigma, 1, g1);
                    setGrundStellung(enigma, 2, g2);
                    setGrundStellung(enigma, 3, g3);
                    
                    advances(enigma, cribCircle->advances[e]);
                    for (int i=0; i<MAX_POSITIONS; i++)
                    {
                        if (theChars[i]>=0)
                        {
                            theChars[i]=encodeCharacter(enigma, theChars[i]);
                        }
                    }                    
                }
                found=false;
                for (int i=0; i<MAX_POSITIONS; i++)
                {
                    if (theChars[i]==i)
                    {
                        found=true;     // Yup, still a character fullfilling the circles so far
                    }
                    else
                    {
                        theChars[i]=-1; // remove character from processing
                    }
                }     
            }
            // Store the character c that works
            // for all circles in the set
            if (found)
            {
                int fc=0;
                int foundChar=-1;
                for (int i=0; i<MAX_POSITIONS; i++)
                {
                    if (theChars[i]==i)
                    {
                        foundChar=i;
                        fc++;
                    }
                }
                processIntermediateChars(enigma, g1, g2, g3, theSet, foundChar, chars);
                if (fc>1)
                {
                    logError("Found multiple %d - %c->%c circles %d\n", fc, theSet->startChar, foundChar+'A', theSet->numOfCircles);
                }
            }
        }
    }
    return found;
}

/**************************************************************************************************\
* 
* Helper: For a found set of steckered values, check if the set is consistent
* 
\**************************************************************************************************/
int turingValidateTheSteckeredValues(SteckeredChars* chars)
{
    bool found=true;
    // Now we found a potential engima settings
    // Check for inconsistencies in the steckered letters.
    // This means: steckered letters may not appear more than once
    // unless mutual
    for (int set=0; set<MAX_POSITIONS && found; set++)
    {
        for (int set2=set+1; set2<MAX_POSITIONS && found; set2++)
        {
            if ((chars[set].foundChar==chars[set2].startChar) &&
                (chars[set2].foundChar!=chars[set].startChar) &&
                (chars[set2].foundChar!='?'))                                             
                 
            {
                found=false;
            }
            if ((chars[set].foundChar!='?') &&
                (chars[set].startChar==chars[set2].foundChar) &&
                (chars[set].foundChar!=chars[set2].startChar))
            {
                found=false;
            }
            if ((chars[set].foundChar!='?') &&
                (chars[set2].foundChar!='?') &&
                (chars[set].foundChar==chars[set2].foundChar))
            {
                found=false;
            }
        }
    }
    return found;
}

/**************************************************************************************************\
* 
* Helper: Print a found solution
* 
\**************************************************************************************************/
void turingPrintSolution(TuringResult* result)
{
    EnigmaSettings* settings=&result->settings;
    logInfo("Solution found: %s - %s %s %s R %d %d %d, G %d %d %d, %s Score %f",
            settings->ukw,
            settings->walzen[0], settings->walzen[1], settings->walzen[2],
            settings->ringStellungen[0], settings->ringStellungen[1], settings->ringStellungen[2],
            settings->grundStellungen[0], settings->grundStellungen[1], settings->grundStellungen[2],
            settings->steckers, result->score);
    logInfo("Solution: %s", result->decoded);
}

/**************************************************************************************************\
* 
* Helper: Create array of steckered chars (per thread)
* 
\**************************************************************************************************/
SteckeredChars* createSteckeredChars()
{
    SteckeredChars*  chars=malloc(sizeof(SteckeredChars)*MAX_POSITIONS);
    for (int c=0; c<MAX_POSITIONS; c++)
    {
        chars[c].startChar='A'+c;
        chars[c].foundChar='?';
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
    int s=0;
    for (int c=0; c<MAX_POSITIONS; c++)
    {
        if (chars[c].foundChar!=chars[c].startChar && 
            chars[c].foundChar!='?')
        {
            bool found=false;
            for (int c2=0; c2<s*3 && !found; c2++)
            {
                if (string[c2]==chars[c].foundChar)
                {
                    found=true;
                }
            }
            if (!found)
            {
                string[s*3]  =chars[c].startChar; 
                string[s*3+1]=chars[c].foundChar; 
                string[s*3+2]=' '; 
                s++;
            }
        }
    }
    string[s*3-1]='\0';
}


/**************************************************************************************************\
* 
* We found a result. Process it:
* * create score
* * add to results list
* * print it
* 
\**************************************************************************************************/
void processResult(TuringResult* result)
{
    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, &result->settings);
    encodeDecode(enigma);
    strncpy(result->decoded, toString(enigma), MAX_TEXT-1);
    result->score=iocIndexOfCoincidence(enigma);
    destroyEnigma(enigma);
    turingPrintSolution(result);
    if (theResults!=NULL)
    {
        linkedListAppendObject(theResults, result);
    }
    else
    {
        free(result);
    }
}


/**************************************************************************************************\
* 
* Simulates the Turing solution: finds the Walzen, RingStellungen and GrundStellungen based
* on a 'crib'. It tries permutations [permutationStart, permutationEnd)
* 
\**************************************************************************************************/
void turingFind(int permutationStart, int permutationEnd)
{
    int             g1, g2, g3, r1, r2, r3;
    struct timeval  stop, start;
    
    logInfo("Processing permutations %d-%d", permutationStart, permutationEnd);

    // Create a set of steckered values (for this thread)
    SteckeredChars* steckeredChars=createSteckeredChars();

    Enigma* enigma=createEnigmaM3(); 
    clearSteckerBrett(enigma);
  
    // Parse the Walze permutations
    for (int w=permutationStart; w<permutationEnd; w++)
    {
        int* permutation=(int*)linkedListObjectAt(tPermutations, w);
        
        if (permutation!=NULL)
        {
            char* w1=walzeNames[permutation[1]];
            char* w2=walzeNames[permutation[2]];
            char* w3=walzeNames[permutation[3]];
            char* ukw=umkehrWalzeNames[permutation[0]];
            sprintf(walzenString,"%6s - %5s %5s %5s", ukw, w1, w2, w3);
           
            logInfo("Processing walzen permutation %3d:%15s", w, walzenString);
            fflush(stdout);
            gettimeofday(&start, NULL);
            long counting        =0;

            // Set the Walzen
            placeWalze(enigma, 1, w1);
            placeWalze(enigma, 2, w2);
            placeWalze(enigma, 3, w3);
            placeUmkehrWalze(enigma, ukw);
            
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
                        // It must be taken into account when the notch position of
                        // the second Walze is reached after a few rotations of R3...
                        r1=theRecipe.R1; 
                        for (r2=theRecipe.startR2; r2<=theRecipe.endR2; r2++)
                        {
                            for (r3=theRecipe.startR3; r3<=theRecipe.endR3; r3++)
                            {

                                setRingStellung(enigma, 1, r1);
                                setRingStellung(enigma, 2, r2);
                                setRingStellung(enigma, 3, r3);

                                bool found=turingValidateHypotheses(enigma, g1, g2, g3, steckeredChars);
                                
                                if (found)
                                {
                                    found=turingValidateTheSteckeredValues(steckeredChars);
                                }                                    
                                
                                if (found)
                                {
                                    mutexLock();
                                    TuringResult* result=malloc(sizeof(TuringResult));
                                    result->settings.numberOfWalzen     =3;
                                    strncpy(result->settings.cipher  , theRecipe.cipher, MAX_TEXT-1);
                                    strncpy(result->settings.walzen[0],  w1, MAX_WALZE_NAME-1);
                                    strncpy(result->settings.walzen[1],  w2, MAX_WALZE_NAME-1);
                                    strncpy(result->settings.walzen[2],  w3, MAX_WALZE_NAME-1);
                                    strncpy(result->settings.ukw      , ukw, MAX_WALZE_NAME-1);
                                    result->settings.ringStellungen[0]  =r1;
                                    result->settings.ringStellungen[1]  =r2;
                                    result->settings.ringStellungen[2]  =r3;
                                    result->settings.grundStellungen[0] =g1;
                                    result->settings.grundStellungen[1] =g2;
                                    result->settings.grundStellungen[2] =g3;
                                    convertSteckeredCharsToString(steckeredChars, result->settings.steckers);
                                    processResult(result);
                                    mutexUnlock();
                                }
                                counting++;
                            }
                        }
                    }

                }
            }
            gettimeofday(&stop, NULL);
            float timeDiff=timeDifference(start, stop);
            logInfo("Walzen permutation %3d:%15s processed, %.0f ms, count %ld, speed %.0f counts/sec", w, walzenString, timeDiff, counting, 1000*counting/timeDiff);
        }
        else
        {
            logFatal("Error!! Permutation is NULL");
            fflush(stdout);
        }
    }
    destroyEnigma(enigma);
    free(steckeredChars);
}   

/**************************************************************************************************\
* 
* Worker function. Processes the work item parsed.
* 
\**************************************************************************************************/
void turingWorkerFunction(int worker, int workItem, void* params)
{
    ThreadWork* item=(ThreadWork*)params;
    logInfo("Worker %d starting work item %3d-%3d", worker, item->permutationStart, item->permutationEnd);
    turingFind(item->permutationStart, item->permutationEnd);    
}


/**************************************************************************************************\
* 
* Finish function. Executed after the work is done. Deletes the permutations.
* 
\**************************************************************************************************/
void turingFinishFunction(void* params)
{

}


/**************************************************************************************************\
* 
* The implementation of the Bombe
* 
\**************************************************************************************************/
void bombeProcess(int cribPosition)
{
    logInfo("Starting the Bombe process for '%s' at position %d", theRecipe.crib, cribPosition);

    turingFindLoops(theRecipe.cipher, theRecipe.crib, cribPosition);

    // Choose from the 5 wehrmacht walzen and 2 UKWs on an M3 Enigma
    if (theRecipe.customPermutations==NULL)
    {
        tPermutations=generateWalzePermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    }
    else
    {
        tPermutations=theRecipe.customPermutations;
    }
    
    int numberOfPermutations=linkedListLength(tPermutations);
    logInfo("Walzen/UKW permutations %d", numberOfPermutations);

    // Each thread gets one work item
    int numOfThreads=theRecipe.numberOfThreads;
    dispatcherClearWorkItems();
    for (int w=0; w<numOfThreads; w++)
    {
        work[w].permutationStart  =(w)*numberOfPermutations/numOfThreads;
        work[w].permutationEnd    =(w+1)*numberOfPermutations/numOfThreads;
        dispatcherPushWorkItem(turingWorkerFunction, &work[w]);
    }

    // Start the time, start the threads and wait till finished
    startTime=time(NULL); 
    dispatcherStartWork(numOfThreads, turingFinishFunction, NULL, true);

}

/**************************************************************************************************\
* 
* Aligns crib to cipher so that no characters match. Returns a linked list with positions that
* the crib fits to; destroy after use!!
* 
\**************************************************************************************************/
LinkedList* turingCribFit(char crib[], char cipher[])
{
    LinkedList* positions=linkedListCreate();
    int cribLength  =strlen(crib);
    int cipherLength=strlen(cipher);
    for (int i=0; i<=cipherLength-cribLength; i++)
    {
        bool found=true;
        for (int j=0; j<cribLength && found; j++)
        {
            if (crib[j]==cipher[i+j])
            {
                found=false;
            }
        }
        if (found)
        {
            int* foundPos=malloc(sizeof(int));
            *foundPos=i;
            linkedListAppendObject(positions, foundPos);
        }
    }
    return positions;
}

/**************************************************************************************************\
* 
* Implements the Turing Bombe. Parse recipe to configure the process:

* cipher            : cipher string, upper case!!
* crib              : crib, upper case. Length must be shorter than cipher and not to long 
*                     to prevent stack overflow
* cribStartPosition : start position in the cipher that corresponds with the start of the crib;
                      if unknown, pass -1; the software will try all legal crib positions
* numberOfThreads   : Use multiple threads to use multi processor cores. Walze combinations are 
*                     split up amongst the threads. Number of permutations (60) should be divisable 
*                     by this number. Hence 1, 2, 3, 4, 5, 6, 10 will do.
* R1                : Value of Ringstellung 1, has no influence
* startR2           : Start value of Ringstellung 2, use 'A'
* endR2             : End value fo Ringstellung 2, use 'A' unless no results are returned
* startR3           : Start value of Ringstellung 3, of the fastes Walze, use 'A'
* endR3             : End value of Ringstellung 3, use 'Z'
* customPermutations: If NULL, a list of permutations will be generated based on next parameters
* enigmaType        : Type of Enigma (only M3 supported)
* walzeSet          : The set of Walzen/UKWs to use
* 
\**************************************************************************************************/
void turingBombe(TuringRecipe recipe, LinkedList* results)
{
    theRecipe           =recipe;
    theResults          =results;

    if (theRecipe.cribPosition>=0)
    {
        bombeProcess(theRecipe.cribPosition);
    }
    else
    {
        LinkedList* positions=turingCribFit(theRecipe.crib, theRecipe.cipher);
        linkedListReset(positions);
        while (linkedListHasNext(positions))
        {
            int* position=(int *)linkedListNextObject(positions);
            bombeProcess(*position);
        }
        linkedListDestroy(positions, true);
    }
    destroyPermutations(tPermutations);
}

/**************************************************************************************************\
* 
* Create a default recipe with most common settings
* 
\**************************************************************************************************/
TuringRecipe* createDefaultTuringRecipe(char* cipher, char* crib, int cribPosition, int numberOfThreads)
{
    TuringRecipe* recipe;
    if (strlen(crib)+cribPosition>strlen(cipher))
    {
        logFatal("Create default recipe: crib exceeds ciphersize");
    }

    recipe                      =malloc(sizeof(TuringRecipe));
    recipe->cipher              =cipher;
    recipe->crib                =crib;
    recipe->cribPosition        =cribPosition;
    recipe->numberOfThreads     =numberOfThreads;
    recipe->R1                  ='A';
    recipe->startR2             ='C';
    recipe->endR2               ='C';
    recipe->startR3             ='A';
    recipe->endR3               ='Z';
    recipe->customPermutations  =NULL;
    recipe->enigmaType          =ENIGMATYPE_M3;
    recipe->walzeSet            =M3_ARMY_1938;
    return recipe;
}


/**************************************************************************************************\
* 
* Destroy settings
* 
\**************************************************************************************************/
void destroyTuringRecipe(TuringRecipe* recipe)
{
    free(recipe);
}



