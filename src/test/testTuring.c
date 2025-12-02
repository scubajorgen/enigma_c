/**************************************************************************************************\
* 
*  turing.c
*  Test the Turing Bombe functions
* 
\**************************************************************************************************/
#include <malloc.h>

#include "testframe.h"
#include "toolbox.h"
#include "log.h"
#include "enigma.h"
#include "turing.h"

/**************************************************************************************************\
* 
* Test the letter links
* 
\**************************************************************************************************/
void testTuringLetterLinks(void)
{
    testStart("LetterLinks 1");
    LinkedLetters* link;
    
    turingGenerateLetterLinks("ENIGMAP","NIEMAGE", 0);
    
    link=&menu['E'-'A'];
    assertIntEquals(  3, link->numOfLinks);
    assertIntEquals(  1, link->links[0].position);
    assertIntEquals( 'N', link->links[0].letter);
    assertIntEquals(  3, link->links[1].position);
    assertIntEquals( 'I', link->links[1].letter);
    assertIntEquals(  7, link->links[2].position);
    assertIntEquals( 'P', link->links[2].letter);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the letter links
* 
\**************************************************************************************************/
void testTuringLetterLinks2()
{
    testStart("LetterLinks 2");
    LinkedLetters* link;

    // Pos  123456789012
    // Text PQRSABCCHIJK
    // Crib     CABAP
    turingGenerateLetterLinks("PQRSABCCHIJK","CABAP", 4);

    link=&menu['A'-'A'];
    assertIntEquals(  3, link->numOfLinks);
    assertIntEquals(  5, link->links[0].position);
    assertIntEquals('C', link->links[0].letter);
    assertIntEquals(  6, link->links[1].position);
    assertIntEquals('B', link->links[1].letter);
    assertIntEquals(  8, link->links[2].position);
    assertIntEquals('C', link->links[2].letter);

    link=&menu['B'-'A'];
    assertIntEquals(  2, link->numOfLinks);
    assertIntEquals(  6, link->links[0].position);
    assertIntEquals('A', link->links[0].letter);
    assertIntEquals(  7, link->links[1].position);
    assertIntEquals('C', link->links[1].letter);

    link=&menu['C'-'A'];
    assertIntEquals(  3, link->numOfLinks);
    assertIntEquals(  5, link->links[0].position);
    assertIntEquals('A', link->links[0].letter);
    assertIntEquals(  7, link->links[1].position);
    assertIntEquals('B', link->links[1].letter);
    assertIntEquals(  8, link->links[2].position);
    assertIntEquals('A', link->links[2].letter);

    link=&menu['H'-'A'];
    assertIntEquals(  1, link->numOfLinks);
    assertIntEquals(  9, link->links[0].position);
    assertIntEquals('P', link->links[0].letter);

    link=&menu['P'-'A'];
    assertIntEquals(  1, link->numOfLinks);
    assertIntEquals(  9, link->links[0].position);
    assertIntEquals('H', link->links[0].letter);

    link=&menu['Z'-'A'];
    assertIntEquals(  0, link->numOfLinks);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the letter links - Example from the Carter article
* 
\**************************************************************************************************/
void testTuringLetterLinks3(void)
{
    testStart("LetterLinks 3");
    LinkedLetters* link;
    
    // Generate links between letters
    turingGenerateLetterLinks("VJAREVEADJEV","DASXISTXEINX", 0);
    
    link=&menu['E'-'A'];
    assertIntEquals(  4, link->numOfLinks);

    assertIntEquals(  5, link->links[0].position);
    assertIntEquals( 'I', link->links[0].letter);
    assertIntEquals(  7, link->links[1].position);
    assertIntEquals( 'T', link->links[1].letter);
    assertIntEquals(  9, link->links[2].position);
    assertIntEquals( 'D', link->links[2].letter);
    assertIntEquals( 11, link->links[3].position);
    assertIntEquals( 'N', link->links[3].letter);
    testWrapUp();
}


/**************************************************************************************************\
* 
* Test the loop finding
* Three loops are present:
* A (  5) C (  7) B (  6) A
* A (  8) C (  7) B (  6) A
* A (  5) C (  8) A
* 
\**************************************************************************************************/
void testTuringFindCribCircles()
{
    testStart("findLoops");
    // Array 012345678901
    // Text  PQRSABCCHIJK
    // Crib      CABAP
    // Pos   123456789012
    turingFindCribCircles("PQRSABCCHIJK","CABAP", 4);

    assertIntEquals     (3, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals     (2, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals     (3, cribCircleSet['C'-'A'].numOfCircles);
    assertIntEquals     (0, cribCircleSet['D'-'A'].numOfCircles);

    assertStringEquals  ("ACBA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals  ("ACA" , cribCircleSet['A'-'A'].cribCircles[1].orgChars);
    assertStringEquals  ("ABCA", cribCircleSet['A'-'A'].cribCircles[2].orgChars);

    assertIntEquals     (3, cribCircleSet['A'-'A'].cribCircles[0].circleSize);
    assertIntEquals     (5, cribCircleSet['A'-'A'].cribCircles[0].advances[0]);
    assertIntEquals     (7, cribCircleSet['A'-'A'].cribCircles[0].advances[1]);
    assertIntEquals     (6, cribCircleSet['A'-'A'].cribCircles[0].advances[2]);

    assertIntEquals     (2, cribCircleSet['A'-'A'].cribCircles[1].circleSize);
    assertIntEquals     (5, cribCircleSet['A'-'A'].cribCircles[1].advances[0]);
    assertIntEquals     (8, cribCircleSet['A'-'A'].cribCircles[1].advances[1]);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the loop finding
* Three Crib Circles
* A (  2) J ( 10) I (  5) E (  9) D (  1) V (  6) S (  3) A
* A (  2) J ( 10) I (  5) E (  9) D (  1) V ( 12) X (  8) A
* A (  3) S (  6) V ( 12) X (  8) A
* 
\**************************************************************************************************/
void testTuringFindCribCircles2()
{
    testStart("findLoops 2");
    // Create list of loops
    // DASXISTXEINX
    // VJAREVEADJEV
    turingFindCribCircles("VJAREVEADJEV","DASXISTXEINX", 0);
    assertIntEquals( 3, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals( 3, cribCircleSet['V'-'A'].numOfCircles);
    assertIntEquals( 2, cribCircleSet['D'-'A'].numOfCircles);
    assertIntEquals( 0, cribCircleSet['T'-'A'].numOfCircles);

    assertStringEquals("AJIEDVSA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("AJIEDVXA", cribCircleSet['A'-'A'].cribCircles[1].orgChars);
    assertStringEquals("ASVXA"   , cribCircleSet['A'-'A'].cribCircles[2].orgChars);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the loop finding
* Four Crib Circles
* SET A -   1
* A (  5) M (  4) G (  6) A
* SET E -   2
* E (  1) N (  2) P (  7) E
* E (  1) N (  8) I (  3) P (  7) E
* SET I -   1
* I (  3) P (  2) N (  8) I
* 
\**************************************************************************************************/
void testTuringFindCribCircles3()
{
    testStart("findLoops 3");
    // ENIGMAPN
    // NPPMAGEI
    turingFindCribCircles("ENIGMAPN","NPPMAGEI", 0); 

    assertIntEquals( 1, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals( 0, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals( 2, cribCircleSet['E'-'A'].numOfCircles);
    assertIntEquals( 2, cribCircleSet['I'-'A'].numOfCircles);

    assertStringEquals("AMGA" , cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("ENPE" , cribCircleSet['E'-'A'].cribCircles[0].orgChars);
    assertStringEquals("ENIPE", cribCircleSet['E'-'A'].cribCircles[1].orgChars);
    assertStringEquals("IPNI" , cribCircleSet['I'-'A'].cribCircles[0].orgChars);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Helper print found Steckered chars
* 
\**************************************************************************************************/
void printChars(SteckeredChars* chars)
 {
    char buffer[MAX_POSITIONS+1];
    for (int i=0; i<MAX_POSITIONS; i++)
    {
        buffer[i]=chars[i].startChar;
    }
    buffer[MAX_POSITIONS]='\0';
    logInfo("letter: %s", buffer);
    for (int i=0; i<MAX_POSITIONS; i++)
    {
        buffer[i]=chars[i].foundChar;
    }
    buffer[MAX_POSITIONS]='\0';
    logInfo("found : %s", buffer);
 }



/**************************************************************************************************\
* 
* Test the hypothesis validation
* One loop present
* D (  7) W ( 11) J ( 10) U (  9) Q (  8) D
* Occurs for D, J, Q, U, W
* 
\**************************************************************************************************/
void testTuringValidateHypothesis()
{
    testStart("hypothesis");
    // Array  012345678901
    // Cipher PQRSTAWDQUJWPQR
    // Crib        WDQUJW
    // Plain  twfjgwdqujwjshp   // G 5 20 12
    // Plain  yavirzjztqvefbc   // G 5 20 1
    // Pos    123456789012
    turingFindCribCircles("PQRSTAWDQUJWPQR", "WDQUJW", 5);

    Enigma* enigma=createEnigmaM3();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");
    
    placeUmkehrWalze(enigma, "UKW B");
    clearSteckerBrett(enigma);
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');  

    SteckeredChars* chars;
    int             found;
    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 5, 20, 12, chars);
    assertIntEquals     (1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (1, found);
    printChars(chars);
    free(chars);

    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 5, 20, 1, chars);
    assertIntEquals     (1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (0, found);
    printChars(chars);
    free(chars);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the hypothesis validation
* Lot of loops present
* 
\**************************************************************************************************/
void testTuringValidateHypothesis2()
{
    testStart("hypothesis2");
    // I II III, UKW B, R 1 24 3 G 22 17 12, bq cr di ej kw mt os px uz gh 
    // Array  0123456789012345678901234
    // Cipher RPVPZILDGRNOPPLOFZNRUALUGCBJFXYNJCFDCOIU
    // Crib   WETTERVORHERSAGEBISKAYA
    // Plain  WETTERVORHERSAGEBISKAYAXHEUTEGIBTESBLITZ
    // Pos    1234567890123456789012345
    turingFindCribCircles("RPVPZILDGRNOPPLOFZNRUALUG", "WETTERVORHERSAGEBISKAYA", 0);

    Enigma* enigma=createEnigmaM3();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");
    
    placeUmkehrWalze(enigma, "UKW B");
    clearSteckerBrett(enigma);
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 24);
    setRingStellung(enigma, 3, 3);  

    SteckeredChars* chars;
    int             found;
    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 22, 17, 12, chars);
    assertIntEquals     (1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (1, found);
    printChars(chars);

    //  Chars contains:
    //  ABCDEFGHIJKLMNOPQRSTUVWXYZ
    //  A???J?H?D??L?NSX?COM?V???U
    //  Found steckers      : EJ GH DI OS PX RC UZ TM'
    //  Not found steckers  : BQ KW
    //  Unsteckered         : A L N V

    assertIntEquals     ('A', chars['A'-'A'].foundChar);
    assertIntEquals     ('?', chars['B'-'A'].foundChar);
    assertIntEquals     ('?', chars['C'-'A'].foundChar);
    assertIntEquals     ('?', chars['D'-'A'].foundChar);
    assertIntEquals     ('J', chars['E'-'A'].foundChar);
    assertIntEquals     ('?', chars['F'-'A'].foundChar);
    assertIntEquals     ('U', chars['Z'-'A'].foundChar);

    free(chars);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the turingValidateTheSteckeredValues()
* 
\**************************************************************************************************/
void testTuringValidateTheSteckeredValues()
{
    testStart("validate steckers");

    bool found;

    SteckeredChars* chars   =createSteckeredChars();
    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (1, found);

    chars['D'-'A'].foundChar='P'; // Stecker 1
    chars['P'-'A'].foundChar='D';
    chars['A'-'A'].foundChar='Z'; // Stecker 2
    chars['Z'-'A'].foundChar='A';
    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (1, found);

    chars['X'-'A'].foundChar='Q'; // Stecker 3 - invalid, same letter
    chars['Y'-'A'].foundChar='Q';    
    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (0, found);

    chars['X'-'A'].foundChar='Y'; // Stecker 3 - invalid, not mutual
    chars['Y'-'A'].foundChar='R';    
    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (0, found);

    chars['X'-'A'].foundChar='R'; // Stecker 3 - invalid, not mutual
    chars['Y'-'A'].foundChar='X';    
    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (0, found);
    printChars(chars);
    free(chars);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test isEqual method for loops
* 
\**************************************************************************************************/
CribCircle testTuringCircle1=
{
    3,
    {1,2,3},
    {'A', 'B', 'C'}
};

CribCircle testTuringCircle1Equal1=
{
    3,
    {1,2,3},
    {'A', 'B', 'C'}
};

CribCircle testTuringCircle1Equal2=
{
    3,
    {3,2,1},
    {'A', 'C', 'B'}
};

CribCircle testTuringCircle1Equal3=
{
    3,
    {3,1,2},
    {'C', 'A', 'B'}
};

CribCircle testTuringCircle1Equal4=
{
    3,
    {2,1,3  },
    {'C', 'B', 'A'}
};

CribCircle testTuringCircle1NotEqual1=
{
    4,
    {1,2,3,4},
    {'A', 'B', 'D', 'C'}
};

CribCircle testTuringCircle1NotEqual2=
{
    3,
    {1,2,5},
    {'A', 'B', 'E'}
};

CribCircle testTuringCircle1NotEqual3=
{
    3,
    {6,2,3},
    {'D', 'A', 'C'}
};

CribCircle testTuringCircle1NotEqual4=
{
    3,
    {1,2,8},
    {'A', 'B', 'C'}
};

CribCircle testTuringCircle2=
{
    9,
    {1,2,3,4,5,6,7,8,9},
    {'A', 'B', 'C', 'K', 'P', 'Z', 'D', 'G', 'T'}
};

CribCircle testTuringCircle2Equal1=
{
    9,
    {6,7,8,9,1,2,3,4,5},
    {'Z', 'D', 'G', 'T','A', 'B', 'C', 'K', 'P'}
};

CribCircle testTuringCircle2Equal2=
{
    9,
    {5,4,3,2,1,9,8,7,6},
    {'Z', 'P', 'K', 'C','B', 'A', 'T', 'G', 'D'}
};

void testTuringIsEqual()
{
    testStart("isEqual");

    assertIntEquals(true , turingIsEqual(&testTuringCircle1, &testTuringCircle1Equal1));
    assertIntEquals(true , turingIsEqual(&testTuringCircle1, &testTuringCircle1Equal2));
    assertIntEquals(true , turingIsEqual(&testTuringCircle1, &testTuringCircle1Equal3));
    assertIntEquals(true , turingIsEqual(&testTuringCircle1, &testTuringCircle1Equal4));
    assertIntEquals(true , turingIsEqual(&testTuringCircle2, &testTuringCircle2Equal1));
    assertIntEquals(true , turingIsEqual(&testTuringCircle2, &testTuringCircle2Equal2));

    assertIntEquals(false, turingIsEqual(&testTuringCircle1, &testTuringCircle1NotEqual1));
    assertIntEquals(false, turingIsEqual(&testTuringCircle1, &testTuringCircle1NotEqual2));
    assertIntEquals(false, turingIsEqual(&testTuringCircle1, &testTuringCircle1NotEqual3));
    assertIntEquals(false, turingIsEqual(&testTuringCircle1, &testTuringCircle1NotEqual4));
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 1 - two solutions, not all steckers found based on Cribs
* 
\**************************************************************************************************/

char* testTuringPlain1   ="wettervorhersage biskaya x heute gibt es blitz und donnerwetter";

// I II III, UKW B, R 1 24 3 G 22 17 12, bq cr di ej kw mt os px uz gh 
char* testTuringCipher1  ="RPVPZILDGRNOPPLOFZNRUALUGCBJFXYNJCFDCOIUMGABPODMHQGVRFW";

char  testTuringCrib1[]  ="WeTTeRVoRHeRSaGeBiSKaYa";

char* testTuringExpected="WETTERVORHERSAGEBISKAYAXHEUTEGIBTESBLITZUNDDONNERWETTER";

void testTuringBombe1()
{
    testStart("Turing Bombe 1");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=0; // I
    p[2]=1; // II
    p[3]=2; // III
    linkedListAppendObject(permutations, (void*)p);

    TuringRecipe* recipe    =createDefaultTuringRecipe(testTuringCipher1, testTuringCrib1, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results     =linkedListCreate();
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_WEHRMACHT);

    assertIntEquals(1, linkedListLength(results));
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringExpected, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4_1 - 2 valid solutions!
* Bit cheating: using just the Steckers that are actually found
* Crib at pos 86
* 
\**************************************************************************************************/
char* testTuringPlain4      ="KOMXADMXUUUBOOTEYFDUUUAUSBXYFDUUUOSTYSSSMMMHHHSSSOSTAZWWFUNF"
                             "XUUUFLOTTXYFUNFXUUUFLOTTXYHAKAXKIELVONVONTORPXFANGBOOTEINSNE"
                             "UNXXEINSZWOUHRJKIELWEINGELAUFENYFFFTTTBLEIBTBESETZTR";
// III II V, UKW B, ?
char* testTuringCipher4A    ="BCVASCWKNQOSAGCRQEBHJMCFWWLIAMPDAZOMENPQALETJCFPRTQJEFSHIXMY"
                             "CYINKGCEENUZTOAUTNDTCXESNSOKTYZGERELRFMTZLCPSVUHKACZKXXPHQZG"
                             "VQRFJALPDVUJDJWPSQQGIWKTUYNCRISVRRRHDMNYCJZHIXHRUIGZ";
// I III II, UKW B, R AAA, G QUA, NO steckers                             
char* testTuringCipher4B    ="PHDESHDKGEHWBZRYEYIFWADIEHLULMZPQFNHVDZIWENQNBUTEYJSJYJVPTQV"
                             "ZLZITVCODDIGMWOZMMIAJTIDKEIJRTSTFTGFTGSYUEKAGKTPXCODFIYJOMTD"
                             "OJLOFFZBTBNWDLWTCNXOOLBZDVKAAACXZZRSFRUUNWKCCVYAVOFK";        
// III II IV, UKW B, R 24 6 21, G 1 1 26, CY DZ EQ HU PK RW SB AV TJ ON
char* testTuringCipher4C     ="TUEASPGSTOCCPQOPJNFLQZMDCHOPNXVZZAIVFYOWQCJKKWXDWBMUYOKBKGRPPMDNUTQFPPT"
                              "ASQAQDZKPJHUHTSTDFZTXLSCEBPYMMPFBMSTIFCIEIUKCGAAZCQVFDCMVELXOBNUDVUEEBG"
                              "MQFBWXGWMHXOXREWIUOBPFHFTUEKFL";


char* testTuringCrib4A      ="HAKAXKIELVONVONTORPXFANGBOOT"; // pos 86
char* testTuringCrib4B      ="KOMXADMXUUUBOOTEYFDUUUAUSBXY";   // pos 0
char* testTuringExpected4A  ="KOMXADMXUUUBOOTEYFDUUUAUSBXYFDUUUOSTYSSSMMMHHHSSSOSTAZWWFUNF"
                             "XUUUFLOTTXYFUNFXUUUFLOTTXYHAKAXKIELVONVONTORPXFANGBOOTEINSNE"
                             "UNXXEINSZWOUHRJKIELWEINGELAUFENYFFFTTTBLEIBTBESETZTR";
char* testTuringExpected4B  ="KOMXADMXUUUBOTTJYWDVXUAUSBXYFDUUUOSTGSSUMMRGAGSSSOSYZZWNFUNF"
                             "XUUUFYOUOXYFUNFXUUUFLOPTXYGAKAXKWJMVONVONTORPXFQNHBOOTJIQLNK"
                             "UNXXEINSZWOQGIEKIJLOJINPJLAUFJNYFFFXTTBLJABLBJNJTBQR";


void testTuringBombe4_1()
{
    testStart("Turing Bombe 4_1");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=4; // V
    linkedListAppendObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher4A, testTuringCrib4A, 86, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=linkedListCreate();
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);

    assertIntEquals(2, linkedListLength(results));
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringExpected4A, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4_2
* Crib at pos 0 -> 6 candidates, 1 valid result
* 
\**************************************************************************************************/
void testTuringBombe4_2()
{
    testStart("Turing Bombe 4-2");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=4; // V
    linkedListAppendObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher4A, testTuringCrib4B, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=linkedListCreate();
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);

    assertIntEquals(1, linkedListLength(results));
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringExpected4A, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4_3 - No steckers used
* Crib at pos 0, ring R2 changes at char 5, R3 at char 6 (double step)
* Use simulator: https://www.101computing.net/enigma-machine-emulator/
* No solutions found because of the 3rd ring moveing
* 
\**************************************************************************************************/
void testTuringBombe4_3()
{
    testStart("Turing Bombe 4_3");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=0; // I
    p[2]=2; // III
    p[3]=1; // II
    linkedListAppendObject(permutations, (void*)p);

    TuringRecipe* recipe        =createDefaultTuringRecipe(testTuringCipher4B, testTuringCrib4B, 0, 1);
    recipe->customPermutations  =permutations;
    LinkedList* results         =linkedListCreate();
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);

    // Because R3 turns, no solutions are found
    assertIntEquals(0, linkedListLength(results));

    // Take into account Ringstellung of R2
    recipe->startR2='A';
    recipe->endR2  ='C';
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);

    // Now one solution is found
    assertIntEquals(1, linkedListLength(results));
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringExpected4A, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the hypothesis validation
* One loop present
* 
\**************************************************************************************************/
void testTuringValidateHypothesis3()
{
    testStart("hypothesis3");
    // I III II, UKW B, R AAA, G QUA, NO steckers 
    // Array  0123456789012345678901234
    // Cipher PHDESHDKGEHWBZRYEYIFWADIEHLULMZPQFNHVD
    // Crib   KOMXADMXUUUBOOTEYFDUUUAUSBXY
    // Plain  
    // Pos    1234567890123456789012345
    turingFindCribCircles(testTuringCipher4B, testTuringCrib4B, 0);

    Enigma* enigma=createEnigmaM3();
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "III");
    placeWalze(enigma, 3, "II");
    
    placeUmkehrWalze(enigma, "UKW B");
    clearSteckerBrett(enigma);
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 1);
    setRingStellung(enigma, 3, 1);  

    SteckeredChars* chars;
    int             found;
    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 17, 21, 1, chars);
    assertIntEquals     (1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (1, found);
    printChars(chars);

    //  Chars contains (no Steckers):
    //  ABCDEFGHIJKLMNOPQRSTUVWXYZ
    //  AB?DEF?HI???M?O???S?U?W?Y?
    assertIntEquals     ('A', chars['A'-'A'].foundChar);
    assertIntEquals     ('B', chars['B'-'A'].foundChar);
    assertIntEquals     ('?', chars['C'-'A'].foundChar);
    assertIntEquals     ('Y', chars['Y'-'A'].foundChar);
    free(chars);
    testWrapUp();
}



/**************************************************************************************************\
* 
* Test turing bombe without knowledge on where to look for the CRIB, 2 valid results
* 
\**************************************************************************************************/
void testTuringBombe4_4()
{
    testStart("Turing Bombe++ 4_4");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=4; // V
    linkedListAppendObject(permutations, (void*)p);

    // Request scanning
    TuringRecipe* recipe        =createDefaultTuringRecipe(testTuringCipher4A, testTuringCrib4A, -1, 1);
    recipe->startCribPosition   =80;
    recipe->endCribPosition     =90;
    recipe->customPermutations  =permutations;
    LinkedList* results=linkedListCreate();
    turingBombe(*recipe, results);
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);

    assertIntEquals(2, linkedListLength(results));
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringExpected4A, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test turing bombe without knowledge on where to look for the CRIB
* 
\**************************************************************************************************/
void testTuringBombe4_5()
{
    testStart("Turing Bombe++ 4_2");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=linkedListCreate();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=3; // IV
    linkedListAppendObject(permutations, (void*)p);

    // Request scanning
    TuringRecipe* recipe        =createDefaultTuringRecipe(testTuringCipher4C, testTuringCrib4A, -1, 1);
    recipe->startCribPosition   =80;
    recipe->endCribPosition     =90;
    recipe->startR2             ='F';   // Cheat
    recipe->endR2               ='F';
    recipe->customPermutations  =permutations;
    LinkedList* results=linkedListCreate();
    turingBombe(*recipe, results);
    assertIntEquals(1, linkedListLength(results));
    turingReport(MESSAGEFORMAT_KRIEGSMARINE);
 
    TuringResult* best=linkedListObjectAt(results, 0);
    assertStringEquals(testTuringPlain4, best->decoded);

    linkedListDestroy(results, true);
    destroyPermutations(recipe->customPermutations);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Crib fit
* 
\**************************************************************************************************/

void testTuringCribFit()
{
    testStart("Turing CribFit");

    LinkedList* positions=turingCribFit(testTuringCrib4B, testTuringCipher4B);
    assertIntEquals(52, linkedListLength(positions));
    assertIntEquals(0, *((int*)linkedListObjectAt(positions, 0)));
    
    testWrapUp();
}  


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testTuring()
{
    moduleTestStart("Turing bombe");

    testTuringLetterLinks();
    testTuringLetterLinks2();
    testTuringLetterLinks3();
    testTuringIsEqual();
    testTuringFindCribCircles();
    testTuringFindCribCircles2();
    testTuringFindCribCircles3();
    testTuringValidateHypothesis();
    testTuringValidateHypothesis2();
    testTuringValidateHypothesis3();
    testTuringValidateTheSteckeredValues();
    testTuringBombe1();
    testTuringBombe4_1();
    testTuringBombe4_2();
    testTuringBombe4_3();
    testTuringBombe4_4();
    testTuringBombe4_5();
    testTuringCribFit();

    moduleTestWrapUp();
}