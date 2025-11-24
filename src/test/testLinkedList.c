/**************************************************************************************************\
* 
* testLinkedList.c
* Test the Linked List implementation
* 
\**************************************************************************************************/

#include <stdio.h>
#include <malloc.h>

#include "testframe.h"
#include "linkedList.h"

/**************************************************************************************************\
* 
* Test the toolbox
* 
\**************************************************************************************************/
void testLinkedListCreate()
{
    testStart("linkedList create");

    int         objects[5]={0, 1, 2, 3, 4};
    int*        objectFromList;
    
    // Linked list
    LinkedList* list=linkedListCreate();
    assertNotNull((void*)list);
    
    int i=0;
    while (i<5)
    {
        linkedListAppendObject(list, (void*)(objects+i));
        i++;
    }
    
    linkedListReset(list);
    i=0;
    while (linkedListHasNext(list))
    {
        objectFromList  =(int*)linkedListNextObject(list);
        assertIntEquals(objects[i], *objectFromList);
        i++;
    }
    linkedListReset(list);
    i=0;
    while (linkedListHasNext(list))
    {
        objectFromList  =(int*)linkedListNextObject(list);
        assertIntEquals(objects[i], *objectFromList);
        i++;
    }

    objectFromList=(int *)linkedListObjectAt(list, 4);
    assertIntEquals(4, *objectFromList);

    objectFromList=(int *)linkedListObjectAt(list, 2);
    assertIntEquals(2, *objectFromList);
    
    objectFromList=(int *)linkedListObjectAt(list, 0);
    assertIntEquals(0, *objectFromList);
    
    objectFromList=(int *)linkedListObjectAt(list, 6);
    assertIsNull((void*)objectFromList);
    
    linkedListDestroy(list, false);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test appending of elements
* 
\**************************************************************************************************/
void testLinkedListAppend()
{
    testStart("linkedList append");

    int         objects[5]={0, 1, 2, 3, 4};
    
    // Linked list
    LinkedList* list=linkedListCreate();

    LinkedListElement* element;

    for(int i=0; i<5; i++)
    {
        element         =malloc(sizeof(LinkedListElement));
        element->object =(objects+i);
        linkedListAppend(list, element);
    }
    assertIntEquals(5, linkedListLength(list));

    linkedListReset(list);
    int count=0;
    while (linkedListHasNext(list))
    {
        int* objectFromList=linkedListNextObject(list);
        assertIntEquals(objects[count], *objectFromList);
        count++;
    }
    linkedListDestroy(list, false);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test appending of elements
* 
\**************************************************************************************************/
void testLinkedListInsert()
{
    testStart("linkedList insert");

    int         objects[6]={0, 1, 2, 3, 4, 5};
    
    // Linked list
    LinkedList* list=linkedListCreate();

    LinkedListElement* element;

    for(int i=2; i<=3; i++)
    {
        element         =malloc(sizeof(LinkedListElement));
        element->object =(objects+i);
        linkedListAppend(list, element);
    }
    assertIntEquals(2, linkedListLength(list));
    // List: 2, 3

    linkedListReset(list);
    LinkedListElement* m=linkedListNext(list); // m=2
    assertIntEquals(2, *((int*)m->object));

    element         =malloc(sizeof(LinkedListElement));
    element->object =(objects+0);
    linkedListInsertBefore(list, element, m);
    // List: 0, 2, 3

    element         =malloc(sizeof(LinkedListElement));
    element->object =(objects+1);
    linkedListInsertBefore(list, element, m);
    // List: 0, 1, 2, 3

    m=list->lastElement; // m=3
    element         =malloc(sizeof(LinkedListElement));
    element->object =(objects+5);
    linkedListInsertAfter(list, element, m);
    // List: 0, 1, 2, 3, 5

    element         =malloc(sizeof(LinkedListElement));
    element->object =(objects+4);
    linkedListInsertAfter(list, element, m);
    // List: 0, 1, 2, 3, 4, 5

    assertIntEquals(6, linkedListLength(list));
    linkedListReset(list);
    int count=0;
    while (linkedListHasNext(list))
    {
        int* objectFromList=linkedListNextObject(list);
        assertIntEquals(objects[count], *objectFromList);
        count++;
    }

    // Test the links in the list
    LinkedListElement* first=list->firstElement;
    for (int i=0; i<6; i++)
    {
        assertIntEquals(objects[i], *((int*)first->object));
        first=first->next;
    }
    LinkedListElement* last =list->lastElement;
    for (int i=0; i<6; i++)
    {
        assertIntEquals(objects[5-i], *((int*)last->object));
        last=last->previous;
    }

    linkedListDestroy(list, false);

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
    testLinkedListAppend();
    testLinkedListInsert();
    moduleTestWrapUp();
}