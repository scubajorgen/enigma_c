/**************************************************************************************************\
* 
* testLinkedList.c
* Test the Linked List implementation
* 
\**************************************************************************************************/

#include <stdio.h>

#include "testframe.h"
#include "linkedList.h"

/**************************************************************************************************\
* 
* Test the toolbox
* 
\**************************************************************************************************/
void testLinkedListCreate()
{
    testStart("linkedList");

    int         objects[5]={0, 1, 2, 3, 4};
    int*        objectFromList;
    
    // Linked list
    LinkedList* list=createLinkedList();
    assertLongNotEquals((long)NULL, (long)list);
    
    int i=0;
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
        assertIntEquals(objects[i], *objectFromList);
        i++;
    }
    resetLinkedList(list);
    i=0;
    while (hasNext(list))
    {
        objectFromList  =(int*)nextLinkedListObject(list);
        assertIntEquals(objects[i], *objectFromList);
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

    testWrapUp();
}


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testLinkedList()
{
    moduleTestStart("LinkedList");
    testLinkedListCreate();
    moduleTestWrapUp();
}