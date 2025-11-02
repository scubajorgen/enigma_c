/**************************************************************************************************\
* 
* testToolbox.h
* Test the toolbox functions
* 
\**************************************************************************************************/
#include<stdio.h>
#include <stdlib.h>

#include "testframe.h"
#include "toolbox.h"

int permutationsExpected1[12][2]=
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

int testSet1[]={1, 2, 3};
int testSet2[]={4, 5, 6, 7};

int permutationsExpected2[6][2]=
{
    {1, 2},
    {1, 3},
    {2, 1},
    {2, 3},
    {3, 2},
    {3, 1}
};

int permutationsExpected3[24][3]=
{
    {4, 5, 6},
    {4, 5, 7},
    {4, 6, 5},
    {4, 6, 7},
    {4, 7, 6},
    {4, 7, 5},

    {5, 4, 6},
    {5, 4, 7},
    {5, 6, 4},
    {5, 6, 7},
    {5, 7, 6},
    {5, 7, 4},

    {6, 5, 4},
    {6, 5, 7},
    {6, 4, 5},
    {6, 4, 7},
    {6, 7, 4},
    {6, 7, 5},

    {7, 5, 6},
    {7, 5, 4},
    {7, 6, 5},
    {7, 6, 4},
    {7, 4, 6},
    {7, 4, 5}

};

int permutationsExpected4a[2][1]=
{
    {1},
    {2}
};

int permutationsExpected4b[6][2]=
{
    {4, 5},
    {4, 6},
    {5, 4},
    {5, 6},
    {6, 5},
    {6, 4},
};


int permutationsExpected4c[12][3]=
{
    {1, 4, 5},
    {1, 4, 6},
    {1, 5, 4},
    {1, 5, 6},
    {1, 6, 5},
    {1, 6, 4},
    {2, 4, 5},
    {2, 4, 6},
    {2, 5, 4},
    {2, 5, 6},
    {2, 6, 5},
    {2, 6, 4}
};


/**************************************************************************************************\
* 
* Test the toolbox
* 
\**************************************************************************************************/
void testLinkedList()
{
    testStart("linkedList");

    LinkedList* list;
    int*        permutation;
    int         objects[5]={0, 1, 2, 3, 4};
    int         permutationElements[4]={1, 2, 3, 4};
    int*        objectFromList;
    int         i;
    int         j;
    
    // character/stellung to position [0..25]
    assertIntEquals(4, stellungToPos('e'));
    assertIntEquals(4, stellungToPos('E'));
    assertIntEquals(4, stellungToPos(5));
    assertIntEquals(9, charToPos('j'));
    assertIntEquals(9, charToPos('J'));
    
    // Linked list
    list=createLinkedList();
    assertLongNotEquals((long)NULL, (long)list);
    
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
        assertIntEquals(i, *objectFromList);
        i++;
    }
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        objectFromList  =(int*)nextLinkedListObject(list);
        assertIntEquals(i, *objectFromList);
        i++;
    }

    objectFromList=(int *)elementAt(list, 4);
    assertIntEquals(4, *objectFromList);


    objectFromList=(int *)elementAt(list, 2);
    assertIntEquals(2, *objectFromList);
    
    objectFromList=(int *)elementAt(list, 0);
    assertIntEquals(0, *objectFromList);
    
    objectFromList=(int *)elementAt(list, 6);
    assertIntIsNull(objectFromList);
    
    destroyLinkedList(list);
    
    // Permutations
    list=createLinkedList();
    permute(list, permutationElements, 4, 2, 0);
    
    assertIntEquals(12, linkedListLength(list));   
    
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        permutation=(int*)nextLinkedListObject(list);
        j=0;
        while (j<2)
        {
            assertIntEquals(permutationsExpected1[i][j], permutation[j]);
            j++;
        }
        free((void *)permutation);
        i++;
    }
    
    destroyLinkedList(list);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the rotorPermutation function
* 
\**************************************************************************************************/
void testToolboxCreatePermutations()
{
    testStart("create perm");

    LinkedList* permutations1=createPermutations(testSet1, 3, 2);
    assertIntEquals( 6, linkedListLength(permutations1));
    resetLinkedList(permutations1);
    int i=0;
    while (hasNext(permutations1))
    {
        int* permutation=(int*)nextLinkedListObject(permutations1);
        for (int j=0; j<2; j++)
        {
            assertIntEquals(permutationsExpected2[i][j], permutation[j]);
        }
        free((void *)permutation);
        i++;
    }

    LinkedList* permutations2=createPermutations(testSet2, 4, 3);
    assertIntEquals(24, linkedListLength(permutations2));
    i=0;
    while (hasNext(permutations2))
    {
        int* permutation=(int*)nextLinkedListObject(permutations2);
        for (int j=0; j<3; j++)
        {
            assertIntEquals(permutationsExpected3[i][j], permutation[j]);
        }
        free((void *)permutation);
        i++;
    }

    destroyLinkedList(permutations1);
    destroyLinkedList(permutations2);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the rotorPermutation function
* 
\**************************************************************************************************/
void testToolboxCombinePermutations()
{
    testStart("combine perm");
    LinkedList* permutations1=createPermutations(testSet1, 2, 1);
    LinkedList* permutations2=createPermutations(testSet2, 3, 2);
    LinkedList* combined=combinePermutations(permutations1, 1, permutations2, 2);
    assertIntEquals( 2, linkedListLength(permutations1));
    assertIntEquals( 6, linkedListLength(permutations2));
    assertIntEquals(12, linkedListLength(combined));

    resetLinkedList(permutations1);
    int i=0;
    while (hasNext(permutations1))
    {
        int* permutation=(int*)nextLinkedListObject(permutations1);
        int j=0;
        for (j=0; j<1; j++)
        {
            assertIntEquals(permutationsExpected4a[i][j], permutation[j]);
        }
        free((void *)permutation);
        i++;
    }

    resetLinkedList(permutations2);
    i=0;
    while (hasNext(permutations2))
    {
        int* permutation=(int*)nextLinkedListObject(permutations2);
        int j=0;
        for (j=0; j<2; j++)
        {
            assertIntEquals(permutationsExpected4b[i][j], permutation[j]);
        }
        free((void *)permutation);
        i++;
    }

    resetLinkedList(combined);
    i=0;
    while (hasNext(combined))
    {
        int* permutation=(int*)nextLinkedListObject(combined);
        int j=0;
        for (j=0; j<3; j++)
        {
            assertIntEquals(permutationsExpected4c[i][j], permutation[j]);
        }
        free((void *)permutation);
        i++;
    }

    destroyLinkedList(permutations1);
    destroyLinkedList(permutations2);
    destroyLinkedList(combined);

    testWrapUp();
}


/**************************************************************************************************\
* 
* Test the rotorPermutation function
* 
\**************************************************************************************************/
void testToolboxRotorPermutations()
{
    testStart("rotorPermutation");
    LinkedList* list;
    int*        permutation;

    list=createRotorPermutations(3, 5);
    assertIntEquals(60, linkedListLength(list));
    
    permutation=elementAt(list, 0);
    
    assertStringEquals("I", walzen[permutation[0]]);
    assertStringEquals("II", walzen[permutation[1]]);
    assertStringEquals("III", walzen[permutation[2]]);

    permutation=elementAt(list, 59);
    
    assertStringEquals("V", walzen[permutation[0]]);
    assertStringEquals("I", walzen[permutation[1]]);
    assertStringEquals("II", walzen[permutation[2]]);
    
    destroyLinkedList(list);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the toolbox: selectRandomIndices()
* 
\**************************************************************************************************/
void testToolboxRandomIndices()
{
    testStart("random indices");
    int available[7]=
    {
        0, 1, 1, 0, 1, 1, 1
    };
    int resultingIndices[3];
    srand(1);
    selectRandomIndices     (available, 7, 3, resultingIndices);
    // Note: might fail on other Systems
    assertIntEquals(5, resultingIndices[0]);
    assertIntEquals(2, resultingIndices[1]);
    assertIntEquals(4, resultingIndices[2]);
    testWrapUp();
}


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testToolbox()
{
    moduleTestStart("toolbox");
    testLinkedList();
    testToolboxCreatePermutations();
    testToolboxCombinePermutations();
    testToolboxRotorPermutations();
    testToolboxRandomIndices();
    moduleTestWrapUp();
}