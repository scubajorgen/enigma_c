/**************************************************************************************************\
* 
*  testIoc.c
*  Test the Index of Coincidence functions
* 
\**************************************************************************************************/
#include <string.h>
#include "testframe.h"
#include "enigma.h"
#include "coincidence.h"

char* iocTestCipher=
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

char* expected=
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
* Test 
* 
\**************************************************************************************************/
void testCoincidenceDecodeText1(void)
{
    testStart("Decode text 1");
    IocRecipe recipe;
    recipe.enigmaType       =ENIGMATYPE_M3;
    recipe.rotorSet         =M3_ARMY_1938;
    recipe.method           =DEPTH_NONE;
    recipe.evalWalzen       =EVAL_IOC;
    recipe.evalSteckers     =EVAL_IOC;
    recipe.maxSteckers      =10;
    recipe.maxSteckersInline=0;
    recipe.ngramSize        =0;
    strncpy(recipe.ngramSet, "none", MAX_NGRAMSETSIZE);
    recipe.scoreListSize    =5;
    recipe.numberOfSolutions=1;
    recipe.numberOfThreads  =6;
    recipe.cipher           =iocTestCipher;
    recipe.displayFormat    =MESSAGEFORMAT_TEXT;
    iocDecodeText(recipe, NULL);
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
    testCoincidenceDecodeText1();
    moduleTestWrapUp();
}