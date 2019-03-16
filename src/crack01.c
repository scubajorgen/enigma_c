#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>

#include "enigma.h"
#include "toolbox.h"

#define NUMBER_OF_THREADS   3

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



ThreadParam params[4];
 
char text[]=
    "ewklmonitsgaeeneevleusccseemdtoeeviregtspaepsnotqpfzqaizemqv" 
    "baqqezzufebmifqqqnmxpzqtxqqzvfeqfebalkramnfncxlgihwzlclzluhm" 
    "rhrclcyqhadxwlxllucdcrqlnyuxxccxydnuhzcqhraaalqllqywldwpcifr" 
    "tlzsdhlyjvieucwginpkcabomqxhyrnqgfocqblczxcwllgxocqbdcngyanq" 
    "kfsfiwcmgcvmtylpmcwwjztqvknyouggrmclzmeybvwcenuwgkkdeymyycup" 
    "rfjkmyvounibdylnblbqazqbzisntslwxtiddydwzbsyfhiltylwzblinzgl" 
    "dyceblcwatwszgznldsinhkdbysnddbymtglbgsnbloyrzystydeldcwatyc" 
    "libwllcwatglsilezdlyxzqdsqzfblbqrzyslidepdcwazysdgsnblcwnhyd" 
    "tybntbbifzwlzycwzlswfzglsydwzdoyrzyslidepddwmpgbbgcnpdoyxtil" 
    "dglnbslqfpgdsidwbbcwrlidtgcntlsirlybsqzwpblirlwlsylyldoialql" 
    "zycnbssqnhqstiswpdbwmzwdtqbnzslqnpqlzkdebdsynhglsgsnblcwnzqd" 
    "tglndscqxpyszqseblswatibtizwlbcwatiddgzypsdwnpgslidwldlqrhwc" 
    "dglebd";   



/*
I II III, RingStellungen 1 1 1, Grundstellungen 1 5 3, UKW B
Nadat eerst de Poolse en nadien, tijdens de Tweede Wereldoorlog, 
de Britse inlichtingendienst erin slaagde de Enigmacodes te breken, 
bleek het toestel een goudmijn van informatie over de Duitse oorlogsmachine. 
Deze informatie, verkregen door ontcijfering van de geheime Duitse 
berichten, kreeg de codenaam "Ultra" en speelde een uiterst belangrijke 
rol in het verloop van de Tweede Wereldoorlog, vooral in de U-bootoorlog 
in de Atlantische Oceaan, de veldslagen in Afrika en de landing in Normandië.
*/
char text_test[]=
"qwifp pcdip norty hdunw acgoq mxlio wddwx eewkx qxrsj onkjk hbxjx "
"cqnjf iupjk jfqsf vguvr uxplb bjsni vbkcl fdzek qgzfh rvkqw fqjsc "
"wlxln yyqcc chtsi rakgh xkhcz uvusk mowjp lrdmm irmie fkjxm nxclw "
"ndovx djqba jnomd worqg ouvnt qsyiz uoffs yaots kyjwm rlwcx gqwfw "
"suclm qrxva iavhu pvvop tixfn wzpae hjpdc ndwop glisd yzhes zklpv "
"xozgj rffbx gspvq xlpup hssnp wkevj nmyzc xkpyo dvjtm efcnw rpbsw "
"ytwpd mydlc ykbnb cttsy pcypm cokfs brynx uimac jsjom mixxi sxufp "
"zlbfz byjtc uqqkz zhcpw emsgx mez";

    time_t          startTime;



/**************************************************************************************************\
* 
* Count letter
\**************************************************************************************************/
int countLetter(Enigma* enigma, char letter)
{
    int i;
    int max;
    int count;
    int letterPos;
    
    letterPos=charToPos(letter);
    
    count=0;
    max=enigma->textSize;
    i=0;
    while (i<max)
    {
        if (enigma->conversion[i]==letterPos)
        {
            count++;
        }
        i++;
    }
    return count;
}



    


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void tryPermutations(int permutationStart, int permutationEnd)
{
    Enigma*         enigma;
    int             g1, g2, g3, r1, r2, r3;
    int             count;
//    int             maxCount;
    long            counting;
    long            prevCounting;
    long            currentTime;
    long            diffTime;
    long            prevTime;
    long            convPerSec;
    int             limit;

    int*            permutation;
    int             w;
    
    
    pthread_mutex_lock(&mutex);
    threadsRunning++;
    pthread_mutex_unlock(&mutex);    

    enigma=createEnigmaM3(); 
    setText(enigma, text);
    // Letter frequency in dutch: e - 19%, n - 10%. Choose 12 % as limit
    limit=enigma->textSize*11/100;
    placeSteckers(enigma, "ze");
    placeUmkehrWaltze(enigma, "UKW C");

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
                   permutation[0], permutation[3], permutation[2], w, waltzenString, currentTime, convPerSec);
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

                                setGrundStellung(enigma, 1, g1);
                                setGrundStellung(enigma, 2, g2);
                                setGrundStellung(enigma, 3, g3);
                                
                                encodeDecode(enigma);

                                count=countLetter(enigma, 'E');

                                if (count>limit)
                                {
                                    printf("Found @ Ringstellungen %d %d %d Grundstellungen %d %d %d\n %s\n\n", 
                                           r1, r2, r3, g1, g2, g3, toString(enigma));
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
  
    tryPermutations(params->start, params->end);
    
    return NULL;
} 
    
/**************************************************************************************************\
* 
* Brute force crack using 4 threads
* 
\**************************************************************************************************/
void crackExample()
{
    int         i; 
    int         numberOfPermutations;


    permutations=createLinkedList();
    permute(permutations, waltzenIndices, 8, 3, 0);
    
/*    
    int*        permutation;
    i=0;
    while (i<permutations->length)
    {
        permutation=elementAt(permutations, i);
        printf("|%3d %3d %3d \n", permutation[0], permutation[1], permutation[2]);
        i++;
    }
    printf("\n");
*/    
    
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
    


