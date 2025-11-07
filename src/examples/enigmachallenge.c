/**************************************************************************************************\
* enigmachallenge.c
* 
* Enigma challenge (see /documents/engimaChallenge.pdf) or 
* https://www.ciphermachinesandcryptology.com/en/challenge.htm
* 
* The challenge consists of decryption of 10 messages, from easy to hard. This file shows
* how the software is used for the decryption.
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "log.h"
#include "enigma.h"
#include "crack.h"
#include "toolbox.h"
#include "coincidence.h"
#include "workDispatcher.h"

char text01[]=
"MUUQ JZVQ LORV MCOL YKXE"
"PMCD CWGH NTQV MEHG ECOE"
"ULBU LBOC ZPGB IXIF WCYX"
"ZKZK LYAE VCJD GXJZ QKQG"
"VXSO RRQN ZMAT PZDO EXIT"
"XFIU VJFI ZUAY LIJW VVGF"
"YXGR DQKA GUUW BNUU OUXQ"
"QUCX KUXP TYUI IXPA YXRL"
"TZPZ QRNL OPAO DDUS VFWM"
"ILZE OBVO PIPW HXVY ADCO"
"RXPI IEUZ VTXB RJRE CTGL"
"CPKQ AJDA MI"; 

char text02_1[]=  
" RGOXT IVDHR FWGIK PUKXK CTISB"
" GRIKM MYAAA SMQFH ZGLBU ZGOFG"
" MGOJF BOPYU KMPPX VGIBT FYGKO"
" LISPX CLRRE QXOUQ OMXIJ OKWYQ"
" NHHQX SYCOJ QGYDU NTPMI LYDOO"
" BPLFV JQTIZ FHHUW GXACF TTHGI"
" SJHXS RWOPP CCMUA CJFNV VUBGV"
" QFDET VS";

char text02_2[]=  
" NGWTV VGTHG ZPQGF EXSWN FKSXH"
" POJCM PGHLL CAJCP UVIQU UZLQW"
" AVOTS CPZFA BNUOR ZZAFM CJPGP"
" TGTDR GWTZA XNQRY JCBMQ ZVCQU"
" CQOCO JGNYN UCAVO MA";

char text03[]=   
" ZJTPL TJNET NLLGO PQVSW XSRHC"
" OSHUT FGUSH HTVPO UMBMV GKLAA"
" FDUBN UVCUV POCFJ XDMIQ CCAUC"
" BQOKP HUMCI ZAJVI QESVG CFHDT"
" ISREH FCMBP JCRTW TTMXC NOIEU"
" WRPOM CEMSU NBBCT WZZRB LFLUF"
" IFBNY OYJGX UMNKP TCQHT GVYWS"
" QDFFM SWVEC IDWIL ZBYLI PRXYI"
" CFCLP DQZNO ZWSKV NJURT GKMWU"
" NFPNL EPOFQ LJMED EFNML RRRRJ"
" YTBVR KBQQG SUWVA WAFUU WFLMP"
" KPHLD ML";


char text04[]=
"KGBJN TWBQY FFJWQ KKCTN ZJVRK"  
"BWPQO FZQTB LCYCM WCWTR XSGKA"
"WIZEZ KFIWC KPEYB OBUBW VUHBO"
"BKEGF WGGSQ WUMIO BKHSF TXAGY"
"XPKAX AOJQJ ANZKZ REKYX TXWWR"
"HJHST EJAJS QFZMZ FLTSE QXBAZ"
"WDSJR WHVGF KIXLM PUYIN NQSAW"
"QHXAJ QJCGU CQUFI HWAFN AAFPR"
"ZSMTR KYLUG AOZKY NMXFC HQQEV"
"MTTIN CHTSW CYCRZ FBKMB VSHEK"
"XDYCY PWSZJ WVZAK IRMSQ DZKTF"
"DDEUX WKXMN PDMKD RKASA ORATL"
"JAEHW INMVR SWASF";

char text05[]=
"ACINZ CRVFJ RPETN UGAVD ZIHXF"
"WTPKK TTVXZ JTAYN XRQMA JKKFI"
"WUXTN HFCMZ PUUYP PKILQ YBYRD"
"MHFBM HPGYL CEIJU NMWGQ OKGHM"
"DLGJW BQBZB VWDTS UWHGX ZRFUX"
"WQTHT RHYPT PJZRO FTCNM XCKSD"
"DNHIW YSUGQ RZUIY IUOTD ZPQRT"
"CXVQX PMZGJ BWLHX ULWLU NLWPW"
"NTONB QUFMM IKNTV WNK";

char text06_1[]=
"OLDTO GVQQY DCOZN NVUMD FJIOC"
"JVaAZ MZJRE GOBDI VNYRF TUAZS"
"RUDPO HWZRL LKHMC CJUKR YRJMR"
"TXMGA UJMKG IENYA ZKXSS OTGWO"
"DHUQP YILYG CSJHV QNILS ITWQY"
"XBFNA GTSWM XJTSM OTVMG HVDZB"
"SQBDL SHaaE ZUHMP GGGMX HIJFY"
"IHWBJ WJAKO TCOKO VCPNR QJPKN"
"IRPYP TFWPB MUPHO HPIBR KRQNA"
"OYHHS BYPBB XPKAF aJIFX GOUPZ";

char text06_2[]=
"DQHFX QVZSX RJJYJ KKGWU QaaIU"
"LIXBO EMFQX AALQX CUYKO KPPVV"
"SGVBG RUAER DDOFA OWCGP PCUSW"
"IQQKK EHGXO CVYFQ TZICT JKOOF"
"QQJLH DXMDX TKCTC IMIaH YKJQQ" 
"IDEGK RAXQT UELIS PWGJC OGHES"
"ZBILO UISLS OKVIO LHZSR HEXUJ"
"AYITW DQNKO HVERX WNMJZ TBSBP"
"SMMCM TGaaa UIZKN VDWDD VEHHN"
"EUPCN GJDLN DOZXS VZBYD UPCTI";

char text06_3[]=
"DQCaa aTHQP LAEIS ZHDJE OFQVA"
"AXXWN GUTIM MXSNA ORBRO WKDTH"
"MOCUI MZJNZ SWTLU OYCAQ XAKGD"
"AMVWS GFYMF PFJXH GCHAS LLHIM"
"OMSZE ZBRLL JM";

char text07_1[]=
 "MLETW NNJZJ ECTHJ JUUVD DBAZS"
 "XBVOF SEWLR MLSBX GNLNL LLIGO"
 "IMYMU HBGRI HZTFG TXKCT CRPJN"
 "BHZMQ QWGUA KBJXJ BITQI BFXMZ"
 "MHHWK OBSSD OGELQ UWQXG IMLVG"
 "YJQSL PIQNZ VQVFN LGQXN PUJEZ"
 "MPUNC MLETS JIXZP HUNBN AGVCB"
 "SAAIN IIHJV CWISA NHEMW JOEIY"
 "MXSFE JAEPU FJLIT VUZYS HYWYP"
 "XTAYJ CARWI YORFI VANOM BXWDT";
 
char text07_2[]= 
 "EWGYY LDFBK IBHZA CZZZP ABUCX"
 "BLLZF KPVAX ZDJXJ GGZZQ RWURS"
 "LFLBV KAQTJ PMUXH NIPUC IWSFY"
 "ROEIQ QHNFO TPVAF RFNLQ OAXRE"
 "GZPJR WYYIL BGZSP PBVTL BRIGO"
 "QXION BZSZX ANZES UJTEN ZVWRS"
 "VRJPP OZLWV IMCKO YQFWY FTCPT"
 "ZSERJ DBPTA UUJGM ";

char text08_1[]=
 "SIAZK QGEML IVDBI YWAKC AMPYK"
 "CFLOP QDCWP VMITC WAYWK BRUJA"
 "VGRYY CISIJ ZSGRM TZEKG EQLWU"
 "XIXYP MQLUH ODQFP NRKBZ DISWX"
 "PHYDB NEQHJ UZJRZ FWWMV TGIXF"
 "SFCQI BVMHG ENWKN KYXMQ RYSMA"
 "WCMBW FHYPN WJEBV YBZEZ RCUFZ"
 "YLIFF JCQFK GOGBY GXMDJ LUJMM"
 "KZDLN NNJIY EAOYU VDFRF CCUVP"
 "WYPJH WFSGG RLXQD FFOKL SKGXZ";
 
char text08_2[]=
 "YNDXI HNTJY ETDDJ VBPCA PORBP"
 "PASUK HYHTH ETMFG JNPUF WAMEB"
 "FIKQB ZGGFZ ZXJMU YNJDW XJXZD"
 "MEEVP YRDGP YMAXW TWHUG DQZTM"
 "JWKYQ RDQXK VGTZY IIMPB VDJPQ"
 "VJLOI OSXQE NZZHC NTWCQ YQYMH"
 "COXPN TDXMT ZWABT WRVYI GMJEI"
 "CMHXH HEITF PKXEF WMICO VTIVI"
 "BIEAC PFVXZ ILJXW TBRVB EFENE"
 "WQZTC CDMWV WGLDZ TXGUD JWSTR";

char text08_3[]=
 "BKWVQ ICHPW RRYJD AXQEI QJKQQ"
 "YMLTP VAKYC JZZTD AODOL STOKL"
 "SSXJR TQCKI KGRRD RJZYZ WWJPT"
 "ABZJE OWGRU KLASP PBMKZ BJRHI"
 "OKPAK YFZPC OUAAX DMZQM TLDFN"
 "NKEZD GRNUZ QA"; 

char text09[]=
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

char text10[]=
 "KYYUG IWKSE YPQDF YPIJN TGNDI" 
 "AHNBR OXDIK EKPTM OUHBE JRRJP"
 "VBAOC UZRDF SAZDC NUNNM RPCCM"
 "CHJBW STIKZ IREBB VJQAX ZARIY"
 "VANIJ VOLDN BUMXX FNZVR QEGOY"
 "XEVVN MPWEB SKEUT JJOKP BKLHI"
 "YWGNF FPXKI EWSNT LMDKY IDMOF"
 "PTDFJ AZOHV VQETN IPVZG TUMYJ"
 "CMSEA KTYEL PZUNH EYFCL AADYP"
 "EEXMH QMVAV ZZDOI MGLER BBLAT"
 "HQJIY CBSUP VVTRA DCRDD STYIX"
 "YFEAF ZYLNZ ZDPNN XXZJN RCWEX"
 "MTYRJ OIAOE KNRXG XPNMT DGKFZ"
 "DSYHM UJAPO BGANC RCZTM EPXES"
 "DZTTJ ZGNGQ RMKNC ZNAFM DAXXT"
 "JSRTA ZTZKR TOXHA HTNPE VNAAV"
 "UZMHL PXLMS TWELS OBCTM BKGCJ"
 "KMDPD QQGCZ HMIOC GRPDJ EZTYV"
 "DQGNP UKCGK FFWMN KWPSC LENWH"
 "UEYCL YVHZN KNVSC ZXUXD PZBDP"
 "SYODL QRLCG HARLF MMTPO CUMOQ"
 "LGJJA VXHZZ VBFLX HNNEJ XS";



/*
    Plain text: 
    Nadat eerst de Poolse en nadien, tijdens de Tweede Wereldoorlog, de Britse inlichtingendienst 
    erin slaagde de Enigmacodes te breken, bleek het toestel een goudmijn van informatie over de 
    Duitse oorlogsmachine. Deze informatie, verkregen door ontcijfering van de geheime Duitse 
    berichten, kreeg de codenaam "Ultra" en speelde een uiterst belangrijke rol in het verloop 
    van de Tweede Wereldoorlog, vooral in de U-bootoorlog in de Atlantische Oceaan, de veldslagen 
    in Afrika en de landing in Normandië.
*/
// Text as encoded
char text04_test1[]=
"zujzl anzfu lwrsg fppug rnpcx qvqcm rsnvg hdbrl vblyz uzvpw bnjki "
"kwkhm rhxgt esqnq yhmzb qoclv szjtd fqavw jkfuc temvr laeqz rdjji " 
"acmdf axaqd kitei pfbpy gwlzl kbwcd vilcm wixvn cflkc cnjse nxjhs "
"nthzs oegnh nbtnm hmydy uvlqx xdioc awrcg nzzcz wjpxa sestv ihzoc "
"ysiqv gcuqe nmitv abtqm qmxan byjzq uxxzt ntmwl qciun fkkuw skiqi "
"rxmdw foyqm pscxl cskds dewyj pumrm ichsz oydtm uhqnw zliwd fkunm "
"waqay vfvrn xskcv puaqu rnikq lmgjv nzhmd avkdc smuzt fjhic omjbk "
"plukp thsvf fesdu dwurk xbdwn ptd";

// Text as ecoded, 1st 15 characters removed
char text04_test2[]=
"fppug rnpcx qvqcm rsnvg hdbrl vblyz uzvpw bnjki "
"kwkhm rhxgt esqnq yhmzb qoclv szjtd fqavw jkfuc temvr laeqz rdjji " 
"acmdf axaqd kitei pfbpy gwlzl kbwcd vilcm wixvn cflkc cnjse nxjhs "
"nthzs oegnh nbtnm hmydy uvlqx xdioc awrcg nzzcz wjpxa sestv ihzoc "
"ysiqv gcuqe nmitv abtqm qmxan byjzq uxxzt ntmwl qciun fkkuw skiqi "
"rxmdw foyqm pscxl cskds dewyj pumrm ichsz oydtm uhqnw zliwd fkunm "
"waqay vfvrn xskcv puaqu rnikq lmgjv nzhmd avkdc smuzt fjhic omjbk "
"plukp thsvf fesdu dwurk xbdwn ptd";


char buffer1[1000];
char buffer2[1000];

/**************************************************************************************************\
* 
* Helper: display decoded text in more digestible format
* 
\**************************************************************************************************/

typedef enum
{
    MESSAGETYPE_KRIEGSMARINE,
    MESSAGETYPE_WEHRMACHT
} Message_t;
// TO DO: improve
void displayEnigmaMessage(char* message, Message_t type)
{
    switch (type)
    {
        // X=. ZZ=, FRAGE=? FRAQ=? 
        case MESSAGETYPE_WEHRMACHT: 
            for (int i=0;i<strlen(message);i++)
            {
                char c=message[i];
                if (c=='X' || c=='x')
                {
                    printf(". ");
                }
                else if (c=='J' || c=='j')
                {
                    printf(" ");
                }
                else 
                {
                    printf("%c", c);
                }
            }
            break;
        // X=. Y=, UD=? Q=ch CENTA=00 MILLE=000 MYRIA=0000
        case MESSAGETYPE_KRIEGSMARINE:
            for (int i=0;i<strlen(message);i++)
            {
                char c=message[i];
                if (c=='X' || c=='x')
                {
                    printf(". ");
                }
                else if (c=='y' || c=='Y')
                {
                    printf(", ");
                }
                else 
                {
                    printf("%c", c);
                }
            }
            break;
    }
    printf("\n");
}

/**************************************************************************************************\
* 
* Helper: generate the grundstellung for deciphering; assumes a configured Engima
* 
\**************************************************************************************************/
void generateGrundstellung(Enigma* enigma, char grundstellungen[3], char grundstellungText[3])
{
    setGrundStellungen(enigma, grundstellungen);
    setText(enigma, grundstellungText);
    encodeDecode(enigma);
    char* decoded=toString(enigma);
    setGrundStellungen(enigma, decoded);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 1
* Simply decode with all info given
* 
\**************************************************************************************************/
void message01()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 01\n");
    printf("#####################################################################################\n");
    Enigma* enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "VIII");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "IV");
    placeSteckers(enigma, "bd co ei gl js kt nv pm qr wz");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 19);    
    setRingStellung(enigma, 2,  7);    
    setRingStellung(enigma, 3, 12);

    generateGrundstellung(enigma, "WTG", "PLT");
    setText(enigma, text01);
    encodeDecode(enigma);
    
    printf("Message 01:\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 1
* All info given, except rotor order. Leaves 6 possibilies
* 
\**************************************************************************************************/
char message02Walzen[][MAX_ROTOR_NAME]  ={"I", "III", "V"};
int message02Indices[]                  ={0, 1, 2};

void message02()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 02\n");
    printf("#####################################################################################\n");

    LinkedList* permutations=createPermutations(message02Indices, 3, 3);
    Enigma* enigma=createEnigmaM3(); 
    resetLinkedList(permutations);
    while (hasNext(permutations))
    {
        int* permutation=(int*)nextLinkedListObject(permutations);
        printf("Permutation: UKW B - %3s - %3s - %3s\n",  
                message02Walzen[permutation[0]],
                message02Walzen[permutation[1]],
                message02Walzen[permutation[2]]);

        placeWalze(enigma, 1, message02Walzen[permutation[0]]);
        placeWalze(enigma, 2, message02Walzen[permutation[1]]);
        placeWalze(enigma, 3, message02Walzen[permutation[2]]);
        placeUmkehrWalze(enigma, "UKW B");
        placeSteckers(enigma, "BL CK DG FP IR MO QW ST VY UZ");
        setRingStellungen(enigma, "25 03 07");
        
        generateGrundstellung(enigma, "XLT", "VPM");

        setText(enigma, text02_1);
        encodeDecode(enigma);
        float ioc=iocIndexOfCoincidence(enigma);
        printf("Index of Coincidence %f\n", ioc);
        if (ioc>0.05)
        {
            printf("Message 02/1:\n");
            displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

            generateGrundstellung(enigma, "HNB", "SFA");
            setText(enigma, text02_2);
            encodeDecode(enigma);
            printf("Message 02/2:\n");
            displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
        }
    }  
    destroyPermutations(permutations);
    destroyEnigma(enigma);
}


/**************************************************************************************************\
* 
* Enigma Challenge message 3
* Missing stecker CE
* 
\**************************************************************************************************/
char message03NotUsed[]="BCDEGIMZ";
char message03Steckers[]="AL FP HX JO KT NV QR SU WY ??";

void message03()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 03\n");
    printf("#####################################################################################\n");
    Enigma* enigma=createEnigmaM3(); 
    placeWalze(enigma, 1, "III");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "V");
    placeUmkehrWalze(enigma, "UKW C");
    setRingStellungen(enigma, "08 19 03");

    float iocMax=0;
    char  c1='\0';
    char  c2='\0';
    for (int i=0;i<strlen(message03NotUsed); i++)
    {
        for (int j=i+1;j<strlen(message03NotUsed); j++)
        {
            message03Steckers[27]=message03NotUsed[i];
            message03Steckers[28]=message03NotUsed[j];
            printf("Testing steckers: %s, ", message03Steckers);
            clearSteckerBrett(enigma);
            placeSteckers(enigma, message03Steckers);
            generateGrundstellung(enigma, "AST", "SGT");
            setText(enigma, text03);
            encodeDecode(enigma);

            float ioc=iocIndexOfCoincidence(enigma);
            printf("IoC: %f\n", ioc);
            if (ioc>iocMax)
            {
                iocMax=ioc;
                c1=message03NotUsed[i];
                c2=message03NotUsed[j];
            }
        }
    }

    message03Steckers[27]=c1;
    message03Steckers[28]=c2;
    //printf("%s\n", steckers);
    clearSteckerBrett(enigma);
    placeSteckers(enigma, message03Steckers);
    generateGrundstellung(enigma, "AST", "SGT");
    setText(enigma, text03);
    
    encodeDecode(enigma);
    printf("Message 03: highest ioc %f, stecker added %c%c :\n", iocMax, c1, c2);
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 4
* Missing Grunstellung and Ringstellung Walze 1
* 
\**************************************************************************************************/
void message04()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 04 - Missing Grundstellung/Ringstellung\n");
    printf("#####################################################################################\n");

    char grundStellungen[4]="?KE";

    Enigma* enigma=createEnigmaM3(); 
    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "IV");
    placeWalze(enigma, 3, "I");
    placeSteckers(enigma, "bd cv el gn iz jo kw mt pr sx");
    placeUmkehrWalze(enigma, "UKW C");
    setRingStellung(enigma, 2, 3);    
    setRingStellung(enigma, 3, 21);    
    setText(enigma, text04);

    int maxCount=0;
    char theG1   =0;
    char theR1   =0;
    char g1=1;
    while (g1<=MAX_POSITIONS)
    {
        char r1=1;
        while (r1<=MAX_POSITIONS)
        {
            setRingStellung (enigma, 1, r1);
            grundStellungen[0]=g1;
            setGrundStellungen(enigma, grundStellungen);
            encodeDecode(enigma);
            
            int count=countLetter(enigma, 'E');
            if (count>maxCount)
            {
                maxCount=count;
                theR1=r1;
                theG1=g1;
            }
            
            r1++;
        }
        g1++;
    }
    setRingStellung (enigma, 1, theR1);
    grundStellungen[0]=theG1;
    setGrundStellungen(enigma, grundStellungen);
    encodeDecode(enigma);
    float ioc=iocIndexOfCoincidence(enigma);
    printf("Found R %d G %d count %d IoC %f:\n", theR1, theG1, maxCount, ioc);
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 5
* Missing Walze 3, Ringstellung and Grundstellung
* 
\**************************************************************************************************/
void message05()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 05 - Missing Walze 3, Rinstellung 3 , Grundstellung 3\n");
    printf("#####################################################################################\n");
    char possibleWalzen[3][4]={"I","III","V"};
    char grundStellungen[4]="EF?";

    Enigma* enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "IV");

    placeSteckers(enigma, "AS CK DE FV GJ LU MW OT PX RZ");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 9);
    setRingStellung(enigma, 2, 2);

    int w=0;
    while (w<3)
    {
        placeWalze(enigma, 3, possibleWalzen[w]);
        
        setText(enigma, text05);
        
        char g3=1;
        while (g3<=MAX_POSITIONS)
        {
            char r3=1;
            while (r3<=MAX_POSITIONS)
            {
                setRingStellung (enigma, 3, r3);
                grundStellungen[2]=g3;
                setGrundStellungen(enigma, grundStellungen);
                encodeDecode(enigma);
                int count=countLetter(enigma, 'E');
                float ioc=iocIndexOfCoincidence(enigma);
                if (ioc>0.05)
                {
                    printf("Found %s R %d G %d 'E' count %d %f:\n", possibleWalzen[w], r3, g3, count, ioc);
                    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
                }
                r3++;
            }
            g3++;
        }
        w++;
    }
    
}


/**************************************************************************************************\
* 
* Enigma Challenge message 6
* Missing characters
* 
\**************************************************************************************************/
void message06()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 06 - Missing characters\n");
    printf("#####################################################################################\n");
    char    test1;
    char    test2;
    char    grundstellungenText[4]="???";

    Enigma* enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "V");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "II");
    placeSteckers(enigma, "cq du en fr gx is jp ko ty vz");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellung(enigma, 1, 10);    
    setRingStellung(enigma, 2, 12);    
    setRingStellung(enigma, 3, 14);

    float maxIoc=0.0f;
    char  theTest1='\0';
    char  theTest2='\0';
    test1=0;
    while (test1<MAX_POSITIONS)
    {
        grundstellungenText[0]='A'+test1;
        grundstellungenText[1]='W';
        grundstellungenText[2]='E';
        generateGrundstellung(enigma, "ACE", grundstellungenText);
        setText(enigma, text06_1);
        encodeDecode(enigma);

        float ioc=iocIndexOfCoincidence(enigma);
        if (ioc>maxIoc)
        {
            maxIoc=ioc;
            theTest1=test1;
        }
        test1++;
    }
    grundstellungenText[0]='A'+theTest1;
    grundstellungenText[1]='W';
    grundstellungenText[2]='E';
    generateGrundstellung(enigma, "ACE", grundstellungenText);
    setText(enigma, text06_1);
    encodeDecode(enigma);
    printf("Message 06-1, IoC %f:\n", maxIoc);
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    test1=0;
    maxIoc=0.0f;
    while (test1<MAX_POSITIONS)
    {
        test2=0;
        while (test2<MAX_POSITIONS)
        {
            grundstellungenText[0]='A';
            grundstellungenText[1]='A'+test1;
            grundstellungenText[2]='A'+test2;
            generateGrundstellung(enigma, "SED", grundstellungenText);
            setText(enigma, text06_2);
            encodeDecode(enigma);
            
            float ioc=iocIndexOfCoincidence(enigma);
            if (ioc>maxIoc)
            {
                maxIoc=ioc;
                theTest1=test1;
                theTest2=test2;
            }
            test2++;
        }
        test1++;
    }    
    grundstellungenText[0]='A';
    grundstellungenText[1]='A'+theTest1;
    grundstellungenText[2]='A'+theTest2;
    generateGrundstellung(enigma, "SED", grundstellungenText);
    setText(enigma, text06_2);
    encodeDecode(enigma);
    printf("Message 06-2, IoC %f:\n", maxIoc);
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    generateGrundstellung(enigma, "HIP", "PYX");
    setText(enigma, text06_3);
    encodeDecode(enigma);
    printf("Message 06-3:\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
}


/**************************************************************************************************\
* 
* Enigma Challenge message 7
* 
* 
\**************************************************************************************************/
void message07()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 07 - Missing Walzen\n");
    printf("#####################################################################################\n");
    int*            permutation;
    char            walzen[5][6]    ={"I", "II", "III", "IV", "V"};
    int             permElements[5] ={0, 1, 2, 3, 4};
    
    Enigma* enigma=createEnigmaM3(); 
    placeSteckers(enigma, "au cm dp ev hl iz jw no qx st");
    placeUmkehrWalze(enigma, "UKW B");
    setRingStellung(enigma, 1, 5);    
    setRingStellung(enigma, 2, 22);    
    setRingStellung(enigma, 3, 11);

    LinkedList* permutations=createPermutations(permElements, 5, 3);
    float maxIoc    =0.0f;
    int*  maxPerm   =NULL;
    for (int i=0; i<linkedListLength(permutations); i++)
    {
        permutation=(int*)elementAt(permutations, i);
        
        logDebug("%d %d %d: %s-%s-%s", permutation[0], permutation[1], permutation[2], walzen[permutation[0]], walzen[permutation[1]], walzen[permutation[2]]);

        placeWalze(enigma, 1, walzen[permutation[0]]);
        placeWalze(enigma, 2, walzen[permutation[1]]);
        placeWalze(enigma, 3, walzen[permutation[2]]);
        generateGrundstellung(enigma, "AEG", "GJW");
        setText(enigma, text07_1);
        encodeDecode(enigma);
        float ioc=iocIndexOfCoincidence(enigma);
        if (ioc>maxIoc)
        {
            maxIoc=ioc;
            maxPerm=permutation;
        }
        i++;
    }
    placeWalze(enigma, 1, walzen[maxPerm[0]]);
    placeWalze(enigma, 2, walzen[maxPerm[1]]);
    placeWalze(enigma, 3, walzen[maxPerm[2]]);
    generateGrundstellung(enigma, "AEG", "GJW");
    setText(enigma, text07_1);
    encodeDecode(enigma);
    printf("Message 07-1:\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    generateGrundstellung(enigma, "VSF", "DNA");
    setText(enigma, text07_2);
    encodeDecode(enigma);
    printf("Message 07-2:\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    destroyPermutations(permutations);
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 8
* Three missing steckers
* 
\**************************************************************************************************/
void message08()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 08 - 3 missing steckers\n");
    printf("#####################################################################################\n");
    int*        permutation;

    char letters[12]={'a', 'c', 'e', 'f', 'g', 'h', 'i', 'j', 'o', 'q', 'w', 'y'};
    int  digits[12] ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    char steckers[] ="bm dv kt ln rs up xz ?? ?? ??";
    
    LinkedList* permutations=createPermutations(digits, 12, 6);
    Enigma* enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "III");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 26);    
    setRingStellung(enigma, 2,  3);    
    setRingStellung(enigma, 3, 16);

    printf("Permutations to try: %d\n", linkedListLength(permutations));
    float maxIoc    =0.0f;
    int*  maxPerm   =NULL;
    resetLinkedList(permutations);
    while (hasNext(permutations))
    {
        permutation=(int*)nextLinkedListObject(permutations);
        steckers[21]=letters[permutation[0]];
        steckers[22]=letters[permutation[1]];
        steckers[24]=letters[permutation[2]];
        steckers[25]=letters[permutation[3]];
        steckers[27]=letters[permutation[4]];
        steckers[28]=letters[permutation[5]];
        placeSteckers(enigma, steckers);
        generateGrundstellung(enigma, "BKL", "UPR");
        setText(enigma, text08_1);
        encodeDecode(enigma);
        float ioc=iocIndexOfCoincidence(enigma);
        if (ioc>maxIoc)
        {
            maxIoc=ioc;
            maxPerm=permutation;
        }
    }   
    steckers[21]=letters[maxPerm[0]];
    steckers[22]=letters[maxPerm[1]];
    steckers[24]=letters[maxPerm[2]];
    steckers[25]=letters[maxPerm[3]];
    steckers[27]=letters[maxPerm[4]];
    steckers[28]=letters[maxPerm[5]];
    placeSteckers(enigma, steckers);
    generateGrundstellung(enigma, "BKL", "UPR");
    setText(enigma, text08_1);
    encodeDecode(enigma);
    printf("Message 08-1\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
    
    generateGrundstellung(enigma, "SPL", "BKK");
    setText(enigma, text08_2);
    encodeDecode(enigma);
    printf("Message 08-2\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
    
    generateGrundstellung(enigma, "DVB", "LTK");
    setText(enigma, text08_3);
    encodeDecode(enigma);
    printf("Message 08-3\n");
    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);

    destroyPermutations(permutations);
    destroyEnigma(enigma);
}


/**************************************************************************************************\
* 
* Enigma Challenge message 9
* Missing Grundstellungen
* 
\**************************************************************************************************/
void message09()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 09 - 3 missing Grundstellungen\n");
    printf("#####################################################################################\n");
    Enigma* enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "IV");
    placeWalze(enigma, 2, "III");
    placeWalze(enigma, 3, "I");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellung(enigma, 1,  7);    
    setRingStellung(enigma, 2, 24);    
    setRingStellung(enigma, 3, 15);
    placeSteckers(enigma, "AT BG DV EW FR HN IQ JX KZ LU");

    setText(enigma, text09);
    int limit=enigma->textSize*12/100;

    int g1=1;
    while (g1<MAX_POSITIONS)
    {
        int g2=1;
        while (g2<MAX_POSITIONS)
        {
            int g3=1;
            while (g3<MAX_POSITIONS)
            {
                setGrundStellung(enigma, 1, g1);
                setGrundStellung(enigma, 2, g2);
                setGrundStellung(enigma, 3, g3);
                
                encodeDecode(enigma);

                int count=countLetter(enigma, 'E');
                if (count>limit)
                {
                    printf("Message 09: G %d %d %d: \n", g1, g2, g3); 
                    displayEnigmaMessage(toString(enigma), MESSAGETYPE_WEHRMACHT);
                }                  
                g3++;
            }
            g2++;
        }
        g1++;
    }
    destroyEnigma(enigma);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 10 step 1 - brute force
* Vary
* * Rotors
* * Ringstellung R3
* * Grundstellungen
* * Steckers
* It takes multiple days to solve, but it does
\**************************************************************************************************/

void message10_step01()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 10 - Just cipher\n");
    printf("#####################################################################################\n");
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=10;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=100;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(text10, 6);
// Results in:
//    1: UKW B  II   V   I R  1  1 19 G 21  6 25 - AO BV DS EX FT HZ IQ JW KU PR - 0.071871
//    2: UKW B  II   V   I R  1  1 18 G 21  6 24 - AO BV DS EX FT HZ IQ JW KU PR - 0.071839
//    3: UKW B  II   V   I R  1  1 17 G 21  6 23 - AO BV DS EX FT HZ IQ JW KU PR - 0.070940
//    4: UKW B  II   V   I R  1  1 16 G 21  6 22 - AO BV DS EX FT HZ IQ JW KU PR - 0.069237
//    5: UKW B  II   V   I R  1  1 15 G 21  6 21 - AO BV DS EX FT HZ IQ JW KU PR - 0.068145
//    6: UKW B  II   V   I R  1  1 14 G 21  6 20 - AO BV DS EX FT HZ IQ JW KU PR - 0.067920
}

/**************************************************************************************************\
* 
* Enigma Challenge message 10 step 1 - brute force
* Just try one permutation - for testing purposes - takes hours iso days
\**************************************************************************************************/

void message10_step01Limited()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 10 - Just cipher - LIMITED\n");
    printf("#####################################################################################\n");

    int numOfThreads=6;

    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // III I II, UKW B, 

    LinkedList*     permutations=createLinkedList();
    int * permutation;
    permutation   =malloc(4*sizeof(int)); // The winning permutation
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=4; // V
    permutation[3]=0; // I
    addObject(permutations, permutation);
    permutation   =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=4; // V
    permutation[3]=2; // III
    addObject(permutations, permutation);
    permutation   =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=4; // V
    permutation[3]=3; // IV
    addObject(permutations, permutation);
    permutation   =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=2; // III
    permutation[2]=4; // V
    permutation[3]=0; // I
    addObject(permutations, permutation);
    permutation   =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=3; // IV
    permutation[2]=4; // V
    permutation[3]=0; // I
    addObject(permutations, permutation);
    permutation   =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=3; // IV
    permutation[3]=0; // I
    addObject(permutations, permutation);


    // Create work item
    dispatcherClearWorkItems();
    for (int i=0;i<6;i++)
    {
        iocWorkItems[i].cipher              =text10;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =i;
        iocWorkItems[i].endPermutation      =i;
        iocWorkItems[i].R1                  =1;
        iocWorkItems[i].startR2             =1;
        iocWorkItems[i].endR2               =1;
        iocWorkItems[i].startR3             =1;
        iocWorkItems[i].endR3               =MAX_POSITIONS;
        iocWorkItems[i].maxCipherChars      =MAX_TEXT;
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i]);
    }
    iocSetCustomWalzePermutations(permutations);

    // Define recipe
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=10;
    recipe.ngramSize        =0;
    recipe.ngramSet[0]      ='\0';
    recipe.scoreListSize    =50;
    recipe.numberOfSolutions=10;
    setOperation(recipe);

    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL, false);
}

/**************************************************************************************************\
* 
* Enigma Challenge message 10 step 2
* 
* 
\**************************************************************************************************/

void message10_step02()
{
    printf("#####################################################################################\n");
    printf("# ENIGMA CHALLENGE  - MESSAGE 10 - Just cipher\n");
    printf("#####################################################################################\n");

    int numOfThreads        =4;
    LinkedList* permutations=createLinkedList();
    int* permutation        =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=4; // V
    permutation[3]=0; // I
    addObject(permutations, permutation);

	// STEP 2: NOW TRY FOR THE BEST ROTOR SETTINGS OF STEP 01 AND VARY ALL R2
    dispatcherClearWorkItems();
    for (int i=0;i<numOfThreads;i++)
    {
		iocWorkItems[i].cipher              =text10;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =0;
        iocWorkItems[i].endPermutation      =0;
        iocWorkItems[i].R1                  =1;
        iocWorkItems[i].startR2             =i*(MAX_POSITIONS-1)/numOfThreads+1;     // Some double ringstellungen at 7, 13, 19
        iocWorkItems[i].endR2               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].startR3             =1;
        iocWorkItems[i].endR3               =MAX_POSITIONS;
        iocWorkItems[i].maxCipherChars      =MAX_TEXT;
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i]); 
    }
//    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 3, NULL);
// TODO
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=10;
    recipe.ngramSize        =0;
    recipe.ngramSet[0]      ='\0';
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    recipe.numberOfSolutions=10;
    setOperation(recipe);

    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL, false);
}


/**************************************************************************************************\
* 
* Enigma Challenge message 10
* 
* 
\**************************************************************************************************/
void message10_exp()
{
/*	
    int             i;
    LinkedList*     permutations;
    int				numOfThreads;
    
    numOfThreads=4;
	
    permutations=createRotorPermutations(3, 5);
  

	// TEST
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads;
    dispatcherClearWorkItems();
    i=0;
    while (i<numOfThreads)
    {
		iocWorkItems[i].cipher              =text10;
        iocWorkItems[i].permutations        =permutations;
        iocWorkItems[i].startPermutation    =23;
        iocWorkItems[i].endPermutation      =23;
        iocWorkItems[i].R1                  =1;
        iocWorkItems[i].startR3             =i*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].endR3               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].startR2             =1;
        iocWorkItems[i].endR2               =1;
        iocWorkItems[i].maxCipherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i]); 
        i++;
    }
    //setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 3, NULL);

    dispatcherStartWork(6, iocFinishFunction, NULL, false);	


    */
// TODO
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=4;
    recipe.ngramSize        =0;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    recipe.numberOfSolutions=10;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(text10, 6);
    
}