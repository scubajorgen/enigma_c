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
    "xzwij qhdwj aiyge sncou yzgay ucwdf rnqxf yxlym bwsdw hy";

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
    recipe.scoreListSize    =52;
    recipe.numberOfSolutions=1;
    setOperation(recipe);
    iocDecodeText(iocTestCipher, 6);
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
    //testCoincidenceDecodeText1();
    moduleTestWrapUp();
}