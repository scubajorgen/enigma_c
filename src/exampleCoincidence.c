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

// The orignal Gillogly cipher text and enigma settings, 7 steckers
EnigmaSettings iocExampleSettings01=
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

// The orignal Gillogly cipher text but other engima settings, 8 steckers
// The original Gillogly method won't find the solution
EnigmaSettings iocExampleSettings01_2=
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

// The orignal Gillogly cipher text but other engima settings, 10 steckers
// The original Gillogly method won't find the solution
EnigmaSettings iocExampleSettings01_3=
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


// Some random pretty large message with only 4 steckers
// rotor 3 just before step
EnigmaSettings iocExampleSettings02=
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



IocResults  iocExampleResults10=
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
        " PPFRI MWQHT QSB"
    }
};

// GC voorbeeld
IocResults  iocExampleResults10Test2=
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

// GC GC6ZZBB https://www.geocaching.com/geocache/GC6ZZBB The original cipher
IocResults  iocExampleResults10Test3=
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
IocResults  iocExampleResults10Test4=
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
IocResults  iocExampleResults10Test5=
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

EnigmaSettings iocExampleSettings07=
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

// A short english text
EnigmaSettings iocExampleSettings03=
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
    "chyjp rtitl yuzid ooeik hetwk xjvuj fkznw zpyis vahdi ffoid habsa iqooc ugdvo uvgwf cmkpd edcxs fiiek vxgek dqsuc ivlzv yabac rqriv ixvnz ldaei qabsb znzqi ndjfn zpyhp rplfo lndpc jvywv ydwjm vaexw plycd xlpbd swath hwc"
};

/**************************************************************************************************\
* 
* Example from the original James Gillogly article
* Works. 2nd result is correct
* 
\**************************************************************************************************/
void iocExample00()
{
    printEnigmaSettings(&iocExampleSettings01, "INDEX OF COINCIDENCE METHOD EXAMPLE 0 - James Gillogly");

    setEvaluationMethod(METHOD_IOC, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings01.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption
* Does not work. 5th result comes closest
* 
\**************************************************************************************************/
void iocExample01A()
{
    printEnigmaSettings(&iocExampleSettings01_2, "INDEX OF COINCIDENCE METHOD EXAMPLE 1A - James Gillogly");

    setEvaluationMethod(METHOD_IOC, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings01_2.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption
* Works
* 
\**************************************************************************************************/
void iocExample01B()
{
    printEnigmaSettings(&iocExampleSettings01_2, "INDEX OF COINCIDENCE METHOD EXAMPLE 1B - James Gillogly");

    setEvaluationMethod(METHOD_IOC_R3, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings01_2.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from the original James Gillogly article - other encryption
* ?
* 
\**************************************************************************************************/
void iocExample01C()
{
    printEnigmaSettings(&iocExampleSettings01_3, "INDEX OF COINCIDENCE METHOD EXAMPLE 1C - James Gillogly");

    setEvaluationMethod(METHOD_IOC_R2R3, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings01_3.cipher, 6);
}


/**************************************************************************************************\
* 
* Pretty long english text example generated using https://cryptii.com/pipes/enigma-machine
* Works. 3rd result is correct.
* 
\**************************************************************************************************/
void iocExample02A()
{
    printEnigmaSettings(&iocExampleSettings02, "INDEX OF COINCIDENCE METHOD EXAMPLE 2A");

    setEvaluationMethod(METHOD_IOC, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings02.cipher, 6);
}

/**************************************************************************************************\
* 
* Example from Geocaching GC6ZZBB - response
* 
\**************************************************************************************************/
void iocExample07A()
{
    printEnigmaSettings(&iocExampleSettings07, "INDEX OF COINCIDENCE METHOD EXAMPLE 7A");

    setEvaluationMethod(METHOD_IOC, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings07.cipher, 10);
}

/**************************************************************************************************\
* 
* Example from Geocaching GC6ZZBB - response
* 
\**************************************************************************************************/
void iocExample07B()
{
    printEnigmaSettings(&iocExampleSettings07, "INDEX OF COINCIDENCE METHOD EXAMPLE 7B");

    setEvaluationMethod(METHOD_IOC_R3, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings07.cipher, 10);
}

/**************************************************************************************************\
* 
* Example , short english text
* Works. ?th result is correct
* 
\**************************************************************************************************/
void iocExample03C()
{
    printEnigmaSettings(&iocExampleSettings03, "INDEX OF COINCIDENCE METHOD EXAMPLE 8B - Short english text");

    setEvaluationMethod(METHOD_IOC_R2R3, 10, 10, 0, NULL);

    iocDecodeText(iocExampleSettings03.cipher, 6);
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
    
    settings=&iocExampleResults10.settings;
    
    printEnigmaSettings(settings, "INDEX OF COINCIDENCE METHOD EXAMPLE 3C - IoC");

    setEvaluationMethod(METHOD_IOC_DEEP, 10, 10, 0, NULL);

    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cipher            =settings->cipher;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =40;
    iocWorkItems[0].endPermutation    =40;
    iocWorkItems[0].R1                =1;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCipherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettings(&iocWorkItems[0], 10);

    iocDumpTopResults(1);

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
    
    settings=&iocExampleResults10.settings;
    
    printEnigmaSettings(settings, "INDEX OF COINCIDENCE METHOD EXAMPLE 3C - NGram");

    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 5, 3, "DE");


    // Start with 5 Wehrmacht rotors
    permutations=createRotorPermutations(3, 5);

    iocWorkItems[0].cipher            =settings->cipher;
    iocWorkItems[0].permutations      =permutations;
    iocWorkItems[0].startPermutation  =40;
    iocWorkItems[0].endPermutation    =40;
    iocWorkItems[0].R1                =7;
    iocWorkItems[0].startR2           =24;
    iocWorkItems[0].endR2             =24;
    iocWorkItems[0].startR3           =15;
    iocWorkItems[0].endR3             =15;
    iocWorkItems[0].maxCipherChars    =MAX_TEXT;

    
    strncpy(iocWorkItems[0].ukw, "UKW C", MAX_ROTOR_NAME);

    iocEvaluateEngimaSettings(&iocWorkItems[0], 5);

    iocDumpTopResults(1);

    i=0;
    while (i<iocNumberOfResults)
    {
        iocFindSteckeredCharsNgram(&iocTopResults[i], 10, 3);
        i++;
    }

    iocDumpTopResults(1);

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
    
    settings=&iocExampleResults10.settings;
    printEnigmaSettings(settings, "INDEX OF COINCIDENCE METHOD EXAMPLE 3C - Ngram test");
    
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "DE");

    strncpy(iocExampleResults10.settings.steckers, "AT BG DV EW FR HN IQ JX KZ LU PS", 35);
    printf("\nSteckers %s\n", iocExampleResults10.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults10, 13, 3);
    printf("Steckers %s\n", iocExampleResults10.settings.steckers);
    dumpDecoded(&iocExampleResults10.settings);
    
    strncpy(iocExampleResults10.settings.steckers, "AT DV JX KZ QR IP", 35);
    printf("\nSteckers %s\n", iocExampleResults10.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults10, 13, 3);
    printf("Steckers %s\n", iocExampleResults10.settings.steckers);
    dumpDecoded(&iocExampleResults10.settings);
    
    strncpy(iocExampleResults10.settings.steckers, "AT", 35);
    printf("\nSteckers %s\n", iocExampleResults10.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults10, 13, 3);
    printf("Steckers %s\n", iocExampleResults10.settings.steckers);
    dumpDecoded(&iocExampleResults10.settings);
    
    strncpy(iocExampleResults10.settings.steckers, "", 35);
    printf("\nSteckers %s\n", iocExampleResults10.settings.steckers);
    iocFindSteckeredCharsNgram(&iocExampleResults10, 13, 3);
    printf("Steckers %s\n", iocExampleResults10.settings.steckers);
    dumpDecoded(&iocExampleResults10.settings);
}


/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest2()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 6, 3, "GC");


    printf("Steckers %s\n", iocExampleResults10Test2.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults10Test2, 10, 3);
    
    printf("Steckers %s\n", iocExampleResults10Test2.settings.steckers);
    
    dumpDecoded(&iocExampleResults10Test2.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest3()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "GC");


    printf("Steckers %s\n", iocExampleResults10Test3.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults10Test3, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults10Test3.settings.steckers);
    
    dumpDecoded(&iocExampleResults10Test3.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest4()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 10, 10, 3, "GC");


    printf("Steckers %s\n", iocExampleResults10Test4.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults10Test4, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults10Test4.settings.steckers);
    
    dumpDecoded(&iocExampleResults10Test4.settings);
    
}

/**************************************************************************************************\
* 
* Quicky to test iocFindSteckeredCharsNgram
* 
\**************************************************************************************************/
void ngramTest5()
{
    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 3, 3, "DE");


    printf("Steckers %s\n", iocExampleResults10Test5.settings.steckers);

    iocFindSteckeredCharsNgram(&iocExampleResults10Test5, 13, 3);
    
    printf("Steckers %s\n", iocExampleResults10Test5.settings.steckers);
    
    dumpDecoded(&iocExampleResults10Test5.settings);
    
}
