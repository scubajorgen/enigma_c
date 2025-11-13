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
* Test the conversion functions
* 
\**************************************************************************************************/
void testToolboxStellungPos()
{
    testStart("stellung/pos");

    // character/stellung to position [0..25]
    assertIntEquals(4, stellungToPos('e'));
    assertIntEquals(4, stellungToPos('E'));
    assertIntEquals(4, stellungToPos(5));
    assertIntEquals(9, charToPos('j'));
    assertIntEquals(9, charToPos('J'));

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the permute function
* 
\**************************************************************************************************/
void testToolboxPermute()
{
    int         permutationElements[4]={1, 2, 3, 4};

    testStart("permute");
    // Permutations
    LinkedList* list=createLinkedList();
    permute(list, permutationElements, 4, 2, 0);
    
    assertIntEquals(12, linkedListLength(list));   
    
    resetLinkedList(list);
    int i=0;
    while (hasNext(list))
    {
        int* permutation=(int*)nextLinkedListObject(list);
        int j=0;
        while (j<2)
        {
            assertIntEquals(permutationsExpected1[i][j], permutation[j]);
            j++;
        }
        i++;
    }

    resetLinkedList(list);
    while (hasNext(list))
    {
        int* permutation=(int*)nextLinkedListObject(list);
        free(permutation);
        i++;
    }
    destroyLinkedList(list);
    testWrapUp();
}


/**************************************************************************************************\
* 
* Test the walzePermutation function
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
        i++;
    }
    destroyPermutations(permutations1);
    destroyPermutations(permutations2);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the walzePermutation function
* 
\**************************************************************************************************/
void testToolboxCombinePermutations()
{
    testStart("combine perm");
    LinkedList* permutations1   =createPermutations(testSet1, 2, 1);
    LinkedList* permutations2   =createPermutations(testSet2, 3, 2);
    LinkedList* combined        =combinePermutations(permutations1, 1, permutations2, 2);
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
        i++;
    }

    destroyPermutations(permutations1);
    destroyPermutations(permutations2);
    destroyPermutations(combined);

    testWrapUp();
}


/**************************************************************************************************\
* 
* Test the walzePermutation function
* 
\**************************************************************************************************/
void testToolboxWalzePermutations()
{
    testStart("walzePermutation");
    LinkedList* list;
    int*        permutation;

    list=createWalzePermutations(3, 5);
    
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
    // TO DO: improve
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
    testToolboxStellungPos();
    testToolboxPermute();
    testToolboxCreatePermutations();
    testToolboxCombinePermutations();
    testToolboxWalzePermutations();
    testToolboxRandomIndices();
    moduleTestWrapUp();
}