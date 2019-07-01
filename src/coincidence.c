/**************************************************************************************************\
* 
* This module implents the index-of-coincidence method of James Gillogly
* (http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)
* Different from the original article, this software tries all rotor settings, 
* RingStellungen included 
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

#define MAX_TRIGRAMS        54


/** Defines the method for decrypting the engima cypher */
typedef struct
{
    method_t    method;             // The method
    int         maxSteckers;        // Maximum number of Steckers to use
    int         maxSteckersIoc;     // METHOD_NGRAMS: the number of Steckers used for IOC; beyond this number NGRAMS
    int         ngramSize;          // n in ngram (2-bigrams, 3-trigrams)
    char        ngramSet[5];        // The set to use ('DE' - german, 'EN' - english, 'GC' - NL, geocaching)
} EvaluationMethod;


typedef struct
{
    float           indexOfCoincidence;
    EnigmaSettings  settings;
    int             steckerTable[MAX_POSITIONS];
} IocResults;



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
IocResults          iocTopTenResults[TOP_RESULTS_SIZE];

EvaluationMethod    evaluationMethod;
IocWorkItem         iocWorkItems[MAX_WORK_ITEMS];
int                 iocNumberOfWorkItems;

pthread_mutex_t     iocMutex;

IocThreadParams     threadParams[MAX_THREADS];


int                 iocThreadsRunning=0;

char iocExampleCypher[]=
"QKRQW UQTZK FXZOM JFOYR HYZWV BXYSI WMMVW BLEBD MWUWB TVHMR"
"FLKSD CCEXI YPAHR MPZIO VBBRV LNHZU POSYE IPWJT UGYOS LAOXR"
"HKVCH QOSVD TRBPD JEUKS BBXHT TGVHG FICAC VGUVO QFAQW BKXZJ"
"SQJFZ PEVJR OJTOE SLBQH QTRAA HXVYA UHTNB GIBVC LBLXC YBDMQ"
"RTVPY KFFZX NDDPC CJBHQ FDKXE EYWPB YQWDX DRDHN IGDXE UJJPV"
"MHUKP CFHLL FERAZ HZOHX DGBKO QXKTL DVDCW KAEDH CPHJI WZMMT"
"UAMQE NNFCH UIAWC CHNCF YPWUA RBBNI EPHGD DKMDQ LMSNM TWOHM"
"AUHRH GCUMQ PKQRK DVSWV MTYVN FFDDS KIISX ONXQH HLIYQ SDFHE"
"NCMCO MREZQ DRPBM RVPQT VRSWZ PGLPI TRVIB PXXHP RFISZ TPUEP"
"LKOTT XNAZM HTJPC HAASF ZLEFC EZUTP YBAOS KPZCJ CYZOV APZZV"
"ELBLL ZEVDC HRMIO YEPFV UGNDL ENISX YCHKS JUWVX USBIT DEQTC"
"NKRLS NXMXY ZGCUP AWFUL TZZSF AHMPX GLLNZ RXYJN SKYNQ AMZBU"
"GFZJC URWGT QZCTL LOIEK AOISK HAAQF OPFUZ IRTLW EVYWM DN";


IocResults  iocExampleResults=
{
    0.0440,
    {
        3,
        {
            "II",
            "I",
            "III"
        },
        "UKW B",
        {
            1, 23, 4
        },
        {
            2, 7, 9
        },
        "MV PX EZ RW JO BL IU",
        "cypher"
    }
};

char iocExampleCypher2[]=
"ISPPG RUMUK CMMCB POESR ZSZCQ FGGON YALYX AMYUB IPWQZ RKTMZ"
"GCSNZ QAWUB FGYWG PRQDS ASTKQ NYAWY LXNAU NFDUU GDBNK EKSKT"
"XFSYO THCCV YULWB OKGUJ WRFYG JBEOQ ACLGL RMUVK AUWON OBOPQ" 
"HRONM VNUDB GUVNO NJCEQ FVPKW TZZYY DZAKW NNZEN HTHPR GSHCL"
"SXHDW EWLWR EPYPZ EUQYJ GCLGN NXINL CNYYB UYOFB RYLPS WMFGO"
"UHDKH YEFOF OQHDN GGZJI ARGDF XVTPW UAKAW JRRSG WEVTL XFMWP"
"VTXXT XVLDN GNJUX WRWDM DVFSM FTBWK DGRGR RZRSU IRHQZ MLXYO"
"CIZPK QZEVR SGIRN BRRZF PXDHF PPOWI TYJYQ ZOOQJ JMISB CEQZW"
"HSKWM QHCIR XARDD XGYCE LXWYF ZAECT FSTWS GPQCS QSWEP OGMIZ"
"COURI FZLMH XBTHW CTNMY MLHJY LPJCQ FXYEB ZBBKV NFKDQ MPZYQ"
"JLYBA JEHNB INVEL HHBQE NXCNH HELEE QDBPK QBPCO WUHXF AGHIC"
"XDQAN BAQVA GDRNQ UDMTV BPRSJ ANOZN GSOEB OLZOW FHISB ROGYX"
"XEOIW XHDTJ AWYGZ SNCOO YZGAY NTWDA RNXMF YTLOV BVSDW XU";


IocResults  iocExampleResults2=
{
    0.067571,
    {
        3,
        {
            "IV",
            "V",
            "III"
        },
        "UKW B",
        {
            1, 23, 4
        },
        {
            2, 7, 9
        },
        "MV PX EZ RW JO BL IU KT AC SY",
        "cypher"
    }
};


char iocExampleCypher3[]=
" INAVH YMVHI AGMJO PKVJH SGJYY"  
" KNHLF KRZWH WLAKK EGGHZ FEAKV"
" VIDDS YYVEY QFQJP VYHLF UZESA"
" OLGNH TXTTB DZJVO AGEAW HBBWC"
" ADYYT HSLRX MPEDI CATSM ALBZY"
" LBPZM QDSXZ HPFSX VYCBK GEBTG"
" QGZII DQJDB YDACS WJGXU CUXLT"
" RTMZH HWXZP ESSYE EPFCQ AOWOS"
" PLUZU CVOKY JXCPY GNJHS PNCFS"
" WTLLM SGACQ BSUTP SAVGU YFVKS"
" UBSQE GVZKV NRLXF IXZQW FKSXC"
" PPFRI MWQHT QSB";

IocResults  iocExampleResults3=
{
    0.0440,
    {
        3,
        {
            "IV",
            "III",
            "I"
        },
        "UKW C",
        {
            7, 24, 15
        },
        {
            11, 18, 9
        },
        "AT BG DV EW FR HN IQ JX KZ LU",
        "cypher"
    }
};


IocResults  iocExampleResults3Test=
{
    0.0440,
    {
        3,
        {
            "IV",
            "III",
            "I"
        },
        "UKW C",
        {
            7, 24, 15
        },
        {
            11, 18, 9
        },
        "AT BG DV EW FR",
        ""
    }
};


IocResults  iocExampleResults3Test2=
{
    0.0,
    {
        3,
        {
            "III",
            "I",
            "II"
        },
        "UKW B",
        {
            1, 1, 1
        },
        {
            1, 17, 12
        },
//        "AZ BY CX DW EV FU GT HS IQ JR",
        "AZ BY CX DW EV FU",
//        "AZ DW EV IQ JR",
//          "AZ CG DW EV IQ NX",


        "QAVIM KGJZT XYFBC MVCON NTBZK LEZNB QMCLE UQXPY TYHEB VNCKO VACPK "
        "GCWPW UVHUF WJFWT CNIJV ULFBV ADZRQ WMLVD DQFRJ AZUPS IXACS KJHLR "
        "DQFMX PSZMK I"
    }
};

IocResults  iocExampleResults3Test3=
{
    0.0,
    {
        3,
        {
            "V",
            "I",
            "III"
        },
        "UKW B",
        {
            1, 1, 1
        },
        {
            12, 6, 18
        },

        "BW DH EM IY KL OR",
        
        "CRSMDDACIGRLKUPAHWCYFGDSEPBHSYPYYDDNWUDMBRKT"
        "GPDULRSDTRWWYWVLWKVRIBHHWFVCDXVIEREHLGSQSBIQU"
        "DGACRUFWBMYRSPYGBKESJLNNFTVAGSOXBDIOXJACSJRSAV"
        "VXDAWBIMEDLQSLQKLFHATDCUGFPOYSBKPBSFLRHUPXDMM"
        "TTDPRKZCOCNZDSULIHEJGLCLVQWXVAZZPWMOOKPPFWDSQ"
        "VW"
    }
};


int ioctrigramCount[MAX_TRIGRAMS];


/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/
/**************************************************************************************************\
* 
* 
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
* Counts the occurrences of the most occurring trigrams
* 
\**************************************************************************************************/
int iocCountTrigrams(Enigma* enigma)
{
    int i;
    int count;
    int length;
    
    length=sizeof(ioctrigrams3)/sizeof(NGram);
    
    count=0;
    i=0;
    while (i<length)
    {
        count+=countTrigram(enigma, ioctrigrams3[i].ngram)*ioctrigrams3[i].score;
        i++;
    }
    
    return count;
}

/**************************************************************************************************\
* 
* Counts the occurrences of the most occurring trigrams
* 
\**************************************************************************************************/
int iocCountNgrams(Enigma* enigma)
{
    int i;
    int count;
    int length;
    
    length=sizeof(iocBiGrams)/sizeof(NGram);
    
    count=0;
    i=0;
    while (i<length)
    {
        count+=countNgram(enigma, iocBiGrams[i].ngram, 2)*iocBiGrams[i].score;
        i++;
    }
    
    return count;
}

/**************************************************************************************************\
* 
* Store the results in the top 10 of best results
* Returns the lowest index of coincidence value
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
        if (results->indexOfCoincidence > iocTopTenResults[i].indexOfCoincidence)
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
                iocTopTenResults[i]=iocTopTenResults[i-1];
            }
            i--;
        } 
        // Add the new record
        iocTopTenResults[index]=*results;

        if (iocNumberOfResults<TOP_RESULTS_SIZE)
        {
            iocNumberOfResults++;
        }   
    }
    return iocTopTenResults[iocNumberOfResults-1].indexOfCoincidence;
}

/**************************************************************************************************\
* 
* Show the top 10 results. En passant, translate the found steckertable into stecker pairs
* 
\**************************************************************************************************/

void iocDumpTopTenResults(int withDecode)
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
        results=&iocTopTenResults[i];
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
* This method processes the indicated Waltzen permutations with UKW. It looks for the best 
* index of coincidence. The best solutions are stored in the Top 10. 
* Whereas the original James Gillogly method fixed the R2 setting and tries to find the 
* best R2 setting after the rotor settings have been found, this function varies
* also the R2 setting. Takes more time, but all settings are tried
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
    int         startR2;
    int         endR2;
    int         startR3;
    int         endR3;
    char*       ukw;
    long        startTime;
    long        count;
    LinkedList* permutations;
    char*       cypher;


    // The work item
    cypher      =work->cypher;
    permutations=work->permutations;
    start       =work->startPermutation;
    end         =work->endPermutation;
    startR2     =work->startR2;
    endR2       =work->endR2;
    startR3     =work->startR3;
    endR3       =work->endR3;
    ukw         =work->ukw;

    
    results     =malloc(sizeof(IocResults));
    
    enigma      =createEnigmaM3(); 

    placeUmkehrWaltze(enigma, ukw);
        
    clearSteckerBrett(enigma);

    setText(enigma, cypher);


    count       =0;
    startTime   =time(NULL);


    iocMax=0.0;
    // Parse the Waltzen permutations assigned
    w=start;
    while (w<=end)
    {
        permutation=(int*)elementAt(permutations, w);

        placeWaltze(enigma, 1, waltzen[permutation[0]]);
        placeWaltze(enigma, 2, waltzen[permutation[1]]);
        placeWaltze(enigma, 3, waltzen[permutation[2]]);

        printf("Trying permutation %d: %s - %s %s %s - R2 %d-%d R3 %d-%d\n", 
                w,
                ukw,
                waltzen[permutation[0]], 
                waltzen[permutation[1]], 
                waltzen[permutation[2]],
                startR2, endR2, startR3, endR3);
        fflush(stdout);

        // The Ringstellung of the 1st Waltze has no meaning
        r1=1;

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
                            
                            ioc=iocIndexOfCoincidence(enigma);
                            if (ioc>iocMax)
                            {
                                results->indexOfCoincidence=ioc;
                                results->settings.numberOfRotors=3;
                                strncpy(results->settings.cypher, cypher, MAX_TEXT);
                                strncpy(results->settings.rotors[0], waltzen[permutation[0]], MAX_ROTOR_NAME);
                                strncpy(results->settings.rotors[1], waltzen[permutation[1]], MAX_ROTOR_NAME);
                                strncpy(results->settings.rotors[2], waltzen[permutation[2]], MAX_ROTOR_NAME);
                                strncpy(results->settings.ukw, ukw, MAX_ROTOR_NAME);
                                results->settings.ringStellungen[0]=r1;
                                results->settings.ringStellungen[1]=r2;
                                results->settings.ringStellungen[2]=r3;
                                results->settings.grundStellungen[0]=g1;
                                results->settings.grundStellungen[1]=g2;
                                results->settings.grundStellungen[2]=g3;
                        
                                pthread_mutex_lock(&iocMutex);
                                iocMax=iocStoreResults(results);
                                pthread_mutex_unlock(&iocMutex);
                                iocDumpTopTenResults(0);
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
    
    printf("Executed %ld decryptions, %ld per sec\n", count, count/(time(NULL)-startTime));

}


/**************************************************************************************************\
* 
* After the rotor settings have been found, this method finds the steckers. 
* It uses the Index of Coincidence as measure of plain text fitness
* 
\**************************************************************************************************/
void iocFindSteckeredChars(IocResults* results, int maxNumOfSteckers)
{
    Enigma* enigma;
    int     s;
    int     s1;
    int     s2;
    int     s1Max;
    int     s2Max;
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
  
    enigma=createEnigmaM3();
    
    setEnigma(enigma, &results->settings);
    
    maxIoc          =results->indexOfCoincidence;
    
    s=0;
    found=1;
    while (s<maxNumOfSteckers && found)
    {
        s1Max=0;
        s2Max=0;
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
                    
                    // Quick and dirty way to set up the steckers
                    memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                    
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
                        found=1;
                    }
                    
                    // Remove stecker
                    steckerTable[s1]=s1;
                    steckerTable[s2]=s2;
                                    
                }
                s2++;
            }
            s1++;
            
        }
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            printf("Found steckered chars %c-%c : ioc %f\n", s1Max+'A', s2Max+'A', maxIoc);
            fflush(stdout);
            steckerTable[s1Max]=s2Max;
            steckerTable[s2Max]=s1Max;
        }
        s++;
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));

    // Convert the stecker positions to a stecker brett string
    s1=0;
    s2=0;
    while (s1<MAX_POSITIONS)
    {
        if (steckerTable[s1]>s1)
        {
            if (s2>0)
            {
                results->settings.steckers[3*s2-1]=' ';
            }
            results->settings.steckers[3*s2]=s1+'A';
            results->settings.steckers[3*s2+1]=steckerTable[s1]+'A';
            results->settings.steckers[3*s2+2]='\0';
            s2++;
        }
        s1++;
    }
    
    destroyEnigma(enigma);
    fflush(stdout);
        
}


/**************************************************************************************************\
* 
* After the rotor settings have been found, this method finds the steckers.
* It uses ngrams
* 
\**************************************************************************************************/
void iocFindSteckeredCharsNgram(IocResults* results, int maxNumOfSteckers, int ngramSize)
{

    Enigma* enigma;
    int     s;
    int     s1;
    int     s2;
    int     s1Max;
    int     s2Max;
    int     steckerTable[MAX_POSITIONS];
    float   maxIoc;
    float   ioc;
    int     found;
    char*   steckers;


    // Initialise stecker brett table: no steckers
    s1=0;
    while (s1<MAX_POSITIONS)
    {
        steckerTable[s1]=s1;
        s1++;
    }
  
    // Convert the steckers from the Enigma to the stecker table
    s=0;
    steckers=results->settings.steckers;

    s2=0;
    while (s2<strlen(steckers))
    {
        steckerTable[steckers[s2+0]-'A']=steckers[s2+1]-'A';
        steckerTable[steckers[s2+1]-'A']=steckers[s2+0]-'A';
        s++;
        s2+=3;
    }

    enigma=createEnigmaM3();
    
    setEnigma(enigma, &results->settings);
    
    maxIoc          =-1000000.0 ;

    found=1;
    while (s<maxNumOfSteckers && found)
    {
        s1Max=0;
        s2Max=0;
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
                        found=1;
                    }
                    // Remove stecker
                    steckerTable[s1]=s1;
                    steckerTable[s2]=s2;
                                    
                }
                s2++;
            }
            s1++;
            
        }
        if (!found)
        {
            printf("No improvement found\n");
        }
                   
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            printf("Found steckered chars %c-%c : ioc %f\n", s1Max+'A', s2Max+'A', maxIoc);
            fflush(stdout);
            steckerTable[s1Max]=s2Max;
            steckerTable[s2Max]=s1Max;
        }
        s++;
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));

    // Convert the stecker positions to a stecker brett string
    s1=0;
    s2=0;
    while (s1<MAX_POSITIONS)
    {
        if (steckerTable[s1]>s1)
        {
            if (s2>0)
            {
                results->settings.steckers[3*s2-1]=' ';
            }
            results->settings.steckers[3*s2]=s1+'A';
            results->settings.steckers[3*s2+1]=steckerTable[s1]+'A';
            results->settings.steckers[3*s2+2]='\0';
            s2++;
        }
        s1++;
    }
    
    destroyEnigma(enigma);
    fflush(stdout);
        
}


/*********************************************************************************3*****************\
* 
* After the rotor settings have been found, this method finds the steckers.
* It uses ngrams
* 
\**************************************************************************************************/
void iocFindSteckeredCharsNgram2(IocResults* results, int maxNumOfSteckers, int ngramSize)
{

    Enigma* enigma;
    int     s;
    int     s1;
    int     s2;
    int     s3;
    int     s1Swap;
    int     s2Swap;
    int     s3Swap;
    int     s1Max;
    int     s2Max;
    int     s3Max;
    int     s1SwapMax;
    int     s2SwapMax;
    int     s3SwapMax;
    int     steckerTable[MAX_POSITIONS];
    float   maxIoc;
    float   ioc;
    int     found;
    char*   steckers;


    // Initialise stecker brett table: no steckers
    s1=0;
    while (s1<MAX_POSITIONS)
    {
        steckerTable[s1]=s1;
        s1++;
    }
  
    // Convert the steckers from the Enigma to the stecker table
    s=0;
    steckers=results->settings.steckers;

    s2=0;
    while (s2<strlen(steckers))
    {
        steckerTable[steckers[s2+0]-'A']=steckers[s2+1]-'A';
        steckerTable[steckers[s2+1]-'A']=steckers[s2+0]-'A';
        s++;
        s2+=3;
    }


int i;
i=0;
while (i<26)
{
  printf("%c", steckerTable[i]+'A');
  i++;
}
printf("\n");
  
    enigma=createEnigmaM3();
    
    setEnigma(enigma, &results->settings);
    
    maxIoc          =-1000000.0 ;
    
    found=1;
    while (s<maxNumOfSteckers && found)
    {
        s1Max=-1;
        s2Max=-1;
        s3Max=-1;
        found=0;
        s1=0;
        while (s1<MAX_POSITIONS-1)
        {
            s2=/*s1+1*/0;
            while (s2<MAX_POSITIONS)
            {
                if ((s1!=s2) && (steckerTable[s1]==s1)/* && (steckerTable[s2]==s2)*/)
                {
                  if (steckerTable[s2]==s2)
                  {
                      // ***** MAIN COMBINATION *****
                      // Place stecker  
                      s1Swap=steckerTable[s1];
                      s2Swap=steckerTable[s2];
                      steckerTable[s1]=s2Swap;
                      steckerTable[s2]=s1Swap;
                      
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
                          s3Max=-1;
                          s1SwapMax=s2Swap;
                          s2SwapMax=s1Swap;
                          s3SwapMax=-1;
                          found=1;
                      }
                      // Remove stecker
                      steckerTable[s1]=s1Swap;
                      steckerTable[s2]=s2Swap;
                  }
                  else
                  {
                      s3=steckerTable[s2];

                      // ***** ADDITIONAL COMBINATION 1 *****
                      // Place stecker
                      s1Swap=steckerTable[s1];
                      s2Swap=steckerTable[s2];
                      s3Swap=steckerTable[s3];
                      steckerTable[s1]=s2Swap;
                      steckerTable[s2]=s3Swap;
                      steckerTable[s3]=s1Swap;
                      
                      // Quick and dirty way to set up the steckers
                      memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                      
                      setGrundStellung(enigma, 1, results->settings.grundStellungen[0]);
                      setGrundStellung(enigma, 2, results->settings.grundStellungen[1]);
                      setGrundStellung(enigma, 3, results->settings.grundStellungen[2]);
                      
                      encodeDecode(enigma);
                      ioc=ngramScore(enigma, ngramSize);

                      if (ioc>maxIoc)
                      {
                          printf("Found 231\n");
                          maxIoc=ioc;
                          s1Max=s1;
                          s2Max=s2;
                          s3Max=s3;
                          s1SwapMax=s2Swap;
                          s2SwapMax=s3Swap;
                          s3SwapMax=s1Swap;
                          found=1;
                      }
                      // Remove stecker
                      steckerTable[s1]=s1Swap;
                      steckerTable[s2]=s2Swap;
                      steckerTable[s3]=s3Swap;



                      // ***** ADDITIONAL COMBINATION 2 *****
                      // Place stecker
                      s1Swap=steckerTable[s1];
                      s2Swap=steckerTable[s2];
                      s3Swap=steckerTable[s3];
                      steckerTable[s1]=s3Swap;
                      steckerTable[s2]=s1Swap;
                      steckerTable[s3]=s2Swap;
                      
                      // Quick and dirty way to set up the steckers
                      memcpy(enigma->steckerBrett, steckerTable, MAX_POSITIONS*sizeof(int));
                      
                      setGrundStellung(enigma, 1, results->settings.grundStellungen[0]);
                      setGrundStellung(enigma, 2, results->settings.grundStellungen[1]);
                      setGrundStellung(enigma, 3, results->settings.grundStellungen[2]);
                      
                      encodeDecode(enigma);
                      ioc=ngramScore(enigma, ngramSize);

                      if (ioc>maxIoc)
                      {
                          printf("Found 312\n");
                          maxIoc=ioc;
                          s1Max=s1;
                          s2Max=s2;
                          s3Max=s3;
                          s1SwapMax=s3Swap;
                          s2SwapMax=s1Swap;
                          s3SwapMax=s2Swap;
                          found=1;
                      }
                      // Remove stecker
                      steckerTable[s1]=s1Swap;
                      steckerTable[s2]=s2Swap;
                      steckerTable[s3]=s3Swap;

                  }
                  
                }
                s2++;
            }
            s1++;
            
        }
                   
        // If improvement of ioc found, make the switch definitive
        if (found)
        {
            printf("Found steckered chars %c-%c-%c : ioc %f\n", s1Max+'A', s2Max+'A', s3Max>=0?s3Max+'A':'=', maxIoc);
            fflush(stdout);
            steckerTable[s1Max]=s1SwapMax;
            steckerTable[s2Max]=s2SwapMax;
            if (s3Max>=0)
            {
                steckerTable[s3Max]=s3SwapMax;
            }
        }
        else
        {
            printf("No improvement found\n");
        }
        s++;
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));

    // Convert the stecker positions to a stecker brett string
    s1=0;
    s2=0;
    while (s1<MAX_POSITIONS)
    {
        if (steckerTable[s1]>s1)
        {
            if (s2>0)
            {
                results->settings.steckers[3*s2-1]=' ';
            }
            results->settings.steckers[3*s2]=s1+'A';
            results->settings.steckers[3*s2+1]=steckerTable[s1]+'A';
            results->settings.steckers[3*s2+2]='\0';
            s2++;
        }
        s1++;
    }
    
    destroyEnigma(enigma);
    fflush(stdout);
        
}


/**************************************************************************************************\
* 
* This method is meant to try to find best stecker settings for each rotor setting. 
* 
\**************************************************************************************************/
void iocFindSteckeredCharsDeep(Enigma* enigma, IocResults* results, int g1, int g2, int g3, int maxSteckers)
{
    int     s;
    int     s1;
    int     s2;
    int     s1Max;
    int     s2Max;
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
    
    s=0;
    found=1;
    while (s<maxSteckers && found)
    {
        s1Max=0;
        s2Max=0;
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
                        found=1;
                    }
                    
                    // Remove stecker
                    steckerTable[s1]=s1;
                    steckerTable[s2]=s2;
                }
                s2++;
            }
            s1++;
            
        }
        if (found)
        {
            steckerTable[s1Max]=s2Max;
            steckerTable[s2Max]=s1Max;
        }
        s++;
    }     

    // Store the raw stecker table
    memcpy(results->steckerTable, steckerTable, MAX_POSITIONS*sizeof(int));
   
    results->indexOfCoincidence=maxIoc;
        
}


/**************************************************************************************************\
* 
* This method processes the indicated Waltzen permutations with UKW. It looks for the best 
* index of coincidence. The best solutions are stored in the Top 10. 
* For each rotor setting the steckers are tried, which makes this very slow.
* Best approach is to fix R2 to one setting, and try to find the best result. Then finalize 
* by trying all R2 for the found rotor setting.
* Works for short messages and >5 steckers
* 
\**************************************************************************************************/
void iocEvaluateEngimaSettingsDeep(IocWorkItem* work, int maxSteckers)
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
    int         startR2;
    int         endR2;
    int         startR3;
    int         endR3;
    char*       ukw;
    long        startTime;
    long        count;
    time_t      now;
    LinkedList* permutations;
    char*       cypher;
    int         maxCypherChars;


    // The work item
    cypher          =work->cypher;
    maxCypherChars  =work->maxCypherChars;
    permutations    =work->permutations;
    start           =work->startPermutation;
    end             =work->endPermutation;
    ukw             =work->ukw;
    startR2         =work->startR2;
    endR2           =work->endR2;
    startR3         =work->startR3;
    endR3           =work->endR3;
    
    
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
        printf("%s", ctime(&now)); 
        printf("Trying permutation %d: %s - %s %s %s - R2 %d-%d R3 %d-%d\n", 
                w,
                ukw,
                waltzen[permutation[0]], 
                waltzen[permutation[1]], 
                waltzen[permutation[2]],
                startR2, endR2, startR3, endR3);
        fflush(stdout);

        // The Ringstellung of the 1st Waltze has no meaning
        r1=1;

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

                            iocFindSteckeredCharsDeep(enigma, results, g1, g2, g3, maxSteckers);
                            
                            ioc=results->indexOfCoincidence;

                            if (ioc>iocMax)
                            {
                                results->indexOfCoincidence=ioc;
                                results->settings.numberOfRotors=3;
                                strncpy(results->settings.cypher, cypher, MAX_TEXT);
                                strncpy(results->settings.rotors[0], waltzen[permutation[0]], MAX_ROTOR_NAME);
                                strncpy(results->settings.rotors[1], waltzen[permutation[1]], MAX_ROTOR_NAME);
                                strncpy(results->settings.rotors[2], waltzen[permutation[2]], MAX_ROTOR_NAME);
                                strncpy(results->settings.ukw, ukw, MAX_ROTOR_NAME);
                                results->settings.ringStellungen[0]=r1;
                                results->settings.ringStellungen[1]=r2;
                                results->settings.ringStellungen[2]=r3;
                                results->settings.grundStellungen[0]=g1;
                                results->settings.grundStellungen[1]=g2;
                                results->settings.grundStellungen[2]=g3;
                        
                                pthread_mutex_lock(&iocMutex);
                                iocMax=iocStoreResults(results);
                                pthread_mutex_unlock(&iocMutex);
                                iocDumpTopTenResults(0);
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
    
    printf("Executed %ld decryptions, %ld per sec\n", count, count/(time(NULL)-startTime));

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
    method_t            method;
    int                 maxSteckers;
    int                 maxSteckersIoc;
    int                 ngramSize;
    
    
    
    params      =(IocThreadParams*)vargp;
    permutations=params->permutations;
    id          =(long)params->threadId;
    
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
        }
        else
        {
            done=1;
        }
        pthread_mutex_unlock(&iocMutex);     

        // PHASE 1
        // process the item
        if (!done)
        {
            printf("Thread %ld starting work item: %d-%d, %s, R2 %d-%d R3: %d-%d\n", 
                   id, item->startPermutation, item->endPermutation, item->ukw, 
                   item->startR2, item->endR2, 
                   item->startR3, item->endR3);
            fflush(stdout);
            switch (method)
            {
                case METHOD_IOC:
                  iocEvaluateEngimaSettings(item);
                  break;
                case METHOD_IOC_DEEP:
                  iocEvaluateEngimaSettingsDeep(item, maxSteckers);
                  break;
                case METHOD_IOC_NGRAM:
                  iocEvaluateEngimaSettingsDeep(item, maxSteckersIoc);
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
        
        switch (method)
        {
            case METHOD_IOC:
                // Now we have got the Top 10 best results, try to find the 
                // Stecker Positions for each of them (only for the not deep method)
                i=0;
                while (i<iocNumberOfResults)
                {
                    printf("Finding steckers for %d\n", i);
                    iocFindSteckeredChars(&iocTopTenResults[i], maxSteckers);
                    i++;
                }
              break;
            case METHOD_IOC_DEEP:
              break;
            case METHOD_IOC_NGRAM:
                i=0;
                while (i<iocNumberOfResults)
                {
                    iocFindSteckeredCharsNgram2(&iocTopTenResults[i], maxSteckers, ngramSize);
                    i++;
                }
              break;
        }
        
        // Show the final result 
        printf("FOUND SOLUTIONS: \n");
        iocDumpTopTenResults(1);

        if (permutations!=NULL)
        {
            destroyLinkedList(permutations);
        }
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
void setEvaluationMethod(method_t method, int maxSteckers, int maxSteckersIoc, int ngramSize, char* ngrams)
{
    evaluationMethod.method         =method;
    evaluationMethod.maxSteckers    =maxSteckers;
    evaluationMethod.maxSteckersIoc =maxSteckersIoc;
    evaluationMethod.ngramSize      =ngramSize;

    if (ngrams!=NULL)
    {
        strncpy(evaluationMethod.ngramSet, ngrams, 5);
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
    permutations=createRotorPermutations(3, 5);
    length=linkedListLength(permutations);
   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads*2;

    i=0;
    while (i<numOfThreads)
    {
        iocWorkItems[i*2].cypher                =cypher;
        iocWorkItems[i*2].permutations          =permutations;
        iocWorkItems[i*2].startPermutation      =i*length/numOfThreads;
        iocWorkItems[i*2].endPermutation        =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2].startR2               =1;
        iocWorkItems[i*2].endR2                 =MAX_POSITIONS;
        iocWorkItems[i*2].startR3               =1;
        iocWorkItems[i*2].endR3                 =MAX_POSITIONS;
        iocWorkItems[i*2].maxCypherChars        =MAX_TEXT;
        strncpy(iocWorkItems[i*2].ukw, "UKW B", MAX_ROTOR_NAME);

        
        iocWorkItems[i*2+1].cypher              =cypher;
        iocWorkItems[i*2+1].permutations        =permutations;
        iocWorkItems[i*2+1].startPermutation    =i*length/numOfThreads;
        iocWorkItems[i*2+1].endPermutation      =(i+1)*length/numOfThreads-1;
        iocWorkItems[i*2+1].startR2             =1;
        iocWorkItems[i*2+1].endR2               =MAX_POSITIONS;
        iocWorkItems[i*2+1].startR3             =1;
        iocWorkItems[i*2+1].endR3               =MAX_POSITIONS;
        iocWorkItems[i*2+1].maxCypherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i*2+1].ukw, "UKW C", MAX_ROTOR_NAME);
        i++;
    }

    iocExecuteWorkItems(numOfThreads, permutations);
}


/**************************************************************************************************\
* 
* Example from the original James Gillogly article
* 
\**************************************************************************************************/
void iocExample()
{
    EnigmaSettings* settings;
    
    settings=&iocExampleResults.settings;
    
    printf("\n");
    printf("#####################################################################################\n");
    printf("# INDEX OF COINCIDENCE METHOD EXAMPLE\n");
    printf("# Cypher                    : \n%s\n", iocExampleCypher);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], settings->rotors[1], settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");

    setEvaluationMethod(METHOD_IOC, 10, 10, 0, NULL);

    iocDecodeText(iocExampleCypher, 6);
}


/**************************************************************************************************\
* 
* Example to play with short messages
* 
\**************************************************************************************************/
void iocExampleDeep1()
{
    EnigmaSettings* settings;
    LinkedList*     permutations;
    
    settings=&iocExampleResults2.settings;
    
    printf("\n");
    printf("#####################################################################################\n");
    printf("# INDEX OF COINCIDENCE METHOD EXAMPLE\n");
    printf("# Cypher                    : \n%s\n", iocExampleCypher2);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], settings->rotors[1], settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");

    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 0, NULL);

    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cypher            =iocExampleCypher2;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =45;
    iocWorkItems[0].endPermutation    =45;
    iocWorkItems[0].startR2           =23;
    iocWorkItems[0].endR2             =23;
    iocWorkItems[0].startR3           =4;
    iocWorkItems[0].endR3             =4;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW B", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettingsDeep(&iocWorkItems[0], 10);

    iocDumpTopTenResults(1);

    destroyLinkedList(permutations);    
}

/**************************************************************************************************\
* 
* Example to play with short messages
* 
\**************************************************************************************************/
void iocExampleDeep2()
{
    EnigmaSettings* settings;
    LinkedList*     permutations;
    
    settings=&iocExampleResults3.settings;
    
    printf("\n");
    printf("#####################################################################################\n");
    printf("# INDEX OF COINCIDENCE METHOD EXAMPLE\n");
    printf("# Cypher                    : \n%s\n", iocExampleCypher3);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], settings->rotors[1], settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");

    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 0, NULL);


    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cypher            =iocExampleCypher3;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =40;
    iocWorkItems[0].endPermutation    =40;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettingsDeep(&iocWorkItems[0], 10);

    iocDumpTopTenResults(1);

    destroyLinkedList(permutations);    
}

/**************************************************************************************************\
* 
* Example to play with short messages
* 
\**************************************************************************************************/
void iocExampleNgram()
{
    EnigmaSettings* settings;
    LinkedList*     permutations;
    int             i;
    
    settings=&iocExampleResults3.settings;
    
    printf("\n");
    printf("#####################################################################################\n");
    printf("# INDEX OF COINCIDENCE METHOD EXAMPLE\n");
    printf("# Cypher                    : \n%s\n", iocExampleCypher3);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], settings->rotors[1], settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");

    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "DE");


    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cypher            =iocExampleCypher3;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =40;
    iocWorkItems[0].endPermutation    =40;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettingsDeep(&iocWorkItems[0], 6);

    i=0;
    while (i<iocNumberOfResults)
    {
        iocFindSteckeredCharsNgram2(&iocTopTenResults[i], 10, 3);
        i++;
    }

    iocDumpTopTenResults(1);

    destroyLinkedList(permutations);    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "DE");

    strcpy(iocExampleResults3Test.settings.cypher, iocExampleCypher3);

    printf("Steckers %s\n", iocExampleResults3Test.settings.steckers);

    iocFindSteckeredCharsNgram2(&iocExampleResults3Test, 10, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test.settings);
    
}


/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest2()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "GC");


    printf("Steckers %s\n", iocExampleResults3Test2.settings.steckers);

    iocFindSteckeredCharsNgram2(&iocExampleResults3Test2, 10, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test2.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test2.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest3()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "DE");


    printf("Steckers %s\n", iocExampleResults3Test3.settings.steckers);

    iocFindSteckeredCharsNgram2(&iocExampleResults3Test3, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test3.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test3.settings);
    
}