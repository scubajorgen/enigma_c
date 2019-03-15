#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

#include "enigma.h"
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
* 
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
* 
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
* 
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
* 
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
    
    assertIntEquals("toolbox", 1, 4, stellungToPos('e'));
    assertIntEquals("toolbox", 2, 4, stellungToPos('E'));
    assertIntEquals("toolbox", 3, 4, stellungToPos(5));
    assertIntEquals("toolbox", 4, 9, charToPos('j'));
    assertIntEquals("toolbox", 5, 9, charToPos('J'));
    
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
    
    destroyLinkedList(list);
    
    
    list=createLinkedList();
    permute(list, permutationElements, 4, 2, 0);
    
    assertIntEquals("toolbox", 17, 12, linkedListLength(list));   
    
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
    
    
    
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

void test02(void)
{
    Enigma*     enigma;

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

    setRingStellungen(enigma, "09 10 11");
    assertIntEquals("waltze", 5, 'I'-'A', enigma->ringStellung[2]);
    
    setGrundStellungen(enigma, "STU");
    assertIntEquals("waltze", 6, 'U'-'A', enigma->grundStellung[0]);
    
    setGrundStellungen(enigma, "b c d");
    assertIntEquals("waltze", 7, 'B'-'A', enigma->grundStellung[2]);

    setGrundStellungen(enigma, "26 10 01");
    assertIntEquals("waltze", 8, 'Z'-'A', enigma->grundStellung[2]);
    
    
    destroyEnigma(enigma);    
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void test03(void)
{
    Enigma*     enigma;
    
    enigma=createEnigmaM3();
    
    placeUmkehrWaltze(enigma, "UKW B");

    assertIntEquals("ukw", 1, 23, enigma->umkehrWaltzeFunction[9]);   // J (9) maps to X (23)


    placeUmkehrWaltze(enigma, "UKW C2");

    assertIntEquals("ukw", 2, 23, enigma->umkehrWaltzeFunction[18]);   //  maps to X

    destroyEnigma(enigma);  
    
}

/**************************************************************************************************\
* 
* 
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
* 
* 
\**************************************************************************************************/
void test05(void)
{
    Enigma*     enigma;
    char*       result;

    
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


    // Test 2
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
    
    return 0;
}
