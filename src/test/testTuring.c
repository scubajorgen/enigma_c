/**************************************************************************************************\
* 
*  turing.c
*  Test the Turing Bombe functions
* 
\**************************************************************************************************/
#include <malloc.h>

#include "testframe.h"
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
     
    // ENIGMAPN
    // NPPMAGEI
    turingFindLoops("ENIGMAPN","NPPMAGEI", 0); 
    assertIntEquals( 2, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals( 0, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals( 4, cribCircleSet['E'-'A'].numOfCircles);

    assertStringEquals("AMGA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("AGMA", cribCircleSet['A'-'A'].cribCircles[1].orgChars);
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
     
    // Create list of loops
    // DASXISTXEINX
    // VJAREVEADJEV
    turingFindLoops("VJAREVEADJEV","DASXISTXEINX", 0); 
    dumpSets();
    assertIntEquals( 6, cribCircleSet['V'-'A'].numOfCircles);
    assertIntEquals( 4, cribCircleSet['D'-'A'].numOfCircles);
    assertIntEquals( 0, cribCircleSet['T'-'A'].numOfCircles);

    assertStringEquals("SAXVS"   , cribCircleSet['S'-'A'].cribCircles[1].orgChars);
    assertIntEquals   ( 3        , cribCircleSet['S'-'A'].cribCircles[1].advances[0]);
    assertIntEquals   ( 8        , cribCircleSet['S'-'A'].cribCircles[1].advances[1]);
    assertIntEquals   (12        , cribCircleSet['S'-'A'].cribCircles[1].advances[2]);
    assertIntEquals   ( 6        , cribCircleSet['S'-'A'].cribCircles[1].advances[3]);

    assertStringEquals("AJIEDVSA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("AJIEDVXA", cribCircleSet['A'-'A'].cribCircles[1].orgChars);
    assertStringEquals("ASVXA"   , cribCircleSet['A'-'A'].cribCircles[3].orgChars);

    testWrapUp();
}


/**************************************************************************************************\
* 
* Test the loop finding
* 
\**************************************************************************************************/
void testTuringFindLoops()
{
    testStart("findLoops");
    // Array 012345678901
    // Text  PQRSABCCHIJK
    // Crib      CABAP
    // Pos   123456789012
    turingFindLoops("PQRSABCCHIJK","CABAP", 4);

    assertIntEquals     (6, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals     (4, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals     (6, cribCircleSet['C'-'A'].numOfCircles);
    assertIntEquals     (0, cribCircleSet['D'-'A'].numOfCircles);

    assertStringEquals  ("ACBA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals  ("ACA" , cribCircleSet['A'-'A'].cribCircles[1].orgChars);

    assertIntEquals     (3, cribCircleSet['A'-'A'].cribCircles[0].circleSize);
    assertIntEquals     (5, cribCircleSet['A'-'A'].cribCircles[0].advances[0]);
    assertIntEquals     (7, cribCircleSet['A'-'A'].cribCircles[0].advances[1]);
    assertIntEquals     (6, cribCircleSet['A'-'A'].cribCircles[0].advances[2]);


    // Array 012345678901
    // Text  PQRSTAWDQUJWPQR
    // Crib       WDQUJW
    // Pos   123456789012
    turingFindLoops("PQRSTAWDQUJWPQR", "WDQUJW", 5);

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
    free(chars);

    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 5, 20, 1, chars);
    assertIntEquals     (1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     (0, found);
    free(chars);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 1 - two solutions, not all steckers found
* 
\**************************************************************************************************/
// I II III, UKW B, R 1 24 3 G 22 17 12, bq cr di ej kw mt os px uz gh 
char* testTuringPlain1   ="wettervorhersage biskaya x heute gibt es blitz und donnerwetter";
char* testTuringCipher1  ="RPVPZILDGRNOPPLOFZNRUALUGCBJFXYNJCFDCOIUMGABPODMHQGVRFW";
char* testTuringCrib1    ="WETTERVORHERSAGEBISKAYA";
char* testTuringExpected1="KETTERVORAERSAGEQISWAYAXHENTEGIQTESPLITZUNDRONNERTETTEU";

void testTuringBombe1()
{
    testStart("Turing Bombe1");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=createLinkedList();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=0; // I
    p[2]=1; // II
    p[3]=2; // III
    addObject(permutations, (void*)p);

    TuringRecipe* recipe    =createDefaultTuringRecipe(testTuringCipher1, testTuringCrib1, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results     =createLinkedList();
    turingBombe(*recipe, results);
    EnigmaSettings* settings=elementAt(results, 1);

    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    logInfo("Solution returned: %s", toString(enigma));
    assertStringEquals(testTuringExpected1, toString(enigma));
    destroyEnigma(enigma);

    destroyLinkedList(results, true);
    destroyTuringRecipe(recipe);
    
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 2 - two solutions, bit cheating: using just the Steckers that are actually found
* 
\**************************************************************************************************/
// I II III, UKW B, R 1 24 3 G 22 17 12, EJ GH ID OS PX RC TM ZU 
char* testTuringCipher2  ="DPVPZILDGRNOPPLORZNYUALUGCQJFXYRJCFNCOIUMGAQPODMHNGVRFK";
char* testTuringCrib2    ="WETTERVORHERSAGEBISKAYA";
char* testTuringExpected2="WETTERVORHERSAGEBISKAYAXHEUTEGIBTESBLITZUNDDONNERWETTER";

void testTuringBombe2()
{
    testStart("Turing Bombe2");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=createLinkedList();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=0; // I
    p[2]=1; // II
    p[3]=2; // III
    addObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher2, testTuringCrib2, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=createLinkedList();
    turingBombe(*recipe, results);
    EnigmaSettings* settings=elementAt(results, 0);

    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    logInfo("Solution returned: %s", toString(enigma));
    assertStringEquals(testTuringExpected2, toString(enigma));
    destroyEnigma(enigma);

    destroyLinkedList(results, true);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4A - 2 solutions, bit cheating: using just the Steckers that are actually found
* Crib at pos 86
* 
\**************************************************************************************************/

char* testTuringPlain4      ="KOMXADMXUUUBOOTEYFDUUUAUSBXYFDUUUOSTYSSSMMMHHHSSSOSTAZWWFUNF"
                             "XUUUFLOTTXYFUNFXUUUFLOTTXYHAKAXKIELVONVONTORPXFANGBOOTEINSNE"
                             "UNXXEINSZWOUHRJKIELWEINGELAUFENYFFFTTTBLEIBTBESETZTR";
char* testTuringCipher4     ="BCVASCWKNQOSAGCRQEBHJMCFWWLIAMPDAZOMENPQALETJCFPRTQJEFSHIXMY"
                             "CYINKGCEENUZTOAUTNDTCXESNSOKTYZGERELRFMTZLCPSVUHKACZKXXPHQZG"
                             "VQRFJALPDVUJDJWPSQQGIWKTUYNCRISVRRRHDMNYCJZHIXHRUIGZ";
char* testTuringCrib4A      ="HAKAXKIELVONVONTORPXFANGBOOT"; // pos 86
char* testTuringCrib4B      ="KOMXADMXUUUBOOTEYFDUUUAUSBXY";   // pos 0
char* testTuringExpected4A  ="KOMXADMXUUUBOOTEYFDUUUAUSBXYFDUUUOSTYSSSMMMHHHSSSOSTAZWWFUNF"
                             "XUUUFLOTTXYFUNFXUUUFLOTTXYHAKAXKIELVONVONTORPXFANGBOOTEINSNE"
                             "UNXXEINSZWOUHRJKIELWEINGELAUFENYFFFTTTBLEIBTBESETZTR";
char* testTuringExpected4B  ="KOMXADMXUUUBOTTJYWDVXUAUSBXYFDUUUOSTGSSUMMRGAGSSSOSYZZWNFUNF"
                             "XUUUFYOUOXYFUNFXUUUFLOPTXYGAKAXKWJMVONVONTORPXFQNHBOOTJIQLNK"
                             "UNXXEINSZWOQGIEKIJLOJINPJLAUFJNYFFFXTTBLJABLBJNJTBQR";

void testTuringBombe4A()
{
    testStart("Turing Bombe 4A");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=createLinkedList();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=4; // V
    addObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher4, testTuringCrib4A, 86, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=createLinkedList();
    turingBombe(*recipe, results);
    EnigmaSettings* settings=elementAt(results, 1);

    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    logInfo("Solution returned: %s", toString(enigma));
    assertStringEquals(testTuringExpected4A, toString(enigma));
    destroyEnigma(enigma);

    destroyLinkedList(results, true);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4B
* Crib at pos 0 -> 6 results
* Two steckers not found: EJ and GH
* 
\**************************************************************************************************/

void testTuringBombe4B()
{
    testStart("Turing Bombe 4B");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=createLinkedList();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=2; // III
    p[2]=1; // II
    p[3]=4; // V
    addObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher4, testTuringCrib4B, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=createLinkedList();
    turingBombe(*recipe, results);
    assertIntEquals(6, linkedListLength(results));
    EnigmaSettings* settings=elementAt(results, 5);

    Enigma* enigma=createEnigmaM3();
    setEnigma(enigma, settings);
    encodeDecode(enigma);
    logInfo("Solution returned: %s", toString(enigma));
    assertStringEquals(testTuringExpected4B, toString(enigma));
    destroyEnigma(enigma);

    destroyLinkedList(results, true);
    destroyTuringRecipe(recipe);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test Turing Bombe 4D - bit cheating: using just the Steckers that are actually found
* Crib at pos 0, ring R2 changes at char 5, R3 at char 6 (double step)
* Use simulator: https://www.101computing.net/enigma-machine-emulator/
* 
\**************************************************************************************************/

// I III II, R AAA, G QUA, NO steckers                             
char* testTuringCipher4D    ="PHDESHDKGEHWBZRYEYIFWADIEHLULMZPQFNHVDZIWENQNBUTEYJSJYJVPTQV"
                             "ZLZITVCODDIGMWOZMMIAJTIDKEIJRTSTFTGFTGSYUEKAGKTPXCODFIYJOMTD"
                             "OJLOFFZBTBNWDLWTCNXOOLBZDVKAAACXZZRSFRUUNWKCCVYAVOFK";                         

void testTuringBombe4D()
{
    testStart("Turing Bombe 4D");
    // Just one permutation for speed...
    // Note: permutations will be destroyed as part of the process, recipe will not
    LinkedList* permutations=createLinkedList();
    int* p;
    p=malloc(4*sizeof(int));
    p[0]=1; // UKW B
    p[1]=0; // I
    p[2]=2; // III
    p[3]=1; // II
    addObject(permutations, (void*)p);

    TuringRecipe* recipe=createDefaultTuringRecipe(testTuringCipher4D, testTuringCrib4B, 0, 1);
    recipe->customPermutations=permutations;
    LinkedList* results=createLinkedList();
    turingBombe(*recipe, results);

    // Because R3 turns, no solutions are found
    assertIntEquals(0, linkedListLength(results));

    destroyLinkedList(results, true);
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

    LinkedList* positions=turingCribFit(testTuringCrib4B, testTuringCipher4D);
    assertIntEquals(52, linkedListLength(positions));
    assertIntEquals(0, *((int*)elementAt(positions, 0)));
    
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
    testTuringFindLoops();
    testTuringBombe1();
    testTuringBombe2();
    testTuringBombe4A();
    testTuringBombe4B();
    testTuringBombe4D();
    testTuringCribFit();
    moduleTestWrapUp();
}