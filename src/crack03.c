#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "enigma.h"
#include "crack.h"
#include "toolbox.h"
#include "coincidence.h"
#include "ngramscore.h"

// GC6ZZBB

char textGC6ZZBB[]=
"CRSMDDACIGRLKUPAHWCYFGDSEPBHSYPYYDDNWUDMBRKT"
"GPDULRSDTRWWYWVLWKVRIBHHWFVCDXVIEREHLGSQSBIQU"
"DGACRUFWBMYRSPYGBKESJLNNFTVAGSOXBDIOXJACSJRSAV"
"VXDAWBIMEDLQSLQKLFHATDCUGFPOYSBKPBSFLRHUPXDMM"
"TTDPRKZCOCNZDSULIHEJGLCLVQWXVAZZPWMOOKPPFWDSQ"
"VW";

char replyGC6ZZBB[]=
"QLTP KMZR JFEK XOWP AZXP JJJQ LCIV DSVF UMCN UGRV TLXE EVYT "
"WDEF ZQAM ITJU SFQC RNMY HXVI FGRP HKBJ BEAP DPJW BYTI ILQM "
"AZIM WFNG LJJI SZXF QURM HWWV IKUJ VJHR WYJD KHVP YXCB FUOL "
"FCMJ WTXF MNJB RFON YOQT SWQG KETQ FGTN KUIW HSTC OAFJ KOHX "
"HYRI HNLZ EWJQ QBAB YIKU WXGC INOC VQPE BQJY XKUM WEDG BJPA "
"LDRU SFEL KUIH PRHD AEXH GKMA UMHI IKKI AHPS JUAB XMJK DJHD "
"JVQE UHOI IYYJ MIVK DUEB LVGR KJMH ECFY AYIK DNJW YDWJ STBZ "
"PIJQ MQAP NKWO OZEJ UKYV ZECN KAYG ADJH MBZA FGWS GXZY HIUY "
"MRJU VZOZ VMCH ZGIL JZGJ OBEJ HIXJ QXQE GMTD FVHX MAVD HDRO "
"HBKK ZNAH ICTD MLUX HEUL AYCP OSQT GDAP JMIX OMGG RIWR TPCQ "
"JUJT CJEW BKPY QHVE WURA KVMJ IRNP RCEE PARI UMCW THIL QLDZ "
"MRSD SRMA OQTL SUFV UYPD KGHT IGTL RMMB ZPVU KMCH UKZK UPRK "
"EVYJ IBWV ZCXV XNHM TZEF ZXVV PQBG TTJJ HMQT USCY FAGX XQOJ "
"JFNK QTXA XRLZ GDJE OMXN AJWH VXMS HIVU UAWG PDSZ KBHU KJVA "
"NRSL CDRU YEET MZAM BODU PCIW SEOH ZYMS TEAF NKDE XPPK MWUN "
"RCAV OKVS WQWO KSFB XANM MUUI AYFL WNMU AWGG OGAS BYER VJCL "
"PCBV NWAG MWEM UYJI MAQJ JGDA DNAW MPKK PFFF KXXP SLMU ZSLB "
"EQJI LYZZ AXLY STAA VIPO SFBW YBPO SFBH NQVX ICOK HFZV NKZG "
"FJLU TYUG FGGA GKPZ OAJD CRWI FZPK MCDT SVML UMPN KSKD GNYZ "
"PUJQ MEBC IPYA IPWT AGQB SEYL NHNU FDGG TVLJ WUYS SAWC ZMFA "
"OUVU WJQI GQFT ACHW ENNS HUNN RUKX WCOP WFLA PAUT ROCM EJVC "
"UZMH PJXW PEON LUWC EHXW DGTB OUKZ WRTY GYHV TBAF OVPE HTYY "
"KWML BGVI JIVM UEZY ZPGK RTMB XLIX BYWO PVQX ZBWY BDJG VLHA "
"VEPD OQZA PRTF ETCR ZUFU TKOO ZWTG POIU KJKQ PPIU KKQX HRZP "
"LVIN XQOC JGXN VRFW DNAD OVZW PBUQ VKYL THIC CXVM EPMQ TCGQ "
"NILD TBOX XDAA UZYA NLKW MLZN TRFM ZUEJ QRZF KDSO COFV YLUV "
"MIIW OOKA XLQE KRJG GDCD QLJM NGKD KBHM ZJZO PYKG HPNI MIDR "
"UXEM RGVF YNZD CTXK GMHU EYMG ZAUE IQOC DKHM IGUR NEVB XLJL "
"PYLG DEBA KZJX TTRR IZUF ZKBA QWQW BUXM GNLR VXEA WJTP AFWQ "
"HALG LSHF AFPQ MSAD GINE XECU DJDC BWEO EQJC WBNG LHIP XGJU "
"BWRY LYLY NABZ RUEW CKSU BWCY JGLY ZBFV FLAM EVRP TRAH WYKH "
"UFQF RMYX PJOH NVZA OVBF IGCI JONB KNVZ XMVN BSXA WVEE CXKW "
"WNOV VRRG SAJA ZEDT QMKC FSGL DWSD FJCN ZQEU FDGJ LPMF MAGC "
"BZEJ EMGG HRXE DBDE NGDJ ZVXZ TJNK TYJX UGHD HXNK ZBOW XJUC "
"IZNH HITG FEHA MFDQ XKSY WBQI LNXX MNRJ BHKZ PBVG QDPK MSGJ "
"XICN RGSP KQWM TBLO VSXQ OVII BLHZ CAUF KQSB AGHH NLAP RJJG "
"PEAI OULZ SNJV BPHI ERCW SGVR AXRV BIZG ALTM XRZL TCIC APAD "
"YSLB UTXV EBAC ZDXJ GGMR BAIX SIED PSAC VTFP TXOQ LHSE ZTRQ "
"ETYT LABB KDDX UCHE BWNF UMGU QJTD QNCP YLNW PFZF JZUN FGQJ "
"CYQL SXDQ KKNR JGPH FPKH DUPE WBJJ JSVC LBCS IJMJ VUDK UXRF "
"IQIU ZTNE NNHZ NWLO JDVP NYPY IRMO UUXE FYMP CROO SKKG AJOH "
"UPLX LHDA OHDQ EHYF TYFK ZNUH UNXA GDPN UNDE ZEFJ BXUK QCAP "
"MXFC NKQW KKCL YUES QJLS FMAC BYRD JUSJ IHDC MXDA RSAP DQGM "
"VJVQ VNYG TRVB KURG CEZQ TLMF TOTF SJOY XLAT ZHJA RLNM KYXX "
"AIJI WMWV RECN JDXC VEUZ LMNX IEVA RCVU EHAM PAHC RZUJ NTIJ "
"DAMA CDLJ CRIG NKXF GALB UAKH LCZW TMGQ DRTW UMKG SBZD GQGK "
"QZCU QQXE VDDX BLZP IRQM PUHT QNKC NAAK YQLK WSUE LLXD QBMU "
"MOLF XKEO ZZLB VXSF TPUL YPUZ GFJY TMUP XIVO UYAC HYVA SEBI "
"NORM VEPM JKXC FNUL BJUR DBDD KXVA DTGO UDDX TXZP ZNTJ FPIU "
"AUGS QSIA HSNB ZLBB WVSS CYIC SYAT XDUJ ZEMF DQIV UTKW CFJZ "
"KHEE QNJA WDAV TOEL FOKW BLXG SSFB FQAX MUGI JEXW KZPJ OSNB "
"ZJFX LZHP XAJF GLOH NBWP MPNC MBCI NDWL WODI SOZX TARG YCNM "
"HIUM TZHL GGOC OGWP NQTB VGCB CVJG VAWS IJNM KBZM XMBV YWQD "
"RMBJ HPIL KIMT WPQB NFQM KKUR ZEOY RZHE SYXH DWIL XMZH MKWA "
"ZEFB OWEY JSKJ SFHR MQIG MHYH MWPA QTXQ YVRC BNKC GHIT SQYH "
"SADF AJTI NESF MNJM WTIX CEXT GRKO GOOI ZIJG AYNF DQDZ XARQ "
"IORJ NYXH LECS WTFR GZVK WLUG TDBQ RREE SCNO BBVO LDBR VNKN "
"QOZR VOVZ DTKG OCSR YTYJ FLWT GLEI XFTZ ZHNC YKKB VLRZ CYEN "
"UKMX QSCF DDIK TODT CFIU TIVX QLVK WTKR GETZ XFRQ KTPH MXPE "
"LOSO QYQU TCBS MLKE BBTU EKML LBCQ EFWQ MLKD BWFH MBTU NGHG "
"CDSY VURJ ZSPO HYRU FETJ KZKL YVUK UVRU EZDW BYAO KPLR BNPH "
"KWTJ JUBL WXRL VODB NCHN ZKLG XZMP ZEQZ LNOR DSOL PFFB SIRL "
"FQSE URQW XHDS KLPX VFCV VEYK HLVU ZAYY LQAI XKJZ FPIY HKOA "
"AVAE AXVH JWTL ISBN USID NWYT IWXX CVYU NZPC TVHM UEGV CSIN "
"XYPA FTJF QEWE MBNX CRMV OOTC BUMU APMT FITW AQRG HUAW IHVC "
"JRZE MOQQ NEQU DLMC MUQW JENQ ZAFF R";


char text11[]       = "MCIKVFMLPDWBWMLQRVKEYPSFBVYHLSGSYAFZPXZCBFWPEUMWMBUM"
                      "GCUMKCZPQJEEXEEEWOSBXQJTHQEDNJMSJENPWYSCKWOVPMAYWNQ";


// R: 1 1 1 G: 1 17 12, SB: AZ BY CX DW EV FU GT HS IQ JR

char text11_test[]  = "QAVIMKGJZTXYFBCMVCONNTBZKLEZNBQMCLEUQXPYTYHEBVNCKOVACPK"
                      "GCWPWUVHUFWJFWTCNIJVULFBVADZRQWMLVDDQFRJAZUPSIXACSKJHLR"
                      "DQFMXPSZMKI";


EnigmaSettings  fourthProblemTest=
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
    "AZ BY CX DW EV FU GT HS IQ JR",
    "QAVIM KGJZT XYFBC MVCON NTBZK LEZNB QMCLE UQXPY TYHEB VNCKO VACPK "
    "GCWPW UVHUF WJFWT CNIJV ULFBV ADZRQ WMLVD DQFRJ AZUPS IXACS KJHLR "
    "DQFMX PSZMK I"
};


void theThirdProblem()
{
    int             i;
    LinkedList*     permutations;
    int				numOfThreads;
    
	
    numOfThreads=4;
	
    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // Three Waltzen out of I-V, R1 = R2 = R3 = 1, UKW B
    // We know it because it was encrypted on

    permutations=createRotorPermutations(3, 5);

	
	// STEP 1: INITIAL TRY: TRY ALL ROTOR POSTIONS
    // Start with 5 Wehrmacht rotors


    int length=linkedListLength(permutations);
   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads;

    i=0;
    while (i<numOfThreads)
    {
		iocWorkItems[i].cypher            =textGC6ZZBB;
        iocWorkItems[i].permutations      =permutations;
        iocWorkItems[i].startPermutation  =i*length/numOfThreads;
        iocWorkItems[i].endPermutation    =(i+1)*length/numOfThreads-1;
        iocWorkItems[i].startR2           =1;
        iocWorkItems[i].endR2             =1;
        iocWorkItems[i].startR3           =1;
        iocWorkItems[i].endR3             =1;
        iocWorkItems[i].maxCypherChars    =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);
        i++;
    }

    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "DE");

    iocExecuteWorkItems(numOfThreads, permutations);	
	
/*	
	// THIS RESULTS IN THE BEST SOLUTION:
	//  1: UKW B  II   V   I R  1  1 18 G 21  6 24 - AO BV DS EX FT HZ IQ JW KU PR - 0.071839

	// STEP 2: NOW TRY THIS ROTOR SETTINGS AND VARY ALL R2
   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads;

    i=0;
    while (i<numOfThreads)
    {
		iocWorkItems[i].cypher              =textGC6ZZBB;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =23;
        iocWorkItems[i].endPermutation      =23;
        iocWorkItems[i].startR2             =i*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].endR2               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].maxCypherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);

        i++;
    }

    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 3, "DE");
    iocExecuteWorkItems(numOfThreads, permutations);	
*/	
}



void theFourthProblem()
{
    int             i;
    LinkedList*     permutations;
    int             numOfThreads;
	
    numOfThreads=4;
	
    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // III I II, UKW B

    permutations=createRotorPermutations(3, 5);

   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads;

    i=0;
    while (i<numOfThreads)
    {
//        iocWorkItems[i].cypher              =text11;
        iocWorkItems[i].cypher              =text11_test;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =27;
        iocWorkItems[i].endPermutation      =27;
        iocWorkItems[i].startR2             =1;
        iocWorkItems[i].endR2               =1;
        iocWorkItems[i].startR3             =1;
        iocWorkItems[i].endR3               =1;
//        iocWorkItems[i].startR3             =i*(MAX_POSITIONS-1)/numOfThreads+1;
//        iocWorkItems[i].endR3               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].maxCypherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);

        i++;
    }

    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "GC");
    iocExecuteWorkItems(numOfThreads, permutations);	
	
}


