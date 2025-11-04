/**************************************************************************************************\
* 
* This file presents a number of IoC examples. First the original James Gillogly example, then
* a number of other ciphers.
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


/**************************************************************************************************\
* 
* Example from the original James Gillogly article. 7 steckers
* After step 1 (rotor position): solution on place 2 of the list
* Note on place 1 is a UKW C solution; Gillogly did not ake UKW C into account in his example
* After step 2 (ringstellungen): solution on place 1
* 
\**************************************************************************************************/

// The orignal Gillogly cipher text and enigma settings, 7 steckers
EnigmaSettings iocExampleSettings00=
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
    "GFZJC URWGT QZCTL LOIEK AOISK HAAQF OPFUZ IRTLW EVYWM DN"
};

void iocExample00()
{
    printEnigmaSettings(&iocExampleSettings00, "INDEX OF COINCIDENCE METHOD EXAMPLE 0 - James Gilloglys' example, 7 steckers");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=1;
    recipe.scoreListSize    =5;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings00.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption with 8 steckers
* Works
* After step 1: solution on place 52
* After step 2: solution on place 1
\**************************************************************************************************/

// The orignal Gillogly cipher text but other engima settings, 8 steckers
// The original Gillogly method won't find the solution
EnigmaSettings iocExampleSettings01A=
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
    "RW JO BL IU KT AC SY PQ",
    "isxxr rfvuk ivvcf xozlr ewedp fggon aalyq avyrb ixwwk melvf"
    "gcsne pawuq fgywg xapbs astop qyawu kqnau nfduu gdinf zksnt"
    "rfsyo thctm yglkb kkgup wofxg jbzxp aslll rrusk avwoc oloxp"
    "hvbnv mnuqb rumpo njczp fmxkw sszay heaxw cnezn rtqxc gsvcn"
    "sqhfw zwlar zxyxz zuryj cclgv ntfno knjyb ubofb rnlce wvfgi"
    "yhbko lzfof opndn sgeji ahgvf qqtvw uakaw jvrsg fzmtr jfvwb"
    "mtdqt qmldn fnjuq gujdv dsfsv obbwt dgrdr xeryu irhpz vlqyd"
    "ciexk eezmy suion bsrgf qsdhf uxows tykyp zoopr jsisg czpiw"
    "hikww pheir qagld fgycz fqwyf zazct kstas gxycs powzx qgvie"
    "iosrm felbh qftdw ctnvy tlhny lrgcl fqyzb ebbkm tfkdd vxeyp"
    "sijlu jzhrl inmzj hhupz nqcch hzlnz pdbxk ebxcf quhcf aghia"
    "qdpsn bapmv harup udvim bxwsq ajoen gsozk jteot fliqb rokyq"
    "xzwij qhdwj aiyge sncou yzgay ucwdf rnqxf yxlym bwsdw hy"
};

void iocExample01Ioc()
{
    printEnigmaSettings(&iocExampleSettings01A, "INDEX OF COINCIDENCE METHOD EXAMPLE 1 - 8 steckers");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=3;
    recipe.scoreListSize    =52;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings01A.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption with 8 steckers
* After step 1: solution on place 5
* After step 2: solution on place 1
* (Note that this method takes significantly longer than iocExample01Ioc(), with the same outcome)
* 
\**************************************************************************************************/
void iocExample01IocR3()
{
    printEnigmaSettings(&iocExampleSettings01A, "INDEX OF COINCIDENCE METHOD EXAMPLE 1 - 8 steckers");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_R3;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=3;
    recipe.scoreListSize    =5;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings01A.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption with 8 steckers
* Works
* 
\**************************************************************************************************/
void iocExample01Ngram2()
{
    printEnigmaSettings(&iocExampleSettings01A, "INDEX OF COINCIDENCE METHOD EXAMPLE 1 - 8 steckers");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_NGRAM;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =3;
    recipe.numberOfSolutions=3;
    recipe.scoreListSize    =60;
    strncpy(recipe.ngramSet, "DE", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings01A.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption with 9 steckers
* Does not work
* After step 1: position ?
* After step 2: position 12, but wrong steckers
* 
\**************************************************************************************************/
// The orignal Gillogly cipher text but other engima settings, 9 steckers
// The original Gillogly method won't find the solution
EnigmaSettings iocExampleSettings02=
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
    "MV PX EZ RW JO BL IU KT AC",
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
    "XEOIW XHDTJ AWYGZ SNCOO YZGAY NTWDA RNXMF YTLOV BVSDW XU"
};


void iocExample02Ioc()
{
    printEnigmaSettings(&iocExampleSettings02, "INDEX OF COINCIDENCE METHOD EXAMPLE 1C - 10 steckers");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "DE", MAX_NGRAMSETSIZE);
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.numberOfSolutions=20;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    setOperation(recipe);
    iocDecodeText(iocExampleSettings02.cipher, 6);
}


/**************************************************************************************************\
* 
* Pretty long english text example generated using https://cryptii.com/pipes/enigma-machine
* Works
* 
\**************************************************************************************************/

// Some pretty large english message with only 4 steckers
// rotor 3 just before step
EnigmaSettings iocExampleSettings03=
{
    3,
    {
        "IV",
        "V",
        "II"
    },
    "UKW B",
    {
        1, 18, 24
    },
    {
        9, 21, 3
    },

    "bq cr di ep",
    "kqloo dyocm rhnhc oexpt wnnpb fibdu cpemy ktgti dobwb etorl ctevu lazab xgvxk ouplq jjqwm"
    "tkroh hiekp uonaw sectb glqse egotr zatwb xhift klpab nzmfk mlmws exulj wxbps ecbec wiits"
    "ztwkz cqrjn wxxin hdida vicjj zvhub kdsdi asapa uulug gskpg ekrzi uhqlm orbxw dlvit zrpmw"
    "crsyu mhvvd iemqc ehpty hnlxe hzkwl swsec clqoq neonx frobn mrizp ehxlw ahvjx bznjt zpxoq"
    "sejdm gpofo cvddi emcom lrnsm vnkpy bwfhq blhwl xizfi euvjr iagmn rdocb nylvn mvvty szuyg"
    "rzntp itzps mfhcv rgtxu udvmi hsizi azicg gzqab ilswh slfif mruln rctub qtgxs slyhx kbmtn"
    "umiyl moycv xykop dudrl bjang yvaoo ujibe acpim tbthy uijdi nhxpr ookgv jzwno vpcqa wjako"
    "xsvjj qitct iosnk aonjz cqksr aqpjy awwrz sjuel wvpfg rgmvz ehalw usdsn jbirb uuksj ychph"
    "ceaqk pljnk yupwr vhnqa wpwzp jbxms cwjeg ghleb bterp rmgys wipuz kowaa pjxkk hypqo mjywg"
    "gtjel mkfin tktcf rdjpl wmfpt nroje lkzhp thuzx mljms tvcea djzkr cwang vspvo aqajh vyucv"
    "oulko qymra uaoyb wwndy mnswn pyufy bmzjs qkyei syxnn kkkhl eyjay ohsqw ltdkg rborj mepgh"
    "knyfj uculb sankm ndhzi grqdy qdeew zdyiv whhta ppfjk dtegd urdqu vtmbp hmxmb lltbw qjbbv"
    "wukdh quraa bkucm rgqsk volqi zeare jfggf hqvyh nwwih dzt"
};

void iocExample03Ioc()
{
    printEnigmaSettings(&iocExampleSettings03, "INDEX OF COINCIDENCE METHOD EXAMPLE 2A");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=1;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings03.cipher, 6);
}

/**************************************************************************************************\
* 
* Example , short english text
* Works pretty well. Result is almost correct:
* ENIGMA MESSAGES CAN BE SOLVXD BY RECOVERING THE MESSAGE KEE SETTINGS THE RING SETTINGS ANY THE 
* PLUG SETTINGS INDIVIDUALTY RECOVERY OF THE MESSAGE KEY SATTING IS SENSITIVE ENOUGH TO DMSTINGUISH
* THE CORRECT ROTOR OUDER
* 
\**************************************************************************************************/

// A short english text, 4 steckers
EnigmaSettings iocExampleSettings04=
{
    3,
    {
        "I",
        "II",
        "IV"
    },
    "UKW C",
    {
        1, 6, 21
    },
    {
        2, 22, 12
    },
    "RW JO BL IU",
    "chyjp rtitl yuzid ooeik hetwk xjvuj fkznw zpyis vahdi ffoid habsa iqooc"
    "ugdvo uvgwf cmkpd edcxs fiiek vxgek dqsuc ivlzv yabac rqriv ixvnz ldaei"
    "qabsb znzqi ndjfn zpyhp rplfo lndpc jvywv ydwjm vaexw plycd xlpbd swath"
    "hwc"
};

void iocExample04Ioc()
{
    printEnigmaSettings(&iocExampleSettings04, "INDEX OF COINCIDENCE METHOD EXAMPLE 3C - Short english text");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=10;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings04.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from Geocaching GC6ZZBB - original text
* 
\**************************************************************************************************/
// GC GC6ZZBB https://www.geocaching.com/geocache/GC6ZZBB The original cipher
IocResults  iocExampleSettings07A=
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

void iocExample07A()
{
    printEnigmaSettings(&iocExampleSettings07A.settings, "INDEX OF COINCIDENCE METHOD EXAMPLE 7A");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=100;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings07A.settings.cipher, 10);
}

/**************************************************************************************************\
* 
* Example from Geocaching GC6ZZBB - response
* 
\**************************************************************************************************/
EnigmaSettings iocExampleSettings07B=
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

    "",
    "DYGCXZMEWSRXKBJCNMKCWWWDYPVIQFISHZPAHTEIGYKRRILG"
    "JQRSMDNZVGWHFSDPEAZLUKIVSTECUXOWORNCQCWJOLGVVYDZ"
    "NMVZJSATYWWVFMKSDHEZUJJIVXHWIWUEJLWQXUICLKPOSHBY"
    "SPZWJGKSZAWOESBALBDGFJDTXRGDSTGAXHVJUFGPBNSWXBUK"
    "ULEVUAYMRJWDDONOLVXHJKTPVABPIDCRODWLKXHZJRQTOWCN"
    "YQEHFSRYXHVUCEUQNRKUTXZNHZUVVXXVNUCFWHNOKZWXQWUQ"
    "WIDRHUBVVLLWZVIXQHROYITEXWZURPSLNLVXQAWJLQJWFGOM"
    "CVWDZDNCAXJBBMRWHXLIMRPAXNLTNQWUZOMNSTJFTKMLUVHL"
    "ZEWHIMBMIZPUMTVYWMTWBORWUVKWDKDRTZGQSIUKZNIQUQEB"
    "UOXXMANUVPGQZYHKURHYNLPCBFDGTQNCWZVKBZTTEVJEGCPD"
    "WHWGPWRJOXCLDUIRJHENXIZWVEACEPRRCNEVHZPJGUVYDYQM"
    "ZEFQFEZNBDGYFHSIHLCQXTUGVTGYEZZOMCIHXZPUHXMXHCEX"
    "RILWVOJIMPKIKAUZGMRSMKIICDOTGGWWUZDGHFPLSNTKKDBW"
    "WSAXDGKNKEYMTQWRBZKANWJUIKZFUVIHHNJTCQFMXOUHXWIN"
    "AEFYPQEHLRRGZMNZOBQHCPVJFRBUMLZFGRNSAXQRKCCXZJHA"
    "EPNIBXIFJDJBXFSOKNAZZHHVNLSYJAZHNJTTBTNFOLREIWPY"
    "CPOIAJNTZJRZHLWVZNDWWTQNQANJZCXXCSSSXKKCFYZHMFYO"
    "RDWVYLMMNKYLFGNNIVCBFSOJLOCBFSOUADIKVPBXUSMIAXMT"
    "SWYHGLHTSTTNTXCMBNWQPEJVSMCXZPQGFIZYHZCAXFXQTALM"
    "CHWDZROPVCLNVCJGNTDOFRLYAUAHSQTTGIYWJHLFFNJPMZSN"
    "BHIHJWDVTDSGNPUJRAAFUHAAEHXKJPBCJSYNCNHGEBPZRWIP"
    "HMZUCWKJCRBAYHJPRUKJQTGOBHXMJEGLTLUIGONSBICRUGLL"
    "XJZYOTIVWVIZHRMLMCTXEGZOKYVKOLJBCIDKMOJLOQWTIYUN"
    "IRCQBDMNCEGSRGPEMHSHGXBBMJGTCBVHXWXDCCVHXXDKUEMC"
    "YIVAKDBPWTKAIESJQANQBIMJCOHDIXLYGUVPPKIZRCZDGPTD"
    "AVYQGOBKKQNNHMLNAYXJZYMAGESZMHRWDEMSXQFBPBSILYHI"
    "ZVVJBBXNKYDRXEWTTQPQDYWZATXQXOUZMWMBCLXTUCAVZVQE"
    "HKRZETISLAMQPGKXTZUHRLZTMNHRVDBPQXUZVTHEARIOKYWY"
    "CLYTQRONXMWKGGEEVMHSMXONDJDJOHKZTAYEIKRNJWGCNSJD"
    "UNYTYFUSNSCDZFNQTVARKRPHQWQPOJRBRDWPJOATYUVCKTWH"
    "OJELYLYLANOMEHRJPXFHOJPLWTYLMOSISYNZRIECGENUJLXU"
    "HSDSEZLKCWBUAIMNBIOSVTPVWBAOXAIMKZIAOFKNJIRRPKXJ"
    "JABIIEETFNWNMRQGDZXPSFTYQJFQSWPAMDRHSQTWYCZSZNTP"
    "OMRWRZTTUEKRQOQRATQWMIKMGWAXGLWKHTUQUKAXMOBJKWHP"
    "VMAUUVGTSRUNZSQDKXFLJODVYAKKZAEWOUXMCOITDQCXZFTW"
    "KVPAETFCXDJZGOYBIFKDBIVVOYUMPNHSXDFONTUUAYNCEWWT"
    "CRNVBHYMFAWIOCUVREPJFTIENKEIOVMTNYGZKEMYGPVPNCNQ"
    "LFYOHGKIRONPMQKWTTZEONVKFVRQCFNPIGSCGKBDYUFRMGED"
    "RGLGYNOOXQQKHPUROJASHZTHDWGQDAPCLYAJCSMSWMHASTDW"
    "PLDYFKQDXXAEWTCUSCXUQHCRJOWWWFIPYOPFVWZWIHQXHKES"
    "VDVHMGARAAUMAJYBWQICALCLVEZBHHKRSLZCPEBBFXXTNWBU"
    "HCYKYUQNBUQDRULSGLSXMAHUHAKNTQCAHAQRMRSWOKHXDPNC"
    "ZKSPAXDJXXPYLHRFDWYFSZNPOLEQWHFWVUQPZKQNEFNCQDTZ"
    "IWIDIALTGEIOXHETPRMDGYZSGBGSFWBLKYNGMUWNEYAZXLKK"
    "NVWVJZJIERPAWQKPIRHMYZAKVRINEPIHRUNZCNUPEMHWAGVW"
    "QNZNPQYWPEVTAXKSTNYOHNXUYPMJGZTDQEGJHZXTFOMQTDTX"
    "DMPHDDKRIQQKOYMCVEDZCHUGDAXPANNXLDYXJFHRYYKQDOZH"
    "ZBYSKXRBMMYOIKFSGCHYLCHMTSWLGZHCKVIBHLNPULINFROV"
    "ABEZIRCZWXKPSAHYOWHEQOQQXKINQGTBHQQKGKMCMAGWSCVH"
    "NHTFDFVNUFAOMYOOJIFFPLVPFLNGKQHWMRZSQDVIHGXJPSWM"
    "XURRDAWNJQNIGBRYSBXJOYKTFFSOSDNKZHTVWRKJXMCWBFAO"
    "MWSKYMUCKNWSTYBUAOJCZCAPZOPVAQJYJBQVFBMKGNETLPAZ"
    "UVHZGMUYTTBPBTJCADGOITPOPIWTINJFVWAZXOMZKZOILJDQ"
    "EZOWUCVYXVZGJCDOASDZXXHEMRBLEMURFLKUQJVYKZMUZXJN"
    "MRSOBJRLWFXWFSUEZDVTZULUZJCNDGKDLIEPOAXPTUVGFDLU"
    "FNQSNWGVARFSZAWZJGVKPRKGTEXBTBBVMVWTNLASQDQMKNED"
    "VBEWALKUYRPFJGSETMIXJYHTGQODEERRFPABOOIBYQOEIAXA"
    "DBMEIBIMQGXTBPFELGLWSYJGTYRVKSGMMUAPLXXOIYEMPLRA"
    "HXZKDFPSQQVXGBQGPSVHGVIKDYIXJGXETRGMKSEDXGCUZKCR"
    "YBFBDLDHGPOFZYXROOGHRXZYYOPDRSJDZYXQOJSUZOGHATUT"
    "PQFLIHEWMFCBULEHSRGWXMXYLIHXHIEHRMQJOLNBXCYEOACU"
    "XJGWWHOYJKEYIBQOAPUAMXYTKMZCMRDMYABEQFBYCSSOFVEY"
    "SDFRHEDJKUQFXYCKISPIIRLXUYIHMNLLYDNVKXWMSCVLUXBN"
    "NINRNKIUWJGYVFOAHFVQAJLGVJKKPILHAMCPGIUZHRTIPFVA"
    "KLCNSGWSDRJRZOAKPEZIBBGPOHZHNCZGSVGJNDETUHNJVUIP"
    "WEMRZBDDARDHQYZPZHDJWRADMNSSE"
};

void iocExample07B()
{
    printEnigmaSettings(&iocExampleSettings07B, "INDEX OF COINCIDENCE METHOD EXAMPLE 7B");

    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    recipe.numberOfSolutions=10;
    recipe.scoreListSize    =TOP_RESULTS_SIZE;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    setOperation(recipe);
    iocDecodeText(iocExampleSettings07B.cipher, 10);
}



