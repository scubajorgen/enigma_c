/**************************************************************************************************\
* crack02.c
* 
* Three problems from Geocaching. theThirdProblem, theFourthProblem, theFifthProblem
* 
\**************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "enigma.h"
#include "log.h"
#include "toolbox.h"
#include "coincidence.h"
#include "ngramScore.h"
#include "generics/workDispatcher.h"

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

/**************************************************************************************************\
* 
* Geocache GCXQHW, Athens enigma
* https://www.geocaching.com/geocache/GCXQHW
* SOLVED
*
\**************************************************************************************************/
// https://www.geocaching.com/geocache/GCXQHW Athents Engima
// Walze settings as indicated, R1=1, R2=2, R3=3, no steckers
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

void gcAthentsEnigma()
{
    printf("#####################################################################################\n");
    printf("# GEOCACHE GCXQHW, Athens enigma\n");
    printf("#####################################################################################\n");

    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // IV III V, UKW B, no steckers R1=1, R2=1, R3=1
    // We create a custom permutations list with just one permutation
    LinkedList*     permutations=createLinkedList();
    int*            permutation =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=3; // IV
    permutation[2]=2; // III
    permutation[3]=4; // V
    addObject(permutations, permutation);

    // program the IoC solver
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =0;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "GC", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =25;
    recipe.numberOfSolutions=3;
    recipe.numberOfThreads  =1;
    recipe.cipher           =GCXQHWTest.cipher;
    recipe.cipherSize       =MAX_TEXT;
    iocDecodeText(recipe, permutations);
}

/**************************************************************************************************\
* Enigma cipher from GC6ZZBB geochache. It is a short cipher.
* FROM THE GEOCACHE WE KNOW FOLLOWING:
* - http://enigmaco.de/enigma/enigma_de.html was used to encrypt; therefore
* - Three Walzen out of I-V
* - R1 = R2 = R3 = 1
* - UKW B
* - up to 13 steckers
* - Language is probably german
* NOT SOLVED
\**************************************************************************************************/
// GC6ZZBB Engima-nano
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

char replyGC6ZZBB_2[]=
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
"WEMRZBDDARDHQYZPZHDJWRADMNSSE";


void gcEnigmaNano()
{
    int numOfThreads=6;

    // Create a list of permutations with only UKW B
    LinkedList* walzenPermutations  =getWalzenPermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    LinkedList* walzePermutations   =createLinkedList();
    int*        permutation         =malloc(sizeof(int));
    permutation[0]                  =1; // Index of UKW B
    addObject(walzePermutations, permutation);
    LinkedList* permutations=combinePermutations(walzePermutations, 1, walzenPermutations, 3);
    destroyPermutations(walzePermutations);
    destroyPermutations(walzenPermutations);

//    setEvaluationMethod(METHOD_IOC, 13, 13, 0, NULL);
//    setEvaluationMethod(METHOD_IOC_DEEP, 13, 13, 3, NULL);
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "DE");
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "GB");
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 0, 3, "DE");
// TO DO
    IocRecipe* recipe=createDefaultIocRecipe(textGC6ZZBB, numOfThreads);
    recipe->method           =DEPTH_R3;
    recipe->evalWalzen       =EVAL_IOC;
    recipe->evalSteckers     =EVAL_IOC;
    recipe->maxSteckers      =13;
    recipe->maxSteckersInline=9;
    recipe->knownSteckers[0] ='\0';
    recipe->ngramSize        =3;
    strncpy(recipe->ngramSet, "DE", MAX_NGRAMSETSIZE);
    recipe->scoreListSize    =400;
    recipe->numberOfSolutions=100;
    iocDecodeText(*recipe, permutations);
    destroyIocRecipe(recipe);
}

/**************************************************************************************************\
* Enigma cipher from GC6ZZBB geochache. It is a short cipher.
* FROM THE GEOCACHE WE KNOW FOLLOWING:
* - http://enigmaco.de/enigma/enigma_de.html was used to encrypt; therefore
* - Three Walzen out of I-V
* - R1 = R2 = R3 = 1
* - UKW B
* - up to 13 steckers
* - Language is probably german
* NOT SOLVED
\**************************************************************************************************/
void gcEnigmaNanoReply()
{
    int numOfThreads=6;

    // Create a list of permutations with only UKW B
    LinkedList* walzenPermutations  =getWalzenPermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    LinkedList* walzePermutations   =createLinkedList();
    int*        permutation         =malloc(sizeof(int));
    permutation[0]=1; // Index of UKW B
    addObject(walzePermutations, permutation);
    LinkedList* permutations=combinePermutations(walzePermutations, 1, walzenPermutations, 3);
    destroyPermutations(walzePermutations);
    destroyPermutations(walzenPermutations);


//    setEvaluationMethod(METHOD_IOC, 13, 13, 0, NULL);
//    setEvaluationMethod(METHOD_IOC_DEEP, 13, 13, 3, NULL);
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "DE");
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 6, 3, "GB");
//    setEvaluationMethod(METHOD_IOC_NGRAM, 13, 0, 3, "DE");
// TO DO
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_R3;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =13;
    recipe.maxSteckersInline=10;
    recipe.knownSteckers[0] ='\0';
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "DE", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =400;
    recipe.numberOfSolutions=10;
    recipe.numberOfThreads  =numOfThreads;
    recipe.cipher           =replyGC6ZZBB;
    recipe.cipherSize       =MAX_TEXT;
    iocDecodeText(recipe, permutations);
}


/**************************************************************************************************\
* 
* Geocache Code Breakers, GC6R1M1, level 5
* Hint: met twee een acht x ab x cd x ef
* SOLVED
*
\**************************************************************************************************/
// Code breakers GC6R1M1
char textCodeBreakers[]= "MCIKVFMLPDWBWMLQRVKEYPSFBVYHLSGSYAFZPXZCBFWPEUMWMBUM"
                         "GCUMKCZPQJEEXEEEWOSBXQJTHQEDNJMSJENPWYSCKWOVPMAYWNQ";

void gcCodeBreakers()
{
    printf("#####################################################################################\n");
    printf("# GEOCACHE GC6R1M1, Code Breakers - Level 5\n");
    printf("#####################################################################################\n");


    // FROM THE GEOCACHE WE KNOW FOLLOWING:
    // III I II, UKW B, 
    // We create a custom permutations list with just one permutation
    LinkedList* permutations=createLinkedList();
    int*        permutation =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=2; // III
    permutation[2]=0; // I
    permutation[3]=1; // II
    addObject(permutations, permutation);

    // program the IoC solver
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =3;
    recipe.maxSteckersInline=0;
    strncpy(recipe.knownSteckers, "AB CD EF", MAX_STECKER_STRING-1);
    recipe.ngramSize        =3;
    strncpy(recipe.ngramSet, "NL", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =25;
    recipe.numberOfSolutions=5;
    recipe.numberOfThreads  =1;
    recipe.cipher           =textCodeBreakers;
    recipe.cipherSize       =MAX_TEXT;
    recipe.displayFormat    =MESSAGEFORMAT_TEXT;
//    recipe.cipher           =text11_test;
    iocDecodeText(recipe, permutations);
}

/**************************************************************************************************\
* 
* Geocache: Enigma, GC7ENW4
* Rad-Auftrag: IV, II, V
* Ring-Einstellungen: 15, 23, 26
* Stecker Anschlüsse: EJ OY IV AQ KW FX MT PS lU BD
* NOT SOLVED
*
\**************************************************************************************************/
// Enigma GC7ENW4
char textGC7ENW4[]= "EKSY YUIC OERV TZZF FVON TTXN ZNOC QTJF CSCZ DJOM ADRV BEZK OHSG RIFH KJEP "
                    "MQMT MKBG FNRN ZXPG OYTX AASW DYOL QJJR JCLN";

void gcEnigma()
{
    printf("#####################################################################################\n");
    printf("# GEOCACHE GC7ENW4, 'Enigma'\n");
    printf("#####################################################################################\n");

    LinkedList* permutations=createLinkedList();
    int*        permutation =malloc(4*sizeof(int));
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=3; // IV
    permutation[3]=4; // V
    addObject(permutations, permutation);
    permutation     =malloc(4*sizeof(int));
    permutation[0]=2; // Index of UKW C
    permutation[1]=1; // II
    permutation[2]=3; // IV
    permutation[3]=4; // V
    addObject(permutations, permutation);

    // program the IoC solver
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =3;
    recipe.maxSteckersInline=0;
    strncpy(recipe.knownSteckers, "EJ OY IV AQ KW FX MT PS LU BD", MAX_STECKER_STRING-1);
    recipe.ngramSize        =0;
    recipe.ngramSet[0]      =0;
    recipe.scoreListSize    =25;
    recipe.numberOfSolutions=25;
    recipe.numberOfThreads  =1;
    recipe.cipher           =textGC7ENW4;
    recipe.cipherSize       =MAX_TEXT;
    recipe.displayFormat    =MESSAGEFORMAT_TEXT;
    iocDecodeText(recipe, permutations);
}



