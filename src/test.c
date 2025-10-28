#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>

#include "enigma.h"
#include "turing.h"
#include "ngramscore.h"
#include "toolbox.h"
#include "workDispatcher.h"

int permutations[12][2]=
{
    {1, 2},
    {1, 3},
    {1, 4},
    {2, 1},
    {2, 3},
    {2, 4},
    {3, 2},
    {3, 1},
    {3, 4},
    {4, 2},
    {4, 3},
    {4, 1}
};
/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertStringEquals(char* test, int testCase, char* expected, char* result)
{
    if (strcmp(expected, result)==0)
    {
        printf("Test %10s %3d: Passed!\n", test, testCase);
    }
    else
    {
        printf("Test %10s %3d: Failed! Expected %s, result was %s\n", test, testCase, expected, result);
        exit(0);
    }
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertIntIsNull(char* test, int testCase, int* result)
{
    if (result==NULL)
    {
        printf("Test %10s %3d: Passed!\n", test, testCase);
    }
    else
    {
        printf("Test %10s %3d: Failed! Expected NULL, result was %ld\n", test, testCase, (long)result);
        exit(0);
    }
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertIntEquals(char* test, int testCase, int expected, int result)
{
    if (expected==result)
    {
        printf("Test %10s %3d: Passed!\n", test, testCase);
    }
    else
    {
        printf("Test %10s %3d: Failed! Expected %d, result was %d\n", test, testCase, expected, result);
        exit(0);
    }
}



/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertLongNotEquals(char* test, int testCase, long notExpected, long result)
{
    if (notExpected!=result)
    {
        printf("Test %10s %3d: Passed!\n", test, testCase);
    }
    else
    {
        printf("Test %10s %3d: Failed! Not expected %ld, result was %ld\n", test, testCase, notExpected, result);
        exit(0);
    }
}

/**************************************************************************************************\
* 
* Helper funcion
* 
\**************************************************************************************************/
void assertFloatEquals(char* test, int testCase, float expected, float result)
{
    if (floorf(expected*10000.0)==floorf(result*10000.0))
    {
        printf("Test %10s %3d: Passed!\n", test, testCase);
    }
    else
    {
        printf("Test %10s %3d: Failed! Expected %f, result was %f\n", test, testCase, expected, result);
        exit(0);
    }
}


/**************************************************************************************************\
* 
* Test the toolbox
* 
\**************************************************************************************************/
void test01()
{
    LinkedList* list;
    int         objects[5]={0, 1, 2, 3, 4};
    int         permutationElements[4]={1, 2, 3, 4};
    int*        permutation;
    int*        objectFromList;
    int         i;
    int         j;
    
    // character/stellung to position [0..25]
    assertIntEquals("toolbox", 1, 4, stellungToPos('e'));
    assertIntEquals("toolbox", 2, 4, stellungToPos('E'));
    assertIntEquals("toolbox", 3, 4, stellungToPos(5));
    assertIntEquals("toolbox", 4, 9, charToPos('j'));
    assertIntEquals("toolbox", 5, 9, charToPos('J'));
    
    // Linked list
    list=createLinkedList();
    assertLongNotEquals("toolbox", 6, (long)NULL, (long)list);
    
    i=0;
    while (i<5)
    {
        addObject(list, (void*)(objects+i));
        i++;
    }
    
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        objectFromList  =(int*)nextLinkedListObject(list);
        assertIntEquals("toolbox", 7+i, i, *objectFromList);
        i++;
    }
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        objectFromList  =(int*)nextLinkedListObject(list);
        assertIntEquals("toolbox", 12+i, i, *objectFromList);
        i++;
    }

    objectFromList=(int *)elementAt(list, 4);
    assertIntEquals("toolbox", 17, 4, *objectFromList);


    objectFromList=(int *)elementAt(list, 2);
    assertIntEquals("toolbox", 18, 2, *objectFromList);
    
    objectFromList=(int *)elementAt(list, 0);
    assertIntEquals("toolbox", 19, 0, *objectFromList);
    
    objectFromList=(int *)elementAt(list, 6);
    assertIntIsNull("toolbox", 20, objectFromList);
    
    destroyLinkedList(list);
    
    // Permutations
    list=createLinkedList();
    permute(list, permutationElements, 4, 2, 0);
    
    assertIntEquals("toolbox", 21, 12, linkedListLength(list));   
    
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        permutation=(int*)nextLinkedListObject(list);
        j=0;
        while (j<2)
        {
            assertIntEquals("toolbox", 18+i*2+j, permutations[i][j], permutation[j]);
            j++;
        }
        free((void *)permutation);
        i++;
    }
    
    destroyLinkedList(list);
    
    list=createRotorPermutations(3, 5);
    assertIntEquals("toolbox", 42, 60, linkedListLength(list));
    
    permutation=elementAt(list, 0);
    
    assertStringEquals("toolbox", 43, "I", walzen[permutation[0]]);
    assertStringEquals("toolbox", 44, "II", walzen[permutation[1]]);
    assertStringEquals("toolbox", 45, "III", walzen[permutation[2]]);

    permutation=elementAt(list, 59);
    
    assertStringEquals("toolbox", 46, "V", walzen[permutation[0]]);
    assertStringEquals("toolbox", 47, "I", walzen[permutation[1]]);
    assertStringEquals("toolbox", 48, "II", walzen[permutation[2]]);
    
    destroyLinkedList(list);
    
}

/**************************************************************************************************\
* 
* Test the Walze functions
* 
\**************************************************************************************************/

void test02(void)
{
    Enigma*     enigma;
    int         stellung;

    enigma=createEnigmaM3();

    placeWalze(enigma, 1, "I");    

    setRingStellung(enigma, 1, 'j'); 
    assertIntEquals("walze", 1, 9, enigma->ringStellung[2]);

    setRingStellung(enigma, 1, 12); 
    assertIntEquals("walze", 2, 11, enigma->ringStellung[2]);
    
    setRingStellungen(enigma, "JKL");
    assertIntEquals("walze", 3, 'L'-'A', enigma->ringStellung[0]);
    
    setRingStellungen(enigma, "p q r");
    assertIntEquals("walze", 4, 'P'-'A', enigma->ringStellung[2]);

    stellung=getRingStellung(enigma, 1);
    assertIntEquals("walze", 5, 'P'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 2);
    assertIntEquals("walze", 6, 'Q'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 3);
    assertIntEquals("walze", 7, 'R'-'A'+1, stellung);

    setRingStellungen(enigma, "09 10 11");
    assertIntEquals("walze", 8, 'I'-'A', enigma->ringStellung[2]);
    
    setGrundStellungen(enigma, "STU");
    assertIntEquals("walze", 9, 'U'-'A', enigma->grundStellung[0]);
    
    setGrundStellungen(enigma, "b c d");
    assertIntEquals("walze",10, 'B'-'A', enigma->grundStellung[2]);

    setGrundStellungen(enigma, "26 10 01");
    assertIntEquals("walze",11, 'Z'-'A', enigma->grundStellung[2]);
    
    destroyEnigma(enigma);    
}

/**************************************************************************************************\
* 
*  Test the Umkehrwalze functions
* 
\**************************************************************************************************/
void test03(void)
{
    Enigma*     enigma;
    
    enigma=createEnigmaM4();
    
    placeUmkehrWalze(enigma, "UKW B");

    assertIntEquals("ukw", 1, 23, enigma->umkehrWalzeFunction[9]);   // J (9) maps to X (23)


    placeUmkehrWalze(enigma, "UKW C2");

    assertIntEquals("ukw", 2, 23, enigma->umkehrWalzeFunction[18]);   //  maps to X

    destroyEnigma(enigma);  
}

/**************************************************************************************************\
* 
* Test the steckerbrett functions
* 
\**************************************************************************************************/
void test04(void)
{
    Enigma*     enigma;
    
    enigma=createEnigmaM3();

    clearSteckerBrett(enigma);
    
    placeSteckers(enigma, "bq gh");
    assertIntEquals("steckers", 1,  9, enigma->steckerBrett[9]);
    assertIntEquals("steckers", 2, 16, enigma->steckerBrett[1]);
    assertIntEquals("steckers", 3,  1, enigma->steckerBrett[16]);
    assertIntEquals("steckers", 4,  6, enigma->steckerBrett[7]);
    assertIntEquals("steckers", 5,  7, enigma->steckerBrett[6]);

    clearSteckerBrett(enigma);

    assertIntEquals("steckers", 6,  7, enigma->steckerBrett[7]);
    
    destroyEnigma(enigma);  
    
}

/**************************************************************************************************\
* 
* Test the Engima M3
* 
\**************************************************************************************************/
void test05(void)
{
    Enigma*     enigma;
    char*       result;

    // Test 1
    enigma=createEnigmaM3();
    setText(enigma, "RCGXFEAJCT");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");
    
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 17);
    setRingStellung(enigma, 3, 12);

    setGrundStellung(enigma, 1, 'A');
    setGrundStellung(enigma, 2, 'B');
    setGrundStellung(enigma, 3, 'C');

    
    clearSteckerBrett(enigma);
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 1, "HEILHITLER", result);
    
    
    // Test 2
    setText(enigma, "mpyjllkozy");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 1);
    setRingStellung(enigma, 3, 1);

    setGrundStellung(enigma, 1, 'P');
    setGrundStellung(enigma, 2, 'D');
    setGrundStellung(enigma, 3, 'U');
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 2, "HEILHITLER", result);


    // Test 3
    setText(enigma, "boot klar x bei j schnoor j etwa zwo siben x nov x sechs nul cbm x proviant bis zwo nul x dez x benoetige glaeser y noch vier klar x stehe marqu bruno bruno zwo funf x lage wie j schaefer j x nnn www funf y eins funf mb steigend y gute sicht vvv j rasch");

    placeWalze(enigma, 1, "VI");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "III");

    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellungen(enigma, "06 23 12");

    setGrundStellungen(enigma, "01 17 12");
    
    placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 3, "YYTHXJLXTEUXAJGMUWMGWAFQLUFTHQMFYEXMGCZDUQGPNNFEHRUUWNGMFMQADFLWZNQVVWQWGXHBGGNXHWFLCOUZPDZAPTUBQHEOQIMFTQJQDKDEFPPUBAGWZZXSCJYXPPWUPSTHNVKAMNACPYAPROEEZFZCTNCHQJUMIELXPUZFTHHYIPAOLYXWXOWQZJQNJLYI", result);
    
    destroyEnigma(enigma);  
}

/**************************************************************************************************\
* 
* Test the Engima M4
* 
\**************************************************************************************************/
void test05B(void)
{
    Enigma*     enigma;
    char*       result;

    // Test 1
    enigma=createEnigmaM4();
    setText(enigma, "xfla kyyn ba");
    placeWalze(enigma, 1, "Beta");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "II");
    placeWalze(enigma, 4, "III");
    
    placeUmkehrWalze(enigma, "UKW B2");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 17);
    setRingStellung(enigma, 3, 12);
    setRingStellung(enigma, 4, 15);

    setGrundStellung(enigma, 1, 'A');
    setGrundStellung(enigma, 2, 'B');
    setGrundStellung(enigma, 3, 'C');
    setGrundStellung(enigma, 4, 'D');

    
    clearSteckerBrett(enigma);
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 1, "HEILHITLER", result);



    // Test 3
    setText(enigma, "boot klar x bei j schnoor j etwa zwo siben x nov x sechs nul cbm x proviant bis zwo nul x dez x benoetige glaeser y noch vier klar x stehe marqu bruno bruno zwo funf x lage wie j schaefer j x nnn www funf y eins funf mb steigend y gute sicht vvv j rasch");

    placeWalze(enigma, 1, "Beta");
    placeWalze(enigma, 2, "VIII");
    placeWalze(enigma, 3, "VI");
    placeWalze(enigma, 4, "II");
    

    placeUmkehrWalze(enigma, "UKW B2");
    
    setRingStellungen(enigma, "23 17 15 15");

    setGrundStellungen(enigma, "02 02 25 04");
    
    placeSteckers(enigma, "aj kf bc rt lm");
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 3, "SFNXFPQKKAISGWNNTBSTLMJOOWMFJXWIMCVRWALUSDDIIVQGXTEOAXZXURQUXPXDKZYNEMDZTDBEUPNKINOJHYUGZTPLKHAAMIZHSIWXGPHPQPTZDNBCFQDWACPFTXJKWVZGIFVBJYIAWFNXIWDIOXLAZDOXHNAPZXPSSJQSTTHKAIDMSCCNSBLYDLBOLRQNDZUO", result);

    destroyEnigma(enigma);
}





/**************************************************************************************************\
* 
* Test the Turing bombe
* 
\**************************************************************************************************/
void test06()
{
    LinkedLetters* link;
    
    turingGenerateLetterLinks("ENIGMAP","NIEMAGE", 0);
    
    link=&menu['E'-'A'];
    assertIntEquals("turing",  1,   3, link->numOfLinks);
    assertIntEquals("turing",  2,   1, link->links[0].position);
    assertIntEquals("turing",  3, 'N', link->links[0].letter);
    assertIntEquals("turing",  4,   3, link->links[1].position);
    assertIntEquals("turing",  5, 'I', link->links[1].letter);
    assertIntEquals("turing",  6,   7, link->links[2].position);
    assertIntEquals("turing",  7, 'P', link->links[2].letter);
     
    // ENIGMAPN
    // NPPMAGEI
    turingFindLoops("ENIGMAPN","NPPMAGEI", 0); 
    assertIntEquals("turing",  8, 2, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals("turing",  9, 0, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals("turing", 10, 4, cribCircleSet['E'-'A'].numOfCircles);

    assertStringEquals("turing", 11, "AMGA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("turing", 12, "AGMA", cribCircleSet['A'-'A'].cribCircles[1].orgChars);
}

void test06b()
{
    LinkedLetters* link;

    // Pos  123456789012
    // Text PQRSABCCHIJK
    // Crib     CABAP
    turingGenerateLetterLinks("PQRSABCCHIJK","CABAP", 4);

    link=&menu['A'-'A'];
    assertIntEquals("tur menu",  1,   3, link->numOfLinks);
    assertIntEquals("tur menu",  2,   5, link->links[0].position);
    assertIntEquals("tur menu",  3, 'C', link->links[0].letter);
    assertIntEquals("tur menu",  4,   6, link->links[1].position);
    assertIntEquals("tur menu",  5, 'B', link->links[1].letter);
    assertIntEquals("tur menu",  6,   8, link->links[2].position);
    assertIntEquals("tur menu",  7, 'C', link->links[2].letter);

    link=&menu['B'-'A'];
    assertIntEquals("tur menu",  8,   2, link->numOfLinks);
    assertIntEquals("tur menu",  9,   6, link->links[0].position);
    assertIntEquals("tur menu", 10, 'A', link->links[0].letter);
    assertIntEquals("tur menu", 11,   7, link->links[1].position);
    assertIntEquals("tur menu", 12, 'C', link->links[1].letter);

    link=&menu['C'-'A'];
    assertIntEquals("tur menu", 13,   3, link->numOfLinks);
    assertIntEquals("tur menu", 14,   5, link->links[0].position);
    assertIntEquals("tur menu", 15, 'A', link->links[0].letter);
    assertIntEquals("tur menu", 16,   7, link->links[1].position);
    assertIntEquals("tur menu", 17, 'B', link->links[1].letter);
    assertIntEquals("tur menu", 18,   8, link->links[2].position);
    assertIntEquals("tur menu", 19, 'A', link->links[2].letter);

    link=&menu['H'-'A'];
    assertIntEquals("tur menu", 20,   1, link->numOfLinks);
    assertIntEquals("tur menu", 21,   9, link->links[0].position);
    assertIntEquals("tur menu", 22, 'P', link->links[0].letter);

    link=&menu['P'-'A'];
    assertIntEquals("tur menu", 23,   1, link->numOfLinks);
    assertIntEquals("tur menu", 24,   9, link->links[0].position);
    assertIntEquals("tur menu", 25, 'H', link->links[0].letter);

    link=&menu['Z'-'A'];
    assertIntEquals("tur menu", 26,   0, link->numOfLinks);
}

void test06c()
{
    // Array 012345678901
    // Text  PQRSABCCHIJK
    // Crib      CABAP
    // Pos   123456789012
    turingFindLoops("PQRSABCCHIJK","CABAP", 4);

    assertIntEquals     ("tur loops",  1, 6, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals     ("tur loops",  2, 4, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals     ("tur loops",  3, 6, cribCircleSet['C'-'A'].numOfCircles);
    assertIntEquals     ("tur loops",  4, 0, cribCircleSet['D'-'A'].numOfCircles);

    assertStringEquals  ("tur loops",  5, "ACBA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals  ("tur loops",  6, "ACA" , cribCircleSet['A'-'A'].cribCircles[1].orgChars);

    assertIntEquals     ("tur loops",  7, 3, cribCircleSet['A'-'A'].cribCircles[0].circleSize);
    assertIntEquals     ("tur loops",  8, 5, cribCircleSet['A'-'A'].cribCircles[0].advances[0]);
    assertIntEquals     ("tur loops",  9, 7, cribCircleSet['A'-'A'].cribCircles[0].advances[1]);
    assertIntEquals     ("tur loops", 10, 6, cribCircleSet['A'-'A'].cribCircles[0].advances[2]);


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
    assertIntEquals     ("tur loops", 11, 1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     ("tur loops", 12, 1, found);
    free(chars);

    chars   =createSteckeredChars();
    found   =turingValidateHypotheses(enigma, 5, 20, 1, chars);
    assertIntEquals     ("tur loops", 13, 1, found);

    found=turingValidateTheSteckeredValues(chars);
    assertIntEquals     ("tur loops", 14, 0, found);
    free(chars);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void test07()
{
    Enigma* enigma;
    
    char trigram[3]="AZZ";
    char bigram[2]="ZZ";
    
    enigma=createEnigmaM3();
    
    enigma->textSize=15;
    enigma->conversion[0]='A'-'A';
    enigma->conversion[1]='Z'-'A';
    enigma->conversion[2]='Z'-'A';
    enigma->conversion[3]='A'-'A';
    enigma->conversion[4]='B'-'A';
    enigma->conversion[5]='C'-'A';
    enigma->conversion[6]='A'-'A';
    enigma->conversion[7]='Z'-'A';
    enigma->conversion[8]='Z'-'A';
    enigma->conversion[9]='Z'-'A';
    enigma->conversion[10]='B'-'A';
    enigma->conversion[11]='P'-'A';
    enigma->conversion[12]='A'-'A';
    enigma->conversion[13]='Z'-'A';
    enigma->conversion[14]='Z'-'A';
    
    assertIntEquals("count",  4, 4, countLetter(enigma, 'A'));   
    assertIntEquals("count",  5, 2, countLetter(enigma, 'B'));   
    assertIntEquals("count",  6, 4, countConvertedChar(enigma, 0));   
    assertIntEquals("count",  7, 2, countConvertedChar(enigma, 1));   

    assertIntEquals("count",  8, 3, countTrigram(enigma, trigram));   
    assertIntEquals("count",  9, 4, countNgram(enigma, bigram, 2));   

    destroyEnigma(enigma);
}


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void test08()
{
    Enigma* enigma;

    enigma=createEnigmaM3();

    prepareNgramScore(3, "DE");

    enigma->textSize=3;
    enigma->conversion[0]='E'-'A';  // ENS - -5.618547
    enigma->conversion[1]='N'-'A';
    enigma->conversion[2]='S'-'A';
    
    assertFloatEquals("ngram",  1, -5.618547, ngramScore(enigma, 3));   
    
    enigma->textSize=8;
    enigma->conversion[0]='E'-'A';  //END -  -5.427283
    enigma->conversion[1]='N'-'A';  //NDE -  -4.939303
    enigma->conversion[2]='D'-'A';  //DEX -  -9.533161
    enigma->conversion[3]='E'-'A';  //EXL - -12.648394
    enigma->conversion[4]='X'-'A';  //XLI - -13.109703
    enigma->conversion[5]='L'-'A';  //LIC -  -6.167728
    enigma->conversion[6]='I'-'A';  
    enigma->conversion[7]='C'-'A';

    assertFloatEquals("ngram",  2, -51.825572, ngramScore(enigma, 3));   

    destroyEnigma(enigma);
}

void test08B()
{
    int i;
    Enigma* enigma;

    char* text01="TOUGEVXHALJYNHDGTYFYTDQEWUNAKSGPUXLKTNIGGTJYFQNNVOOHMSQGMGMONGBNHBCYHJVMTERATBGMNUUQBPAXJWUAPKEHJJSL";
    char* text02="AUFBEFEHLDESOBERSTENBEFEHLSHABERSSINDIMFALLEXZXZTXUNWAHRSCHEINLICHENXFRANZOESISQENANGRIFFSDIEWESTBEF";

    enigma=createEnigmaM3();

    prepareNgramScore(3, "DE");

    enigma->textSize=100;
    for(i=0;i<100;i++)
    {
        enigma->conversion[i]=text01[i]-'A';
    }
    printf("Random: %f\n", ngramScore(enigma, 3));
    for(i=0;i<100;i++)
    {
        enigma->conversion[i]=text02[i]-'A';
    }
    printf("German: %f\n", ngramScore(enigma, 3));

}


/**************************************************************************************************\
*
* Test The double step thing for rotor 2
* QPP -> QPQ -> QQR -> RRS -> RRT -> RRU -> ...
* 
\**************************************************************************************************/
void test09()
{
    Enigma*     enigma;
    char*       result;

    
    enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "I");
    
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'Q');
    setGrundStellung(enigma, 2, 'P');
    setGrundStellung(enigma, 3, 'P');

    
    clearSteckerBrett(enigma);
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("enigma", 4, "ZGOUZFVQWG", result);
}

/**************************************************************************************************\
*
* Test the advance/reverse function
* 
\**************************************************************************************************/
void test10()
{
    Enigma*     enigma;
    
    enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "III");
    placeWalze(enigma, 3, "II");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'Q');
    setGrundStellung(enigma, 2, 'U');
    setGrundStellung(enigma, 3, 'D');
    clearSteckerBrett(enigma);
    
    // Rotor 3
    advances(enigma, 1);
    assertIntEquals("enigma", 5, 'Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 6, 'U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma", 7, 'E', stellungToChar(getGrundStellung(enigma, 3)));
    
    // Rotor 1, 2, 3
    advances(enigma, 1);
    assertIntEquals("enigma", 8, 'Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 9, 'V', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",10, 'F', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 2, 3 (double step of rotor 2)
    advances(enigma, 1);
    assertIntEquals("enigma",11, 'R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",12, 'W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",13, 'G', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 3
    advances(enigma, 1);
    assertIntEquals("enigma",14, 'R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",15, 'W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",16, 'H', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 3
    advances(enigma, 1);
    assertIntEquals("enigma",17, 'R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",18, 'W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",19, 'I', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 3 reverses
    advances(enigma, -1);
    assertIntEquals("enigma",20, 'R', stellungToChar(getGrundStellung(enigma, 1))); 
    assertIntEquals("enigma",21, 'W', stellungToChar(getGrundStellung(enigma, 2))); 
    assertIntEquals("enigma",22, 'H', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 3 reverses
    advances(enigma, -1);
    assertIntEquals("enigma",23, 'R', stellungToChar(getGrundStellung(enigma, 1))); 
    assertIntEquals("enigma",24, 'W', stellungToChar(getGrundStellung(enigma, 2))); 
    assertIntEquals("enigma",25, 'G', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 2, 3 (double step) reverse
    advances(enigma, -1);
    assertIntEquals("enigma",26, 'Q', stellungToChar(getGrundStellung(enigma, 1))); // R
    assertIntEquals("enigma",27, 'V', stellungToChar(getGrundStellung(enigma, 2))); // W
    assertIntEquals("enigma",28, 'F', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 1, 2, 3 reverse
    advances(enigma, -1);
    assertIntEquals("enigma",29, 'Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",30, 'U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",31, 'E', stellungToChar(getGrundStellung(enigma, 3)));

    // Rotor 3 reverse -> returned to start position
    advances(enigma, -1);
    assertIntEquals("enigma",32, 'Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",33, 'U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",34, 'D', stellungToChar(getGrundStellung(enigma, 3)));

    advances(enigma,  52348);
    advances(enigma, -52348);
    assertIntEquals("enigma",35, 'Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma",36, 'U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma",37, 'D', stellungToChar(getGrundStellung(enigma, 3)));

}

/**************************************************************************************************\
*
* Test the advance/reverse function
* 
\**************************************************************************************************/
void test11()
{
    Enigma*     enigma;
    
    enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "IV");
    placeWalze(enigma, 2, "V");
    placeWalze(enigma, 3, "II");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'I');
    setGrundStellung(enigma, 2, 'Y');
    setGrundStellung(enigma, 3, 'H');
    clearSteckerBrett(enigma);
    
    // 
    advances(enigma, 26);
    assertIntEquals("enigma", 38, 'J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 39, 'A', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma", 40, 'H', stellungToChar(getGrundStellung(enigma, 3)));
    
    // 
    advances(enigma, 26);
    assertIntEquals("enigma", 41, 'J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 42, 'B', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma", 43, 'H', stellungToChar(getGrundStellung(enigma, 3)));

    //
    advances(enigma, -26);
    assertIntEquals("enigma", 38, 'J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 39, 'A', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma", 40, 'H', stellungToChar(getGrundStellung(enigma, 3)));

    //
    advances(enigma, -26);
    assertIntEquals("enigma", 38, 'I', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals("enigma", 39, 'Y', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals("enigma", 40, 'H', stellungToChar(getGrundStellung(enigma, 3)));


}

/**************************************************************************************************\
* 
* Test the work dispatcher
* 
\**************************************************************************************************/
char* workItems[]=
{
    "Item 1",
    "Item 2",
    "Item 3"
};

char* finalItem="Final item";


void workFunction(int worker, int workItem, void* params)
{
    char* item;
    item=(char*) params;
    printf("Workfunction: Worker # %d, Work Item# %d, data: %s\n", worker, workItem, item);
    sleep(1);
    fflush(stdout);
}

void finalFunction(void* params)
{
    char* item;
    item=(char*) params;
    printf("Final function %s\n", item);
    assertStringEquals("Dispatcher", 4, finalItem, item);
    fflush(stdout);
}

void test12()
{
    dispatcherClearWorkItems();
    dispatcherPushWorkItem(workFunction, (void*)workItems[0]);
    dispatcherPushWorkItem(workFunction, (void*)workItems[1]);
    dispatcherPushWorkItem(workFunction, (void*)workItems[2]);
    dispatcherStartWork(3, finalFunction, (void*)finalItem);
} 

/**************************************************************************************************\
* 
* Test the toolbox: selectRandomIndices()
* 
\**************************************************************************************************/
void test13()
{
    int available[7]=
    {
        0, 1, 1, 0, 1, 1, 1
    };
    int resultingIndices[3];
    srand(1);
    selectRandomIndices     (available, 7, 3, resultingIndices);
    // Note: might fail on other Systems
    assertIntEquals("toolbox", 49, 5, resultingIndices[0]);
    assertIntEquals("toolbox", 49, 2, resultingIndices[1]);
    assertIntEquals("toolbox", 49, 4, resultingIndices[2]);
}

/**************************************************************************************************\
*
* Test creation of random settings
* 
\**************************************************************************************************/
void test14()
{
    srand(1);

    Enigma* enigma;
    EnigmaSettings* settings;
    enigma=createEnigmaM3();
    settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    printEnigmaSettings(settings, "Test 42");
    assertIntEquals("enigma", 41, 3, settings->numberOfRotors);
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);

    enigma=createEnigmaM4();
    settings=createRandomSettings(enigma, M4_NAVAL_1941, 5);
    printEnigmaSettings(settings, "Test 42");
    assertIntEquals("enigma", 42, 4, settings->numberOfRotors);
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void test15()
{
    Enigma*         enigma;
    EnigmaSettings* settings;
    char*           test    ="TESTSTRING";
    char*           result;
    int             i;
    enigma=createEnigmaM3();
    for (i=0;i<1000;i++)
    {
        settings=createRandomSettings(enigma, M3_ARMY_1938, 5);

        strncpy(settings->cipher, test, MAX_TEXT-1);
        setEnigma(enigma, settings);
        encodeDecode(enigma);
        result=toString(enigma);

        strncpy(settings->cipher, result, MAX_TEXT-1);
        setEnigma(enigma, settings);
        encodeDecode(enigma);
        result=toString(enigma);

        assertStringEquals("enigma", 43, test, result);

        destroyEnigmaSettings(settings);
    }
    destroyEnigma(enigma);

}


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

int main()
{
    printf("###############################################################################\n");
    printf("TEST\n");
    printf("###############################################################################\n");

    test01();
    test02();
    test03();
    test04();
    test05();
    test05B();
    test06();
    test07();
    test08();
    test09();
    test10();
    test11();
    test06b();
    test06c();
    test13();
    test14();
    test15();
    test08B();
    
    test12();

    
    return 0;
}
