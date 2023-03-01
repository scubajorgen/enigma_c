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
"QLTPKMZRJFEKXOWPAZXPJJJQLCIVDSVFUMCNUGRVTLXEEVYT"
"WDEFZQAMITJUSFQCRNMYHXVIFGRPHKBJBEAPDPJWBYTIILQM"
"AZIMWFNGLJJISZXFQURMHWWVIKUJVJHRWYJDKHVPYXCBFUOL"
"FCMJWTXFMNJBRFONYOQTSWQGKETQFGTNKUIWHSTCOAFJKOHX"
"HYRIHNLZEWJQQBABYIKUWXGCINOCVQPEBQJYXKUMWEDGBJPA"
"LDRUSFELKUIHPRHDAEXHGKMAUMHIIKKIAHPSJUABXMJKDJHD"
"JVQEUHOIIYYJMIVKDUEBLVGRKJMHECFYAYIKDNJWYDWJSTBZ"
"PIJQMQAPNKWOOZEJUKYVZECNKAYGADJHMBZAFGWSGXZYHIUY"
"MRJUVZOZVMCHZGILJZGJOBEJHIXJQXQEGMTDFVHXMAVDHDRO"
"HBKKZNAHICTDMLUXHEULAYCPOSQTGDAPJMIXOMGGRIWRTPCQ"
"JUJTCJEWBKPYQHVEWURAKVMJIRNPRCEEPARIUMCWTHILQLDZ"
"MRSDSRMAOQTLSUFVUYPDKGHTIGTLRMMBZPVUKMCHUKZKUPRK"
"EVYJIBWVZCXVXNHMTZEFZXVVPQBGTTJJHMQTUSCYFAGXXQOJ"
"JFNKQTXAXRLZGDJEOMXNAJWHVXMSHIVUUAWGPDSZKBHUKJVA"
"NRSLCDRUYEETMZAMBODUPCIWSEOHZYMSTEAFNKDEXPPKMWUN"
"RCAVOKVSWQWOKSFBXANMMUUIAYFLWNMUAWGGOGASBYERVJCL"
"PCBVNWAGMWEMUYJIMAQJJGDADNAWMPKKPFFFKXXPSLMUZSLB"
"EQJILYZZAXLYSTAAVIPOSFBWYBPOSFBHNQVXICOKHFZVNKZG"
"FJLUTYUGFGGAGKPZOAJDCRWIFZPKMCDTSVMLUMPNKSKDGNYZ"
"PUJQMEBCIPYAIPWTAGQBSEYLNHNUFDGGTVLJWUYSSAWCZMFA"
"OUVUWJQIGQFTACHWENNSHUNNRUKXWCOPWFLAPAUTROCMEJVC"
"UZMHPJXWPEONLUWCEHXWDGTBOUKZWRTYGYHVTBAFOVPEHTYY"
"KWMLBGVIJIVMUEZYZPGKRTMBXLIXBYWOPVQXZBWYBDJGVLHA"
"VEPDOQZAPRTFETCRZUFUTKOOZWTGPOIUKJKQPPIUKKQXHRZP"
"LVINXQOCJGXNVRFWDNADOVZWPBUQVKYLTHICCXVMEPMQTCGQ"
"NILDTBOXXDAAUZYANLKWMLZNTRFMZUEJQRZFKDSOCOFVYLUV"
"MIIWOOKAXLQEKRJGGDCDQLJMNGKDKBHMZJZOPYKGHPNIMIDR"
"UXEMRGVFYNZDCTXKGMHUEYMGZAUEIQOCDKHMIGURNEVBXLJL"
"PYLGDEBAKZJXTTRRIZUFZKBAQWQWBUXMGNLRVXEAWJTPAFWQ"
"HALGLSHFAFPQMSADGINEXECUDJDCBWEOEQJCWBNGLHIPXGJU"
"BWRYLYLYNABZRUEWCKSUBWCYJGLYZBFVFLAMEVRPTRAHWYKH"
"UFQFRMYXPJOHNVZAOVBFIGCIJONBKNVZXMVNBSXAWVEECXKW"
"WNOVVRRGSAJAZEDTQMKCFSGLDWSDFJCNZQEUFDGJLPMFMAGC"
"BZEJEMGGHRXEDBDENGDJZVXZTJNKTYJXUGHDHXNKZBOWXJUC"
"IZNHHITGFEHAMFDQXKSYWBQILNXXMNRJBHKZPBVGQDPKMSGJ"
"XICNRGSPKQWMTBLOVSXQOVIIBLHZCAUFKQSBAGHHNLAPRJJG"
"PEAIOULZSNJVBPHIERCWSGVRAXRVBIZGALTMXRZLTCICAPAD"
"YSLBUTXVEBACZDXJGGMRBAIXSIEDPSACVTFPTXOQLHSEZTRQ"
"ETYTLABBKDDXUCHEBWNFUMGUQJTDQNCPYLNWPFZFJZUNFGQJ"
"CYQLSXDQKKNRJGPHFPKHDUPEWBJJJSVCLBCSIJMJVUDKUXRF"
"IQIUZTNENNHZNWLOJDVPNYPYIRMOUUXEFYMPCROOSKKGAJOH"
"UPLXLHDAOHDQEHYFTYFKZNUHUNXAGDPNUNDEZEFJBXUKQCAP"
"MXFCNKQWKKCLYUESQJLSFMACBYRDJUSJIHDCMXDARSAPDQGM"
"VJVQVNYGTRVBKURGCEZQTLMFTOTFSJOYXLATZHJARLNMKYXX"
"AIJIWMWVRECNJDXCVEUZLMNXIEVARCVUEHAMPAHCRZUJNTIJ"
"DAMACDLJCRIGNKXFGALBUAKHLCZWTMGQDRTWUMKGSBZDGQGK"
"QZCUQQXEVDDXBLZPIRQMPUHTQNKCNAAKYQLKWSUELLXDQBMU"
"MOLFXKEOZZLBVXSFTPULYPUZGFJYTMUPXIVOUYACHYVASEBI"
"NORMVEPMJKXCFNULBJURDBDDKXVADTGOUDDXTXZPZNTJFPIU"
"AUGSQSIAHSNBZLBBWVSSCYICSYATXDUJZEMFDQIVUTKWCFJZ"
"KHEEQNJAWDAVTOELFOKWBLXGSSFBFQAXMUGIJEXWKZPJOSNB"
"ZJFXLZHPXAJFGLOHNBWPMPNCMBCINDWLWODISOZXTARGYCNM"
"HIUMTZHLGGOCOGWPNQTBVGCBCVJGVAWSIJNMKBZMXMBVYWQD"
"RMBJHPILKIMTWPQBNFQMKKURZEOYRZHESYXHDWILXMZHMKWA"
"ZEFBOWEYJSKJSFHRMQIGMHYHMWPAQTXQYVRCBNKCGHITSQYH"
"SADFAJTINESFMNJMWTIXCEXTGRKOGOOIZIJGAYNFDQDZXARQ"
"IORJNYXHLECSWTFRGZVKWLUGTDBQRREESCNOBBVOLDBRVNKN"
"QOZRVOVZDTKGOCSRYTYJFLWTGLEIXFTZZHNCYKKBVLRZCYEN"
"UKMXQSCFDDIKTODTCFIUTIVXQLVKWTKRGETZXFRQKTPHMXPE"
"LOSOQYQUTCBSMLKEBBTUEKMLLBCQEFWQMLKDBWFHMBTUNGHG"
"CDSYVURJZSPOHYRUFETJKZKLYVUKUVRUEZDWBYAOKPLRBNPH"
"KWTJJUBLWXRLVODBNCHNZKLGXZMPZEQZLNORDSOLPFFBSIRL"
"FQSEURQWXHDSKLPXVFCVVEYKHLVUZAYYLQAIXKJZFPIYHKOA"
"AVAEAXVHJWTLISBNUSIDNWYTIWXXCVYUNZPCTVHMUEGVCSIN"
"XYPAFTJFQEWEMBNXCRMVOOTCBUMUAPMTFITWAQRGHUAWIHVC"
"JRZEMOQQNEQUDLMCMUQWJENQZAFFR";



EnigmaSettings  GCXQHWTest=
{
    3,
    {
        "IV",
        "III",
        "V"
    },
    "UKW B",
    {
        1, 1, 1
    },
    {
        1, 1, 1 // unknown
    },
    "",
    "IFKIN OXPBU ELULE UEHNF KVJCW"
    "XCTWM AFEFZ VGUVA CTASQ CYIAM"
    "GEBFH DXRWK UOFNY JJWPI FUESJ"
    "JGYUR IRAQI XQAPP QQ"
};




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


char fifth[]=
"joelhLeibTtAdneIemeIrDsEtleOlraeaMgigPeSkurMadaOkltOgresbirT"
"uAimkEdteccohnescekCeTrEoTmusrtAedeidpsiesecnIhnignEtLvioTos"
"reddedvOoElIguesnMdOedltaeaMgPtOervieNrckirDiIjdgUeNntxuzTil"
"lavbeOrrvEleotodtoxLlOORReEMmAigPNsAUamLdIOqLUOArustIEtnAImm"
"eatDCMOiNnsieMCVteeNTIuarmAqDUiiPsinsOcsITnrguEDleixtEsREcDI"
"DToaEtiIuOsNMUoLdltAemmcpoOLRAiBnOCriIdsiNDiusNITUutTAllaIbq"
"ouRiepeETXdeOalcOorMemmOaDGoNCaOanlsieqqUUaAuTTDEuniIsmaaUDT"
"mEiiNriUmRVeedNoILaOmRqIUNirsEnPORseTHReUndDeExREirtcIIntVao"
"tLiUoPNTUaltlEAVmecloILTaeBSOSreICsInliLsuImuDtOAlloIrqEUeiU"
"pfEUxgEiaacToNmumLoldaOpCaoRNisAEtQuUrAETXdCUEipstAEUUtresII"
"RnUtrOecDCoAlEOCrAitncRuePPiRDeahTEantdneoRnIptRIonIVDOElNut"
"pstuaNtteivNeCluiltPEasQsueiCOiFlFLIUcMIdaODLeOSrEeReUUNftum"
"goilALTinTualnliampIadReiLaoTruErmEixPcsEUpMTDeoUlrOsRIsnItT"
"OacMcEatECcOaNtsCEUcpTieDtaUtrAatDniopNiPsrCOIInDGEenlTIsTUs"
"neTDIDNoceUIlUpsAmqoUDiToeFmfPiocRIiandceiSDEIrDUunnttmuoTLL"
"laibTOarneIEmTidDoeLLOorReEmMAiGpnsAUamLdioqlUoarUstieTnaImM"
"eAtDcmoInnsIEmCvTEenTIuARMaqduiipsiNsoCsiTnrGUEdLEiXtesrecDI"
"dTOaetIiUoSNMUoldltAEMmcPooLraiBnOcriIdsindIuSNitUuTTallaIBq"
"oUriEPEeTXdeOalcOORMeMMoAdgoNcAoaNlSieQquUaAuttdeuNiiSMaaUDT"
"MeiINRiuMRVeeDNoILaOMRqiuNIrSeNpORsEthReunDDeeXreirtCIinTVAo"
"tLIUOPntUalTleAVMEClOiLTaebsOSrEiCsINlILsUimUDtOalloIRqeueiu"
"PFEUxgeIaactoNMUMLolDaoPcAORniSaetQuuRaetXdCueIpStAeuuTREsiI"
"rnUTroeCdCoaleOcRaiTNCrueppIrDeAhteantdneoRnIpTrIoNivDOeLNuT"
"PsTUantTeIVneCLuILTPeASqSUeiCOiFLfLiuCmiDaoDlEosReEREuuNfTUm"
"goilaltinTuaLNLIAmPiADReilaOtRueRMeIXpCsEUpmTdeOUlrosrISNiTt"
"OacmCEatECCOAntSceucpTiEdtautRaATDNIOpNipsRcoiiNdgeeNltIstUs"
"neTDidNOCeUIlupsAmQoUDitOeFmFPIocRIianDcEisDeIRdUUnnttMuotlL"
"LaiBTOArnEieMTiDDoEllOORrEEmmaIgPnsAuAmldiOQLuOAruSTieTnAiMm"
"EatDcMoiNnsiemcVTEEntiUArmAQduiIpsINsocsitNrGUEdLeixtEsreCdi"
"DToaEtiiUosNmuOlDlTAEmmCPoolRaIbnOcRIiDSindiusniTUUttAlLAiBQ"
"ouRIEpEEtxdeoalCOormemmOAdgOncaOanlsIeQqUUaaUttdEUniISmAauDt"
"meiinRiuMRVeeDNOilAoMrqiUniRSENpoRsEThrEUNdDeexrEirtcIiNTvAO"
"TliuOpntuALtlEAVmecLoiLTAeBSosreIcsINlILsuiMUdToAllOIrQeUeiU"
"pfEUxGEIAactONMUmlOldaOPCAoRNiSaeTQUurAETxdCUeipStaEUUTrESii"
"RnutrOecdCoaLeOcRaItNCrUEpPIrDEahtEANtdneoRnIpTRiONIvDoeLNut"
"pSTuanttEIvNeCluilTPeaSqsueIcoiflflIUcmiDAodleoSREeREUUnFTuM"
"GoiLALTintuAlNLIampIadrEIlAOTRUerMeIxPCsEuPmtDeoUlrosrISNiTt"
"OAcmceATeccOaNtscEucptiEdtautraATDNIOpniPSrCOIIndGEEnltIStUs"
"NEtDidNOcEUIlUpsaMqouditOeFMFpioCrIIAnDCEiSdeirDuunNtTM";


void theThirdProblem()
{
    int             i;
    LinkedList*     permutations;
    int             numOfThreads;
    
	
    numOfThreads=4;
	
    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // (http://enigmaco.de/enigma/enigma_de.html was used to encrypt)
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
//        iocWorkItems[i].cypher            =textGC6ZZBB;
        iocWorkItems[i].cypher            =replyGC6ZZBB;
        iocWorkItems[i].permutations      =permutations;
        iocWorkItems[i].startPermutation  =i*length/numOfThreads;
        iocWorkItems[i].endPermutation    =(i+1)*length/numOfThreads-1;
        iocWorkItems[i].R1                =1;
        iocWorkItems[i].startR2           =1;
        iocWorkItems[i].endR2             =1;
        iocWorkItems[i].startR3           =1;
        iocWorkItems[i].endR3             =1;
        iocWorkItems[i].maxCypherChars    =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);
        i++;
    }


    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 0, 3, "DE");

    iocExecuteWorkItems(numOfThreads, permutations);	

/*
    numOfThreads=1;
	
    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // (http://enigmaco.de/enigma/enigma_de.html was used to encrypt)
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
        iocWorkItems[i].startPermutation  =47;
        iocWorkItems[i].endPermutation    =47;
        iocWorkItems[i].R1                =1;
        iocWorkItems[i].startR2           =1;
        iocWorkItems[i].endR2             =1;
        iocWorkItems[i].startR3           =1;
        iocWorkItems[i].endR3             =1;
        iocWorkItems[i].maxCypherChars    =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);
        i++;
    }


    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 13, 3, "DE");

    iocExecuteWorkItems(numOfThreads, permutations);	
*/

    
	
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
	
    numOfThreads=1;
	
    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // III I II, UKW B, 

    permutations=createRotorPermutations(3, 5);

   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads;

    i=0;
    while (i<numOfThreads)
    {
        iocWorkItems[i].cypher              =text11;
//        iocWorkItems[i].cypher              =text11_test;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =27;
        iocWorkItems[i].endPermutation      =27;
        iocWorkItems[i].R1                  =13;
        iocWorkItems[i].startR2             =5;
        iocWorkItems[i].endR2               =5;
        iocWorkItems[i].startR3             =20;
        iocWorkItems[i].endR3               =20;
//        iocWorkItems[i].startR3             =i*(MAX_POSITIONS-1)/numOfThreads+1;
//        iocWorkItems[i].endR3               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].maxCypherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);

        i++;
    }

    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 10, 3, "GC");
    iocExecuteWorkItems(numOfThreads, permutations);	
	
}


void theFifthProblem()
{
    int i, j;
    int count;
    int value;
    char* level01;
    char* level02;
    
    int length;
    
    length=strlen(fifth);
    printf("Length %d\n", length);
    
    level01=malloc(length);
    i=0;
    while (i<length/2+1)
    {
      level01[i]=fifth[2*i];
      i++;
    }
    i=0;
    while (i<length/2+1)
    {
      level01[length/2+i+1]=fifth[i*2+1];
      i++;
    }
    i=0;
    while(i<length)
    {    
        printf("%c", level01[i]);
        i++;
    }
    printf("\n\n");


    level02=malloc(length-100);
    i=100;
    count=0;
    j=0;
    value=0;
    while(i<length)
    {
      value<<=1;
      if (level01[i]>='a' && level01[i]<='z')
      {
          
      }
      else
      {
          value|=0x01;
      }
      j++;
      if (j==5)
      {
//          printf("%02d ", value);
          printf("%c", 'A'+value);
          level02[count]='A'+value;
          count++;
          j=0;
          value=0;
      } 
      
      i++;
    }
    printf("\n\n");

    i=72;
    while (i<length-100)
    {
        i++;
    }


    free(level01);
}







