/**************************************************************************************************\
* 
* testToolbox.h
* Test the toolbox functions
* 
\**************************************************************************************************/

#include <stdlib.h>

#include "testframe.h"
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
            assertIntEquals(permutations[i][j], permutation[j]);
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
void testRotorPermutations()
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
    testRotorPermutations();
    testToolboxRandomIndices();
    moduleTestWrapUp();
}