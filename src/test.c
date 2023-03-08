#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>

#include "enigma.h"
#include "turing.h"
#include "ngramscore.h"
#include "toolbox.h"

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
    
    assertStringEquals("toolbox", 43, "I", waltzen[permutation[0]]);
    assertStringEquals("toolbox", 44, "II", waltzen[permutation[1]]);
    assertStringEquals("toolbox", 45, "III", waltzen[permutation[2]]);

    permutation=elementAt(list, 59);
    
    assertStringEquals("toolbox", 46, "V", waltzen[permutation[0]]);
    assertStringEquals("toolbox", 47, "I", waltzen[permutation[1]]);
    assertStringEquals("toolbox", 48, "II", waltzen[permutation[2]]);
    
    destroyLinkedList(list);
    
}

/**************************************************************************************************\
* 
* Test the Waltze functions
* 
\**************************************************************************************************/

void test02(void)
{
    Enigma*     enigma;
    int         stellung;

    enigma=createEnigmaM3();

    placeWaltze(enigma, 1, "I");    

    setRingStellung(enigma, 1, 'j'); 
    assertIntEquals("waltze", 1, 9, enigma->ringStellung[2]);

    setRingStellung(enigma, 1, 12); 
    assertIntEquals("waltze", 2, 11, enigma->ringStellung[2]);
    
    setRingStellungen(enigma, "JKL");
    assertIntEquals("waltze", 3, 'L'-'A', enigma->ringStellung[0]);
    
    setRingStellungen(enigma, "p q r");
    assertIntEquals("waltze", 4, 'P'-'A', enigma->ringStellung[2]);

    stellung=getRingStellung(enigma, 1);
    assertIntEquals("waltze", 5, 'P'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 2);
    assertIntEquals("waltze", 6, 'Q'-'A'+1, stellung);
    stellung=getRingStellung(enigma, 3);
    assertIntEquals("waltze", 7, 'R'-'A'+1, stellung);

    setRingStellungen(enigma, "09 10 11");
    assertIntEquals("waltze", 8, 'I'-'A', enigma->ringStellung[2]);
    
    setGrundStellungen(enigma, "STU");
    assertIntEquals("waltze", 9, 'U'-'A', enigma->grundStellung[0]);
    
    setGrundStellungen(enigma, "b c d");
    assertIntEquals("waltze",10, 'B'-'A', enigma->grundStellung[2]);

    setGrundStellungen(enigma, "26 10 01");
    assertIntEquals("waltze",11, 'Z'-'A', enigma->grundStellung[2]);
    
    destroyEnigma(enigma);    
}

/**************************************************************************************************\
* 
*  Test the Umkehrwaltze functions
* 
\**************************************************************************************************/
void test03(void)
{
    Enigma*     enigma;
    
    enigma=createEnigmaM4();
    
    placeUmkehrWaltze(enigma, "UKW B");

    assertIntEquals("ukw", 1, 23, enigma->umkehrWaltzeFunction[9]);   // J (9) maps to X (23)


    placeUmkehrWaltze(enigma, "UKW C2");

    assertIntEquals("ukw", 2, 23, enigma->umkehrWaltzeFunction[18]);   //  maps to X

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
    placeWaltze(enigma, 1, "I");
    placeWaltze(enigma, 2, "II");
    placeWaltze(enigma, 3, "III");
    
    placeUmkehrWaltze(enigma, "UKW B");
    
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

    placeWaltze(enigma, 1, "VI");
    placeWaltze(enigma, 2, "I");
    placeWaltze(enigma, 3, "III");

    placeUmkehrWaltze(enigma, "UKW C");
    
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
* Test the Turing bombe
* 
\**************************************************************************************************/
void test06()
{
    LinkedLetters* link;
    
    turingGenerateLetterLinks("ENIGMAP","NIEMAGE");
    
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
    turingFindLoops("ENIGMAPN","NPPMAGEI"); 
    assertIntEquals("turing",  8, 2, cribCircleSet['A'-'A'].numOfCircles);
    assertIntEquals("turing",  9, 0, cribCircleSet['B'-'A'].numOfCircles);
    assertIntEquals("turing", 10, 4, cribCircleSet['E'-'A'].numOfCircles);

    assertStringEquals("turing", 11, "AMGA", cribCircleSet['A'-'A'].cribCircles[0].orgChars);
    assertStringEquals("turing", 12, "AGMA", cribCircleSet['A'-'A'].cribCircles[1].orgChars);
        
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
    placeWaltze(enigma, 1, "I");
    placeWaltze(enigma, 2, "I");
    placeWaltze(enigma, 3, "I");
    
    placeUmkehrWaltze(enigma, "UKW B");
    
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
    placeWaltze(enigma, 1, "I");
    placeWaltze(enigma, 2, "III");
    placeWaltze(enigma, 3, "II");
    placeUmkehrWaltze(enigma, "UKW B");
    
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
    placeWaltze(enigma, 1, "IV");
    placeWaltze(enigma, 2, "V");
    placeWaltze(enigma, 3, "II");
    placeUmkehrWaltze(enigma, "UKW B");
    
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
    test06();
    test07();
    test08();
    test09();
    test10();
    test11();
    
    return 0;
}
