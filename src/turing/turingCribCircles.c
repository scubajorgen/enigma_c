/**************************************************************************************************\
* turingCribCircles.c
*
* This file implements the method turing used to crack enigma encoded messages, assuming a 'crib':
* this is a piece of plain text that corresponds to part of the cipher. 
* Both are transformed into a 'menu' containing 'crib circles' or 'loops'. Crib circles are used 
* validate hypotheses regarding the steckered letters: a the hypothesis proves right if the 
* steckered value used as input generates the same value as output when traversing the loop.
* Explained in detail on http://www.ellsbury.com/bombe1.htm
* 
* This file contains the functions for generating the crib circles
*
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "log.h"
#include "toolbox.h"
#include "turing.h"

// Defines the links between letters
LinkedLetters       menu[MAX_POSITIONS];

// Defines the crib circle loops
CribCircleSet       cribCircleSet[MAX_POSITIONS];

/**************************************************************************************************\
* 
* Creates a CribCircle instance
* 
\**************************************************************************************************/
CribCircle* createCircle()
{
    CribCircle*  circle =malloc(sizeof(CribCircle));
    circle->circleSize  =0;
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
    printf("%c - size %d\n", circle->orgChars[0], circle->circleSize);
}

/**************************************************************************************************\
* 
* Debugging: show a CribCircle Set
* 
\**************************************************************************************************/
void dumpSet(CribCircleSet* theSet)
{
    printf("SET %c - %3d\n", theSet->startChar, theSet->numOfCircles);
    for (int c=0; c<theSet->numOfCircles; c++)
    {
        CribCircle* theCircle=&theSet->cribCircles[c];
        dumpCircle(theCircle);
    }
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
        dumpSet(theSet);
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
* This function indicates whether circles are equal or oposite but equal
* Like: a-b-c-a is equal to b-c-a-b and oposite equal to a-c-b-a
* 
\**************************************************************************************************/
bool turingIsEqual(CribCircle* circle1, CribCircle* circle2)
{
    bool isEqual=true;
    int circleSize1=circle1->circleSize;
    int circleSize2=circle2->circleSize;
    if (circleSize1==circleSize2)
    {
        char startChar=circle1->orgChars[0];  // start char of circle1
        
        // Find it in circle2, generating an offset
        int offset=-1;
        for (int i=0;i<circleSize2 && offset<0; i++)
        {
            if (circle2->orgChars[i]==startChar)
            {
                offset=i;
            }
        }
        if (offset>=0)
        {
            bool isEqual1=true;
            bool isEqual2=true;
            for (int i=0; i<circleSize1 && (isEqual1 || isEqual2); i++)
            {
                // a-b-c-a vs a-b-c-a
                // a-b-c-a vs b-c-a-b
                if ((circle1->orgChars[i]!=circle2->orgChars[(i+offset)%circleSize1]) ||
                    (circle1->advances[i]!=circle2->advances[(i+offset)%circleSize1]))
                {
                    isEqual1=false;
                }
                // a-b-c-a vs a-c-b-a
                // a-b-c-a vs b-a-c-b
                if ((circle1->orgChars[i]!=circle2->orgChars[(circleSize1-i  +offset)%circleSize1]) ||
                    (circle1->advances[i]!=circle2->advances[(circleSize1-i-1+offset)%circleSize1]))
                {
                    isEqual2=false;
                }
            }
            isEqual=isEqual1 || isEqual2;
        }
        else
        {
            isEqual=false;
        }
    }
    else
    {
        isEqual=false;
    }
    return isEqual;
}

/**************************************************************************************************\
* 
* This function checks of given circle with indicate startChar already exists in some form
* in the set of given circle
* 
\**************************************************************************************************/
bool turingCribCircleExistsInOwnSet(CribCircle* circle)
{
    bool exists=false;

    // Check of the circle exists only in the set of its own start char
    // This only removes the doubles and inverse doubles from each set, like
    // a-b-c-a -> remove a-b-c-a and a-c-b-a. Reduces number of circles to 50%
    char startChar=circle->orgChars[0];
    CribCircleSet* set   =&cribCircleSet[(int)startChar-'A'];
    for (int j=0; j<set->numOfCircles && !exists;j++)
    {
        if (turingIsEqual(circle, &set->cribCircles[j]))
        {
            exists=true;
        }
    }
    return exists;
}

/**************************************************************************************************\
* 
* This function checks of given circle with indicate startChar already exists in some form
* in the list of circle sets
* 
\**************************************************************************************************/
bool turingCribCircleExistsInAllSets(CribCircle* circle)
{
    bool exists=false;

    // Check of the circle exists in the set of each of the letters
    // a-b-c-a -> remove a-b-c-a, a-c-b-a, b-c-a-b, b-a-c-b, etc
    // Reduces number of circles to ~6%, because long crib circles occur in set of each letter in the crib
    // Note: this results of concentration of circles in the set of first characters, depleting sets
    // of next characters. The lower the number of circles, the more false positives
    for (int i=0; i<circle->circleSize; i++)
    {
        char startChar=circle->orgChars[i];
        CribCircleSet* set   =&cribCircleSet[(int)startChar-'A'];
        for (int j=0; j<set->numOfCircles && !exists;j++)
        {
            if (turingIsEqual(circle, &set->cribCircles[j]))
            {
                exists=true;
            }
        }
    }
    return exists;
}

/**************************************************************************************************\
* 
* Recursive function that find all loops starting from one character.
* For longer cribs this method will result in stack overflow.
* 
\**************************************************************************************************/
void followCribCircle(char startChar, LetterLink* currentLink, CribCircle* circle, bool onlyUnique, int step)
{
    if (step>MAX_POSITIONS)
    {
        // Fall back to prevent endless loops
        // Should never happen, unfortunatelly it does
        logFatal("Endless loop\n");
    }
/*
    else if (cribCircleSet[(int)startChar-'A'].numOfCircles>=MAX_CIRCLES)
    {
        logFatal("Maximum number of crib circles %d reached for %c\n", MAX_CIRCLES, startChar);
    }
*/
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
                followCribCircle(startChar, &nextLinks->links[l], nextCircle, onlyUnique, step+1);
                destroyCircle(nextCircle);
            }
        }
        else
        {
            int nextLetter  =currentLink->letter;
            if (nextLetter==startChar)
            {
                // Yes! we found a loop
                circle->orgChars[circle->circleSize]=startChar;                     // finish character string
                circle->orgChars[circle->circleSize+1]='\0';
                CribCircleSet* theSet   =&cribCircleSet[(int)startChar-'A'];        // Add circle to set
                CribCircle*    theCircle=&theSet->cribCircles[theSet->numOfCircles];
                bool alreadyExists=false;
                if (onlyUnique)
                {
                    alreadyExists=turingCribCircleExistsInAllSets(circle);
                }
                else
                {
                    alreadyExists=turingCribCircleExistsInOwnSet(circle);
                }
                if (!alreadyExists)
                {
                    if (theSet->numOfCircles<MAX_CIRCLES)
                    {
                        *theCircle              =*circle;                                   // Copy
                        theSet->numOfCircles++;
                    }
                    else
                    {
                        logFatal("Maximum number of crib circles %d reached for %c\n", MAX_CIRCLES, startChar);
                    }
                }
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
                        followCribCircle(startChar, &nextLinks->links[l], nextCircle, onlyUnique, step+1);
                        destroyCircle(nextCircle);

                    }
                }
            }
        }
    }
}


/**************************************************************************************************\
* 
* This is where the magic happens...
* This method transfers a text and a crib into a list of crib circles; assumes the crib length
* is smaller than or equal to text length.
* First the list of letter links is established
* Then in an iterative way circles are determined, per starting letter (A, B, C, ...)
* 
* The onlyUnique parameter indicates when circles are going to be added:
* true : a circle is only added when they are unique over all crib sets
* false: circles are only unique within their own set, but are added to the sets of each letter 
*        occurring in the circle a-b-c-a is also added as b-c-a-b and c-a-b-c.
* false is preferred, since it makes sets 'stronger'
* 
\**************************************************************************************************/
void turingFindCribCircles(char* text, char* crib, int cribStartPosition, bool onlyUnique)
{
    if (strlen(text)>=strlen(crib))
    {
        // create the menu (the letter links)
        turingGenerateLetterLinks(text, crib, cribStartPosition);
       
        for (int c=0; c<MAX_POSITIONS; c++)
        {
            // reset the circle set for this letter
            cribCircleSet[c].numOfCircles   =0;
            cribCircleSet[c].startChar      ='A'+c;
            
            // recursivel generate the crib loops
            followCribCircle('A'+c, NULL, NULL, onlyUnique, 0);
        }
    }
    else
    {
        logFatal("Invalid crib length\n");
    }
}

/**************************************************************************************************\
* 
* Total number of crib circles
* 
\**************************************************************************************************/
int totalNumberOfCribCircles()
{
    int sum=0;
    for (int i=0;i<MAX_POSITIONS;i++)
    {
        sum+=cribCircleSet[i].numOfCircles;
    }
    return sum;
}

/**************************************************************************************************\
* 
* longest circle in all sets
* 
\**************************************************************************************************/
int maxCribCircleSize()
{
    int max=0;
    for (int i=0;i<MAX_POSITIONS;i++)
    {
        for (int j=0; j<cribCircleSet[i].numOfCircles; j++)
        {
            if (cribCircleSet[i].cribCircles[j].circleSize>max)
            {
                max=cribCircleSet[i].cribCircles[j].circleSize;
            }
        }
    }
    return max;
}

/**************************************************************************************************\
* 
* longest circle in all sets
* 
\**************************************************************************************************/
int minCribCircleSize()
{
    int min=1000000;
    for (int i=0;i<MAX_POSITIONS;i++)
    {
        for (int j=0; j<cribCircleSet[i].numOfCircles; j++)
        {
            if (cribCircleSet[i].cribCircles[j].circleSize<min)
            {
                min=cribCircleSet[i].cribCircles[j].circleSize;
            }
        }
    }
    return min;
}

/**************************************************************************************************\
* 
* longest circle in the set
* 
\**************************************************************************************************/
int maxSetCribCircleSize(CribCircleSet* set)
{
    int max=0;

    for (int j=0; j<set->numOfCircles; j++)
    {
        if (set->cribCircles[j].circleSize>max)
        {
            max=set->cribCircles[j].circleSize;
        }
    }
    return max;
}

/**************************************************************************************************\
* 
* Average crib cicle size
* 
\**************************************************************************************************/
float averageCribCircleSize()
{
    int sum     =0;
    int count   =0;
    for (int i=0;i<MAX_POSITIONS;i++)
    {
        for (int j=0; j<cribCircleSet[i].numOfCircles; j++)
        {
            if (cribCircleSet[i].cribCircles[j].circleSize>0)
            {
                sum+=cribCircleSet[i].cribCircles[j].circleSize;
                count++;
            }
        }
    }
    float average=0.0;
    if (count>0)
    {
        average=(float)sum/count;
    }
    return average;
}
