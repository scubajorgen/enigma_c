#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

#include "toolbox.h"
#include "enigma.h"

#define NUMBER_OF_THREADS   1

typedef struct
{
    int circleSize;
    int advances[10];
}
CribCircle;

// TEST
char decoded[]="wettervorhersagebiskaya";
char cypher[] ="gzsuxqbbefsahpolvbmckzq";

int cribCircleSize=3;
int priorAdvances[]={6, 23, 12}; // rq qa ar


#define numOfCircles 2
CribCircle  cribCircles[numOfCircles]=
{
    {2, {7, 17}},      // vb bv
    {3, {6, 23, 12}} // rq qa ar
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


void implicitAdvance(Enigma* enigma, int places)
{
    int i;
    
    setText(enigma, "x"); // don;t care
    i=0;
    while (i<places)
    {
        encodeDecode(enigma);
        i++;
    }
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void turingProve()
{
    Enigma* enigma;
    char*   decoded;
    char    temp[3];
    
    enigma=createEnigmaM3();
    
    placeWaltze         (enigma, 1, "I");
    placeWaltze         (enigma, 2, "II");
    placeWaltze         (enigma, 3, "III");
    setRingStellungen   (enigma, "26 13 01");
    
    placeSteckers       (enigma, "bq cr di ej kw mt os px uz gh");
    placeUmkehrWaltze   (enigma, "UKW B");
    
    temp[0]='R';
    temp[1]='\0';

    printf("Input: %s, ", temp);

    setGrundStellungen  (enigma, "01 02 03");
    implicitAdvance     (enigma, priorAdvances[0]-1);
    setText             (enigma, temp);
    
    encodeDecode        (enigma);
    decoded=toString    (enigma);
    temp[0]=decoded[0];

    setGrundStellungen  (enigma, "01 02 03");
    implicitAdvance     (enigma, priorAdvances[1]-1);
    setText             (enigma, temp);
    
    encodeDecode        (enigma);
    decoded=toString    (enigma);
    temp[0]=decoded[0];    

    setGrundStellungen  (enigma, "01 02 03");
    implicitAdvance     (enigma, priorAdvances[2]-1);
    setText             (enigma, temp);
    
    encodeDecode        (enigma);
    decoded=toString    (enigma);

    printf("output: %s\n", decoded);
   
    destroyEnigma(enigma);    
}




/**************************************************************************************************\
* 
* 
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
                        r1=1;
                        for (r2=1; r2<=26; r2++)
                        {
                            for (r3=1; r3<=26; r3++)
                            {
                                setRingStellung(enigma, 1, r1);
                                setRingStellung(enigma, 2, r2);
                                setRingStellung(enigma, 3, r3);

                                found   =1;
                                circle  =0;
                                while (circle<numOfCircles && found)
                                {
                                    c       =0;
                                    found   =0;
                                    while (c<MAX_POSITIONS && !found)
                                    {
                                        theChar =c;
                                        e       =0;
                                        while (e<cribCircles[circle].circleSize)
                                        {
                                            setGrundStellung(enigma, 1, g1);
                                            setGrundStellung(enigma, 2, g2);
                                            setGrundStellung(enigma, 3, g3);
                                            
                                            advances(enigma, cribCircles[circle].advances[e]);
                                            
                                            theChar=encodeCharacter(enigma, theChar);
                                            e++;
                                        }
                                        if (theChar==c)
                                        {
                                            found=1;
                                        }
                                        
                                        c++;
                                    }
                                    circle++;
                                }
                                if (found)
                                {
                                    printf("Solution found: R %d %d %d, G %d %d %d, %c\n",
                                           r1, r2, r3, g1, g2, g3, c+'A');
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
* 
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
    permute(permutations, waltzenIndices, 8, 3, 0);
    
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

