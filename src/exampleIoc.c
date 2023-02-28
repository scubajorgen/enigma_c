/**************************************************************************************************\
* 
*
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

// GC voorbeeld
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

        "QAVIM KGJZT XYFBC MVCON NTBZK LEZNB QMCLE UQXPY TYHEB VNCKO VACPK "
        "GCWPW UVHUF WJFWT CNIJV ULFBV ADZRQ WMLVD DQFRJ AZUPS IXACS KJHLR "
        "DQFMX PSZMK I"
    }
};

// GC GC6ZZBB
IocResults  iocExampleResults3Test3=
{
    0.0,
    {
        3,
        {
            "IV",
            "V",
            "II"
        },
        "UKW B",
        {
            1, 1, 1
        },
        {
            10, 21, 24
        },

//        "",
        "AD BL EW FI HZ JR QU",
        
        "CRSMDDACIGRLKUPAHWCYFGDSEPBHSYPYYDDNWUDMBRKT"
        "GPDULRSDTRWWYWVLWKVRIBHHWFVCDXVIEREHLGSQSBIQU"
        "DGACRUFWBMYRSPYGBKESJLNNFTVAGSOXBDIOXJACSJRSAV"
        "VXDAWBIMEDLQSLQKLFHATDCUGFPOYSBKPBSFLRHUPXDMM"
        "TTDPRKZCOCNZDSULIHEJGLCLVQWXVAZZPWMOOKPPFWDSQ"
        "VW"
    }
};


// GC Code breakers
IocResults  iocExampleResults3Test4=
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
            13, 5, 20
        },
        {
            2, 1, 8
        },

        "",
        
        "MCIKVFMLPDWBWMLQRVKEYPSFBVYHLSGSYAFZPXZCBFWPEUMWMBUMGCU"
        "MKCZPQJEEXEEEWOSBXQJTHQEDNJMSJENPWYSCKWOVPMAYWNQ"
    }
};


// GC GC6ZZBB
IocResults  iocExampleResults3Test5=
{
    0.0,
    {
        3,
        {
            "II",
            "III",
            "IV"
        },
        "UKW B",
        {
            1, 1, 1
        },
        {
            21, 9, 15
        },

//        "",
        "",
        
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
"JRZEMOQQNEQUDLMCMUQWJENQZAFFR"


    }
};



/**************************************************************************************************\
* 
* Print cypher nicely formated
* 
\**************************************************************************************************/
void printCypher(char* cypher)
{
  int i;
  int printed;
  
  printf("# ");
  i=0;
  printed=0;
  while(i<strlen(cypher))
  {
      if (cypher[i]!=' ')
      {
          printf("%c", cypher[i]);
          printed++;

      if (printed%5==0)
      {
          if (printed==50)
          {
              printf("\n# ");
              printed=0;
          }
          else
          {
            printf(" ");
          }
      }
      }
      i++;
  }
  if (printed!=0)
  {
    printf("\n");
  }
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
    printf("# INDEX OF COINCIDENCE METHOD EXAMPLE - James Gillogly\n");
    printf("# Cypher                    : \n");
    printCypher(iocExampleCypher);
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
    printf("# Cypher                    : \n");
    printCypher(iocExampleCypher2);
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
    iocWorkItems[0].R1                =1;
    iocWorkItems[0].startR2           =23;
    iocWorkItems[0].endR2             =23;
    iocWorkItems[0].startR3           =4;
    iocWorkItems[0].endR3             =4;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW B", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettings(&iocWorkItems[0], 10);

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
    printf("# Cypher                    : \n");
    printCypher(iocExampleCypher3);
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
    iocWorkItems[0].R1                =1;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettings(&iocWorkItems[0], 10);

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
    printf("# INDEX OF COINCIDENCE+NGRAM METHOD EXAMPLE\n");
    printf("# Cypher                    : \n");
    printCypher(iocExampleCypher3);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], 
                                                       settings->rotors[1], 
                                                       settings->rotors[2]);
    printf("# Original UKW              : %s \n", settings->ukw);
    printf("# Original RingStellungen   : %d %d %d\n", settings->ringStellungen[0], 
                                                       settings->ringStellungen[1], 
                                                       settings->ringStellungen[2]);
    printf("# Original GrundStellungen  : %d %d %d\n", settings->grundStellungen[0],
                                                       settings->grundStellungen[1],
                                                       settings->grundStellungen[2]);
    printf("# Original Steckers         : %s\n", settings->steckers);
    printf("#####################################################################################\n");

    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 5, 3, "DE");


    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cypher            =iocExampleCypher3;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =40;
    iocWorkItems[0].endPermutation    =40;
    iocWorkItems[0].R1                =7;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCypherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettings(&iocWorkItems[0], 5);

    iocDumpTopTenResults(1);

    i=0;
    while (i<iocNumberOfResults)
    {
        iocFindSteckeredCharsNgram(&iocTopTenResults[i], 10, 3);
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
    EnigmaSettings* settings;
    
    settings=&iocExampleResults3.settings;
    printf("\n");
    printf("#####################################################################################\n");
    printf("# STECKER FINDING WITH NGRAM METHOD EXAMPLE\n");
    printf("# Cypher                    : \n");
    printCypher(iocExampleCypher3);
    printf("# Original Waltzen          : %s %s %s\n", settings->rotors[0], 
                                                       settings->rotors[1], 
                                                       settings->rotors[2]);
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

    strcpy(iocExampleResults3.settings.cypher, iocExampleCypher3);

    strncpy(iocExampleResults3.settings.steckers, "AT BG DV EW FR HN IQ JX KZ LU PS", 35);
    printf("\nSteckers %s\n", iocExampleResults3.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults3, 13, 3);
    printf("Steckers %s\n", iocExampleResults3.settings.steckers);
    dumpDecoded(&iocExampleResults3.settings);
    
    strncpy(iocExampleResults3.settings.steckers, "AT DV JX KZ QR IP", 35);
    printf("\nSteckers %s\n", iocExampleResults3.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults3, 13, 3);
    printf("Steckers %s\n", iocExampleResults3.settings.steckers);
    dumpDecoded(&iocExampleResults3.settings);
    
    strncpy(iocExampleResults3.settings.steckers, "AT", 35);
    printf("\nSteckers %s\n", iocExampleResults3.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults3, 13, 3);
    printf("Steckers %s\n", iocExampleResults3.settings.steckers);
    dumpDecoded(&iocExampleResults3.settings);
    
    strncpy(iocExampleResults3.settings.steckers, "", 35);
    printf("\nSteckers %s\n", iocExampleResults3.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults3, 13, 3);
    printf("Steckers %s\n", iocExampleResults3.settings.steckers);
    dumpDecoded(&iocExampleResults3.settings);
    
    
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

    iocFindSteckeredCharsNgram(&iocExampleResults3Test2, 10, 3);
    
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
    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "GC");


    printf("Steckers %s\n", iocExampleResults3Test3.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults3Test3, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test3.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test3.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest4()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 10, 3, "GC");


    printf("Steckers %s\n", iocExampleResults3Test4.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults3Test4, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test4.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test4.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest5()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 3, 3, "DE");


    printf("Steckers %s\n", iocExampleResults3Test5.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults3Test5, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults3Test5.settings.steckers);
    
    dumpDecoded(&iocExampleResults3Test5.settings);
    
}
