/**************************************************************************************************\
* crack01.c
* 
* Enigma challenge (see /documents/engimaChallenge.pdf) or 
* https://www.ciphermachinesandcryptology.com/en/challenge.htm
* 
* The challenge consists of decryption of 10 messages, from easy to hard. This file shows
* how the software is used for the decryption.
* 
\**************************************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>

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

void message01()
{
    Enigma* enigma;
    char*   decoded;
    
	printf("MESSAGE 01\n");
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "VIII");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "IV");
    placeSteckers(enigma, "bd co ei gl js kt nv pm qr wz");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 19);    
    setRingStellung(enigma, 2,  7);    
    setRingStellung(enigma, 3, 12);
      
    setGrundStellung(enigma, 1, 'W');
    setGrundStellung(enigma, 2, 'T');
    setGrundStellung(enigma, 3, 'G');

    setText(enigma, "PLT");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text01);
    
    encodeDecode(enigma);
    
    printf("Message 01: %s\n", toString(enigma));
    
}

void message02()
{
    Enigma* enigma;
    char*   decoded;
    
	printf("MESSAGE 02\n");
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "III");
    placeWalze(enigma, 2, "V");
    placeWalze(enigma, 3, "I");
    placeSteckers(enigma, "BL CK DG FP IR MO QW ST VY UZ");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellungen(enigma, "25 03 07");
      
    setGrundStellungen(enigma, "X L T");
    setText(enigma, "VPM");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text02_1);
    
    encodeDecode(enigma);
    
    printf("Message 02/1: %s\n", toString(enigma));
    
    setGrundStellungen(enigma, "H N B");
    setText(enigma, "SFA");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text02_2);
    
    encodeDecode(enigma);
    
    printf("Message 02/2: %s\n", toString(enigma));
    
    destroyEnigma(enigma);
}


void message03()
{
    Enigma* enigma;
    char*   decoded;
    
	printf("MESSAGE 03\n");
    enigma=createEnigmaM3(); 


    placeWalze(enigma, 1, "III");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "V");
    placeSteckers(enigma, "AL FP HX JO KT NV QR SU WY CE");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellungen(enigma, "08 19 03");
      
    setGrundStellungen(enigma, "A S T");
    setText(enigma, "SGT");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text03);
    
    encodeDecode(enigma);
    
    printf("Message 03: %s\n", toString(enigma));
    
    destroyEnigma(enigma);    
}



void message04()
{
    Enigma* enigma;
    int     g1;
    int     r1;
    int     count;
	int     limit;

	printf("MESSAGE 04\n");
    enigma=createEnigmaM3(); 



    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "IV");
    placeWalze(enigma, 3, "I");
    placeSteckers(enigma, "bd cv el gn iz jo kw mt pr sx");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellung(enigma, 2, 3);    
    setRingStellung(enigma, 3, 21);    


    

    setText(enigma, text04);
    // Frequence e=18%, n=10%. Therefore we put the limit on 14%
    limit=enigma->textSize*10/100;		
    
    g1=1;
    while (g1<=MAX_POSITIONS)
    {
        r1=1;
        while (r1<=MAX_POSITIONS)
        {
            setGrundStellung(enigma, 2, 'K');    
            setGrundStellung(enigma, 3, 'E');    
            setRingStellung (enigma, 1, r1);    
            setGrundStellung(enigma, 1, g1);

            encodeDecode(enigma);
            
            count=countLetter(enigma, 'E');
            
            if (count>limit)
            {
                printf("Found R %d G %d count %d:\n%s\n\n", r1, g1, count, toString(enigma));
            }
            
            r1++;
        }
        g1++;
    }

    
}

void message05()
{
	printf("MESSAGE 05\n");
    Enigma*     enigma;
    int         g1;
    int         r1;
    int         count;
	int         limit;
    char        possibleWalzen[3][4]={"I","III","V"};
    int         w;

    printf("Message 05\n");
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "IV");

    placeSteckers(enigma, "AS CK DE FV GJ LU MW OT PX RZ");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 9);    
    setRingStellung(enigma, 2, 2);    


    w=0;
    while (w<3)
    {
        placeWalze(enigma, 3, possibleWalzen[w]);
        
        setText(enigma, text05);
        // Frequence e=18%, n=10%. 10 gives a good result 
        limit=enigma->textSize*12/100;		
        
        g1=1;
        while (g1<=MAX_POSITIONS)
        {
            r1=1;
            while (r1<=MAX_POSITIONS)
            {
                setGrundStellung(enigma, 1, 'E');    
                setGrundStellung(enigma, 2, 'F');    
                setRingStellung (enigma, 3, r1);    
                setGrundStellung(enigma, 3, g1);

                encodeDecode(enigma);
                
                count=countLetter(enigma, 'E');
                
                if (count>22)
                {
                    printf("Found %s R %d G %d count%d limit %d:\n%s\n", 
                           possibleWalzen[w], r1, g1, count, limit, toString(enigma));
                }
                
                r1++;
            }
            g1++;
        }
        w++;
    }
    
}


void message06()
{
    Enigma* enigma;
    char*   decoded;
    char    test1;
    char    test2;
    int     limit;
    int     count;
    
	printf("MESSAGE 06\n");
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "V");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "II");
    placeSteckers(enigma, "cq du en fr gx is jp ko ty vz");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellung(enigma, 1, 10);    
    setRingStellung(enigma, 2, 12);    
    setRingStellung(enigma, 3, 14);

    test1=0;
    while (test1<MAX_POSITIONS)
    {
        setGrundStellung(enigma, 1, 'A');
        setGrundStellung(enigma, 2, 'C');
        setGrundStellung(enigma, 3, 'E');

        buffer1[0]='A'+test1;
        sprintf(buffer1+1, "WE");
        setText(enigma, buffer1);
        encodeDecode(enigma);
        
        decoded=toString(enigma);
        
        setGrundStellung(enigma, 1, decoded[0]);
        setGrundStellung(enigma, 2, decoded[1]);
        setGrundStellung(enigma, 3, decoded[2]);

        setText(enigma, text06_1);
        limit=enigma->textSize*9/100;
        
        encodeDecode(enigma);
        
        count=countLetter(enigma, 'E');
        if (count>limit)
        {
            decoded=toString(enigma);
            printf("Message 06-1: %s\n", decoded); 
        }            
        test1++;
    }
    
    test1=0;
    while (test1<MAX_POSITIONS)
    {
        test2=0;
        while (test2<MAX_POSITIONS)
        {
            setGrundStellung(enigma, 1, 'S');
            setGrundStellung(enigma, 2, 'E');
            setGrundStellung(enigma, 3, 'D');

            buffer1[0]='A';
            buffer1[1]='A'+test1;
            buffer1[2]='A'+test2;
            buffer1[3]='\0';
            setText(enigma, buffer1);
            encodeDecode(enigma);
            
            decoded=toString(enigma);
            
            setGrundStellung(enigma, 1, decoded[0]);
            setGrundStellung(enigma, 2, decoded[1]);
            setGrundStellung(enigma, 3, decoded[2]);

            setText(enigma, text06_2);
            limit=enigma->textSize*6/100;
            
            encodeDecode(enigma);
            
            count=countLetter(enigma, 'E');
            decoded=toString(enigma);
            if (count>limit/*decoded[0]=='N' && decoded[1]=='S'*/)
            {
                printf("Message 06-2: %s\n", toString(enigma)); 
            }            
            test2++;
        }
        test1++;
    }    

    setGrundStellung(enigma, 1, 'H');
    setGrundStellung(enigma, 2, 'I');
    setGrundStellung(enigma, 3, 'P');

    setText(enigma, "PYX");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text06_3);
    
    encodeDecode(enigma);
    
    printf("Message 06-3: %s\n", toString(enigma)); 

}


void message07()
{
    Enigma*         enigma;
    char*           decoded;
    int*            permutation;
    LinkedList*     permutations;
    int             i;
    int             found;
    
    int             limit;
    int             count;
    
    char            walzen[5][6]       ={"I", "II", "III", "IV", "V"};
    int             permElements[5]     ={0, 1, 2, 3, 4};
    
	printf("MESSAGE 07\n");
    enigma=createEnigmaM3(); 

    placeSteckers(enigma, "au cm dp ev hl iz jw no qx st");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 5);    
    setRingStellung(enigma, 2, 22);    
    setRingStellung(enigma, 3, 11);
      
    permutations=createLinkedList();
    permute(permutations, permElements, 5, 3, 0);

    found=0;
    i=0;
    while ((i<linkedListLength(permutations)) && !found)
    {
        permutation=(int*)elementAt(permutations, i);
        
        printf("%d %d %d: %s-%s-%s\n", permutation[0], permutation[1], permutation[2], walzen[permutation[0]], walzen[permutation[1]], walzen[permutation[2]]);
        
        placeWalze(enigma, 1, walzen[permutation[0]]);
        placeWalze(enigma, 2, walzen[permutation[1]]);
        placeWalze(enigma, 3, walzen[permutation[2]]);

        setGrundStellungen(enigma, "A E G");

        setText(enigma, "GJW");
        encodeDecode(enigma);
        
        decoded=toString(enigma);
        
        setGrundStellung(enigma, 1, decoded[0]);
        setGrundStellung(enigma, 2, decoded[1]);
        setGrundStellung(enigma, 3, decoded[2]);

        setText(enigma, text07_1);
        limit=enigma->textSize*9/100;
        
        encodeDecode(enigma);
        
        count=countLetter(enigma, 'E');
        if (count>limit)
        {
            decoded=toString(enigma);
            printf("Message 06-1: %s\n", decoded); 
            found=1;
        }   
        free(permutation);
        i++;
    }

    setGrundStellungen(enigma, "V S F");

    setText(enigma, "DNA");
    encodeDecode(enigma);
    
    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text07_2);
    
    encodeDecode(enigma);
    decoded=toString(enigma);
    printf("Message 06-2: %s\n", decoded);     

    destroyEnigma(enigma);
    destroyLinkedList(permutations);
}


void message08()
{
    Enigma*     enigma;
    char*       decoded;
    LinkedList* permutations;
    int*        permutation;
    int         i;
    int         found;

    
	printf("MESSAGE 08\n");
    char letters[12]={'a', 'c', 'e', 'f', 'g', 'h', 'i', 'j', 'o', 'q', 'w', 'y'};
    int  digits[12]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    permutations=createLinkedList();
    permute(permutations, digits, 12, 6, 0);
    
    printf("Permutations: %d\n", linkedListLength(permutations));
    
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "II");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "III");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 26);    
    setRingStellung(enigma, 2,  3);    
    setRingStellung(enigma, 3, 16);

    found=0;
    i=0;
    while (i<linkedListLength(permutations) && !found)
    {
        permutation=(int*)elementAt(permutations, i);
        
        sprintf(buffer1, "bm dv kt ln rs up xz ?? ?? ??");
        buffer1[21]=letters[permutation[0]];
        buffer1[22]=letters[permutation[1]];
        buffer1[24]=letters[permutation[2]];
        buffer1[25]=letters[permutation[3]];
        buffer1[27]=letters[permutation[4]];
        buffer1[28]=letters[permutation[5]];
        
        placeSteckers(enigma, buffer1);

        setGrundStellungen(enigma, "BKL");

        setText(enigma, "UPR");
        encodeDecode(enigma);    

        decoded=toString(enigma);
        
        setGrundStellung(enigma, 1, decoded[0]);
        setGrundStellung(enigma, 2, decoded[1]);
        setGrundStellung(enigma, 3, decoded[2]);

        setText(enigma, text08_1);
       
        encodeDecode(enigma);

        decoded=toString(enigma);
        strncpy(buffer2, decoded, 13);

        
        if (strncmp(buffer2, "VONGRUPPEWEST", 13)==0)
        {
            decoded=toString(enigma);
            printf("Message 08-1 @ %s\n%s\n", buffer1, decoded); 
            found=1;
        }

        free(permutation);
        
        i++;
    }   
    
    
    setGrundStellungen(enigma, "SPL");

    setText(enigma, "BKK");
    encodeDecode(enigma);    

    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text08_2);
   
    encodeDecode(enigma);

    decoded=toString(enigma);
    printf("%s\n", decoded);
    
    setGrundStellungen(enigma, "DVB");

    setText(enigma, "LTK");
    encodeDecode(enigma);    

    decoded=toString(enigma);
    
    setGrundStellung(enigma, 1, decoded[0]);
    setGrundStellung(enigma, 2, decoded[1]);
    setGrundStellung(enigma, 3, decoded[2]);

    setText(enigma, text08_3);
   
    encodeDecode(enigma);
    decoded=toString(enigma);
    printf("%s\n", decoded);    


    destroyEnigma(enigma);
    destroyLinkedList(permutations);
}


void message09()
{
    Enigma*     enigma;
    char*       decoded;
    int         g1, g2, g3;
    int         limit;
    int         count;

    
	printf("MESSAGE 09\n");
    enigma=createEnigmaM3(); 

    placeWalze(enigma, 1, "IV");
    placeWalze(enigma, 2, "III");
    placeWalze(enigma, 3, "I");
    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellung(enigma, 1,  7);    
    setRingStellung(enigma, 2, 24);    
    setRingStellung(enigma, 3, 15);
    placeSteckers(enigma, "AT BG DV EW FR HN IQ JX KZ LU");

    setText(enigma, text09);
    limit=enigma->textSize*12/100;


    g1=1;
    while (g1<MAX_POSITIONS)
    {
        g2=1;
        while (g2<MAX_POSITIONS)
        {
            g3=1;
            while (g3<MAX_POSITIONS)
            {
                setGrundStellung(enigma, 1, g1);
                setGrundStellung(enigma, 2, g2);
                setGrundStellung(enigma, 3, g3);
                
                encodeDecode(enigma);

                count=countLetter(enigma, 'E');
                if (count>limit)
                {
                    decoded=toString(enigma);
                    printf("Message 09: G %d %d %d: %s\n", g1, g2, g3, decoded); 
                }                  
                g3++;
            }
            
            g2++;
        }

        g1++;
    }
    destroyEnigma(enigma);
    
}




void message10_step01()
{
    int             i;
    LinkedList*     permutations;
    int				numOfThreads;
    
	
	printf("MESSAGE 10 - STEP 1\n");
    numOfThreads=4;
    permutations=createRotorPermutations(3, 5);

  	// STEP 1: INITIAL TRY: TRY ALL ROTOR POSTIONS
    // Start with 5 Wehrmacht rotors
    // This function employs the IOC_DEEP, which basically is 
    // brute forcing. It takes extemely long
   
    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads*2;

    int length=linkedListLength(permutations);
    dispatcherClearWorkItems();
    i=0;
    while (i<numOfThreads)
    {
		iocWorkItems[i*2].cipher            =text10;
        iocWorkItems[i*2].permutations      =permutations;
        iocWorkItems[i*2].startPermutation  =i*length/numOfThreads;
        iocWorkItems[i*2].endPermutation    =(i+1)*length/numOfThreads;
        iocWorkItems[i*2].R1                  =1;
        iocWorkItems[i*2].startR2           =1;
        iocWorkItems[i*2].endR2             =1;
        iocWorkItems[i*2].startR3           =1;
        iocWorkItems[i*2].endR3             =MAX_POSITIONS;
        iocWorkItems[i*2].maxCipherChars    =250;
        strncpy(iocWorkItems[i*2].ukw, "UKW B", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i*2]);

        
		iocWorkItems[i*2+1].cipher          =text10;
        iocWorkItems[i*2+1].permutations    =permutations;
        iocWorkItems[i*2+1].startPermutation=i*length/numOfThreads;
        iocWorkItems[i*2+1].endPermutation  =(i+1)*length/numOfThreads;
        iocWorkItems[i*2+1].R1               =1;
        iocWorkItems[i*2+1].startR2         =1;
        iocWorkItems[i*2+1].endR2           =1;
        iocWorkItems[i*2+1].startR3         =1;
        iocWorkItems[i*2+1].endR3           =MAX_POSITIONS;
        iocWorkItems[i*2+1].maxCipherChars  =250;
        strncpy(iocWorkItems[i*2+1].ukw, "UKW C", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i*2+1]);        
        i++;
    }
    setWalzePermutations(permutations);
    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 3, "DE");
    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL);	
	
	// THIS RESULTS IN THE BEST SOLUTION:
	//  1: UKW B  II   V   I R  1  1 18 G 21  6 24 - AO BV DS EX FT HZ IQ JW KU PR - 0.071839
    //  1: UKW B  II   V   I R  1  1 19 G 21  6 25 - AO BV DS EX FT HZ IQ JW KU PR - 0.053566 (IOC_DEEP, 10 steckers)

}

void message10_step02()
{
    int             i;
    LinkedList*     permutations;
    int				numOfThreads;
    
	printf("MESSAGE 10 - STEP 2\n");
    numOfThreads=4;
    permutations=createRotorPermutations(3, 5);

    // Create the stack of work for the trheads
    iocNumberOfWorkItems=numOfThreads*2;

	// STEP 2: NOW TRY FOR THE BEST ROTOR SETTINGS OF STEP 01 AND VARY ALL R2
   
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
        iocWorkItems[i].startR2             =i*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].endR2               =(i+1)*(MAX_POSITIONS-1)/numOfThreads+1;
        iocWorkItems[i].startR3             =1;
        iocWorkItems[i].endR3               =MAX_POSITIONS;
        iocWorkItems[i].maxCipherChars      =MAX_TEXT;
        strncpy(iocWorkItems[i].ukw, "UKW B", MAX_ROTOR_NAME);
        dispatcherPushWorkItem(iocWorkerFunction, &iocWorkItems[i]); 

        i++;
    }
    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 3, NULL);
    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL);
}


void message10_exp()
{
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
    setEvaluationMethod(METHOD_IOC_R2R3, 10, 10, 0, NULL);
    dispatcherStartWork(numOfThreads, iocFinishFunction, NULL);	
}




