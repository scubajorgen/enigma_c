/**************************************************************************************************\
* 
*  testIoc.c
*  Test the Index of Coincidence functions
* 
\**************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "test.h"
#include "testframe.h"
#include "enigma.h"
#include "coincidence.h"
#include "log.h"

// Original Gillogly cipher, from the article, 7 steckers
char* iocTestCipher01=
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

// Same settings, 10 steckers: AC BL EZ FG IU JO MV PX RW QY
char* iocTestCipher02=
"KKBYWAYTZKGXZOMJGOQRHQUWVBXQTIWMMVWBLEBPTWUWBTVHMR"
"GLKPDAAXXIQPCHVMPZIAVIBRVLNHZOPOFQXIPZSTUFQOSLCOXL"
"HKVARYOHVDTRBPDJFUKSBBOHTTFWHFGIACAVFHVOYGCYWBKXZJ"
"SYJGZJEVJROATOESLBRHYTRCCHXVQCUHTNBFIBVALBLXAQBDMY"
"WTVHQKGGTXNDDIAAJBHYGFKXEEQWZBQXWDXDRDYNIFDSEUJJPV"
"MHULPZGHKLGZRCZHZPILDFBKOYXWTLDVRAWUCEDHAPQJGWZMMT"
"UCMYBNNGAHUSCWAUHNAGQPWUCNBBNIGPHFRDKMDYLMRNMTZOHM"
"CUHRVFAUMYKKYRKDVSWVMTQVNGRDDSKIISXONXYHHLIQYSDGHE"
"NAMCGMBEZYDRPBMRVPYTVRSWZPFLPITRHFBPXXHPRGISZTPDEP"
"LKOTTXNCZMHTEPAHCCOGZLEGAEZUTPQBCOSXPZAJAQBOVEHZZV"
"ELBLLZEVDAHRMIOMKPGDHFNDLTNISXKAHNEJZWVXUSBITDESVA"
"NKYLSNXMOQZFIUPCWGULTZZSGCHMZXPLJNZRXQJNSKQNYWMZBU"
"FGZJAUTWFTYZATLLOIEKCOISFHCYQGOPGUZIRTLWEVQWMDN";

char* expected01=
"AUFBEFEHLDESOBERSTENBEFEHLSHABERSSINDIMFALLEXZXZTXUNWAHRSCHEINLICHEN"
"XFRANZOESISQENANGRIFFSDIEWESTBEFESTIGUNGENJEDERZAHLENMAESKIGENUEBERL"
"EGENHEITZUMTROTZZUHALTENXFUEHRUNGUNDTRUPPEMUESSENVONDIESEREHRENPFLIQ"
"TDURQDRUNGENSEINXABSXDEMGEMAESSBEHALTEIQMIRDIEERMAEQTIGUNGZURPUFGABE"
"DERBEFESTIGUNGENODERAUQVONTEILENAUSDRUECKLIQPERSOENLIQVORXABSXAENDER"
"UNGDERANWEISUNGXOKHXGENXSTXDXHXERSTEABTXNRXDREIDREIZWOEINSXDREIAQTGX"
"KDOSXVOMJULIEINSNEUNDREIAQTBLEIBTVORBEHALTENXDEROBERBEFEHLSHABERDESH"
"EERESKRKRFLOTTENCHEFANOKMMMXXTORPEDOTREFFERACHTERAUSXSCHIFFMANOEVRIE"
"RUNFAEHIGXWIRKAEMPFENBISZURLETZTENGRANATEXESLEBEDERFUEHRERXDERFUEHRE"
"RISTTOTXDERKAMPFGEHTWEITERXDOENITZX";


/**************************************************************************************************\
* 
* Test iocIndexOfCoincidence
* 
\**************************************************************************************************/

void testCoincidenceIndexOfCoincidence(void)
{
    float ioc;

    testStart("IoC");
    Enigma* enigma=createEnigmaM3();

    for (int i=0; i<strlen(testTextRandom); i++)
    {
        enigma->conversion[i]=testTextRandom[i]-'A';
    }
    enigma->textSize=200;
    ioc=iocIndexOfCoincidence(enigma);
    logInfo("IOC Random text %f", ioc);
    assertFloatEquals(0.038442, ioc);

    for (int i=0; i<strlen(testTextMax); i++)
    {
        enigma->conversion[i]=testTextMax[i]-'A';
    }
    enigma->textSize=strlen(testTextMax);
    ioc=iocIndexOfCoincidence(enigma);
    logInfo("IOC Non random text %f", ioc);
    assertFloatEquals(1.000000, ioc);

    for (int i=0; i<strlen(testTextGerman); i++)
    {
        enigma->conversion[i]=testTextGerman[i]-'A';
    }
    enigma->textSize=strlen(testTextGerman);
    ioc=iocIndexOfCoincidence(enigma);
    logInfo("IOC German text %f", ioc);
    assertFloatEquals(0.075729, ioc);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test iocDecodeText
* 
\**************************************************************************************************/
void testCoincidenceDecodeText1(void)
{
    testStart("Decode text");
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.knownSteckers[0] ='\0';
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =5;
    recipe.numberOfSolutions=1;
    recipe.numberOfThreads  =6;
    recipe.cipher           =iocTestCipher01;
    recipe.cipherSize       =MAX_TEXT;
    recipe.displayFormat    =MESSAGEFORMAT_TEXT;
    iocDecodeText(recipe, NULL);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test iocDecodeText - inline steckers, all permutations, takes hours
* 
\**************************************************************************************************/
void testCoincidenceDecodeTextInline1(void)
{
    testStart("Decode - inline1");
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.walzeSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =7;
    recipe.maxSteckersInline=7;
    recipe.knownSteckers[0] ='\0';
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =5;
    recipe.numberOfSolutions=1;
    recipe.numberOfThreads  =6;
    recipe.cipher           =iocTestCipher01;
    recipe.cipherSize       =MAX_TEXT;
    recipe.displayFormat    =MESSAGEFORMAT_TEXT;
    iocDecodeText(recipe, NULL);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test iocDecodeText - inline steckers, just one permutation to reduce solving time
* 
\**************************************************************************************************/
void testCoincidenceDecodeTextInline2(void)
{
    testStart("Decode - inline2");
    // Define recipe using NGRAM for letterfinding -> succeed
    LinkedList*     permutations=linkedListCreate();
    int* permutation   =malloc(4*sizeof(int)); 
    permutation[0]=1; // Index of UKW B
    permutation[1]=1; // II
    permutation[2]=0; // I
    permutation[3]=2; // III
    linkedListAppendObject(permutations, permutation); 

    IocRecipe* recipe=createDefaultIocRecipe(iocTestCipher02, 1);
    recipe->method           =DEPTH_R3;
    recipe->evalWalzen       =EVAL_IOC;
    recipe->evalSteckers     =EVAL_IOC;
    recipe->maxSteckers      =10;
    recipe->maxSteckersInline=10;
    recipe->scoreListSize    =10;
    recipe->numberOfSolutions=1;
    EnigmaSettings* bestSettings=iocDecodeText(*recipe, permutations);

    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, bestSettings);
    encodeDecode(enigma);
    assertStringEquals(expected01, toString(enigma));
    destroyIocRecipe(recipe);
    destroyEnigma(enigma);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testCoincidence()
{
    moduleTestStart("IoC");
    testCoincidenceIndexOfCoincidence();
    testCoincidenceDecodeText1();
    //testCoincidenceDecodeTextInline1();   // takes hours to execute
    //testCoincidenceDecodeTextInline2();     // takes minutes to execute
    moduleTestWrapUp();
}