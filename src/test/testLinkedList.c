/**************************************************************************************************\
* 
* testLinkedList.c
* Test the Linked List implementation
* 
\**************************************************************************************************/

#include <stdio.h>
#include <malloc.h>

#include "log.h"
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
    
    for (int i=0; i<5; i++)
    {
        linkedListAppendObject(list, (void*)(objects+i));
    }
    
    linkedListReset(list);
    int count=0;
    while (linkedListHasNext(list))
    {
        objectFromList  =(int*)linkedListNextObject(list);
        assertIntEquals(objects[count], *objectFromList);
        count++;
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
* Helper 
\**************************************************************************************************/
void printLinkedList(LinkedList* list, LinkedListElement* elements[10])
{
    printf("\nfirst %d\n", *(int*)(list->firstElement->object));
    for(int i=0; i<10; i++)
    {
        LinkedListElement*prv=elements[i]->previous;
        LinkedListElement*nxt=elements[i]->next;
        if (prv!=NULL)
        {
            printf("%d", *(int*)(prv->object));
        }
        else
        {
            printf("-");
        }
        printf(" <- %d -> ",  *(int*)(elements[i]->object));
        if (nxt!=NULL)
        {
            printf("%d\n", *(int*)(nxt->object));
        }
        else
        {
            printf("-\n");
        }
    }
    printf("Last  %d\n", *(int*)(list->lastElement->object));
}

/**************************************************************************************************\
* Helper
\**************************************************************************************************/
void assertLinkedList(LinkedList* list, int expected[10])
{
    LinkedListElement* readElements[10];

    linkedListReset(list);
    int count=0;
    while (linkedListHasNext(list) && count<10)
    {
        readElements[count]=linkedListNext(list);
        count++;
    }
    assertIntEquals(10, count);
    assertIntEquals(0, linkedListHasNext(list));

    printLinkedList(list, readElements);
    for (int i=0; i<10; i++)
    {
        assertIntEquals(expected[i], *(int*)(readElements[i]->object));
        if (i>0)
        {
            assertPointerEquals(readElements[i]->previous, readElements[i-1]);
        }
        else
        {
            assertIsNull(readElements[i]->previous);
        }
        if (i<9)
        {
            assertPointerEquals(readElements[i]->next, readElements[i+1]);
        }
        else
        {
            assertIsNull(readElements[i]->next);
        }
    }
    assertPointerEquals(list->firstElement, readElements[0]);
    assertPointerEquals(list->lastElement , readElements[9]);

    // Test reverse order
    linkedListResetReverse(list);
    count=0;
    while (linkedListHasNext(list) && count<10)
    {
        readElements[count]=linkedListNext(list);
        count++;
    }
    assertIntEquals(10, count);
    assertIntEquals(0, linkedListHasNext(list));
    for (int i=0; i<10; i++)
    {
        assertIntEquals(expected[i], *(int*)(readElements[9-i]->object));
    }
}


/**************************************************************************************************\
* 
* Test swapping of elements
* 
\**************************************************************************************************/
int testSwapObjects[10]  ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int testSwapExpected1[10]={1, 0, 2, 3, 4, 5, 6, 7, 8, 9};
int testSwapExpected2[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int testSwapExpected3[10]={0, 1, 2, 4, 3, 5, 6, 7, 8, 9};
int testSwapExpected4[10]={0, 1, 2, 4, 3, 5, 6, 8, 7, 9};
int testSwapExpected5[10]={0, 6, 2, 4, 3, 5, 1, 8, 7, 9};
int testSwapExpected6[10]={0, 6, 5, 4, 3, 2, 1, 8, 7, 9};
int testSwapExpected7[10]={9, 6, 5, 4, 3, 2, 1, 8, 7, 0};
int testSwapExpected8[10]={9, 6, 5, 4, 3, 2, 1, 8, 0, 7};

void testLinkedListSwap()
{
    testStart("linkedList swap");

    
    LinkedListElement*  elements[10];
    // Linked list
    LinkedList*         list=linkedListCreate();
    // 0 1 2 3 4 5 6 7 8 9 10
    for(int i=0; i<10; i++)
    {
        elements[i]         =malloc(sizeof(LinkedListElement));
        elements[i]->object =(testSwapObjects+i);
        linkedListAppend(list, elements[i]);
    }
    printLinkedList(list, elements);    

    // 1 0 2 3 4 5 6 7 8 9
    linkedListSwap(list, elements[0], elements[1]);
    assertLinkedList(list, testSwapExpected1);

    // 0 1 2 3 4 5 6 7 8 9
    linkedListSwap(list, elements[0], elements[1]);
    assertLinkedList(list, testSwapExpected2);

    // 0 1 2 4 3 5 6 7 8 9
    linkedListSwap(list, elements[3], elements[4]);
    assertLinkedList(list, testSwapExpected3);

    // 0 1 2 4 3 5 6 8 7 9
    linkedListSwap(list, elements[8], elements[7]);
    assertLinkedList(list, testSwapExpected4);

    // 0 6 2 4 3 5 1 8 7 9
    linkedListSwap(list, elements[1], elements[6]);
    assertLinkedList(list, testSwapExpected5);

    // 0 6 5 4 3 2 1 8 7 9
    linkedListSwap(list, elements[5], elements[2]);
    assertLinkedList(list, testSwapExpected6);

    // 9 6 5 4 3 2 1 8 7 0
    linkedListSwap(list, elements[9], elements[0]);
    assertLinkedList(list, testSwapExpected7);

    // 9 6 5 4 3 2 1 8 7 0
    linkedListSwap(list, elements[0], elements[7]);
    assertLinkedList(list, testSwapExpected8);


    linkedListDestroy(list, false);
    testWrapUp();
}


/**************************************************************************************************\
* 
* Test appending of elements
* 
\**************************************************************************************************/
void testLinkedListNext()
{
    testStart("next");

    int         objects[5]={0, 1, 2, 3, 4};
   
    // Linked list
    LinkedList* list=linkedListCreate();
    for (int i=0; i<5; i++)
    {
        linkedListAppendObject(list, (void*)(objects+i));
    }

    int count;
    // Test two times, to test reset
    for (int i=0; i<2; i++)
    {
        // Test Next Element
        linkedListReset(list);
        count=0;
        while (linkedListHasNext(list))
        {
            LinkedListElement* e=linkedListNext(list);
            assertIntEquals(objects[count], *((int*)e->object));
            count++;
        }

        // Test Next Object
        linkedListReset(list);
        count=0;
        while (linkedListHasNext(list))
        {
            int* obj=(int*)linkedListNextObject(list);
            assertIntEquals(objects[count], *obj);
            count++;
        }
    }
    linkedListDestroy(list, false);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test appending of elements, reverse order
* 
\**************************************************************************************************/
void testLinkedListNextReverse()
{
    testStart("next reverse");

    int         objects[5]={0, 1, 2, 3, 4};
   
    // Linked list
    LinkedList* list=linkedListCreate();
    for (int i=0; i<5; i++)
    {
        linkedListAppendObject(list, (void*)(objects+i));
    }

    int count;
    // Test two times, to test reset
    for (int i=0; i<2; i++)
    {
        // Test Next Element
        linkedListResetReverse(list);
        count=0;
        while (linkedListHasNext(list))
        {
            LinkedListElement* e=linkedListNext(list);
            assertIntEquals(objects[4-count], *((int*)e->object));
            count++;
        }

        // Test Next Object
        linkedListResetReverse(list);
        count=0;
        while (linkedListHasNext(list))
        {
            int* obj=(int*)linkedListNextObject(list);
            assertIntEquals(objects[4-count], *obj);
            count++;
        }
    }
    linkedListDestroy(list, false);
    testWrapUp();
}

/**************************************************************************************************\
* 
* Test appending of elements, reverse order
* 
\**************************************************************************************************/
void testLinkedListDestroyLastElement()
{
    testStart("destroy last");
    int         objects[5]={0, 1, 2, 3, 4};
   
    // Linked list
    LinkedList* list=linkedListCreate();
    assertIntEquals(0, linkedListLength(list));
    linkedListDestroyLastElement(list, false);
    assertIntEquals(0, linkedListLength(list));

    for (int i=0; i<5; i++)
    {
        linkedListAppendObject(list, (void*)(objects+i));
    }
    assertIntEquals(5, linkedListLength(list));

    linkedListDestroyLastElement(list, false);
    assertIntEquals(4, linkedListLength(list));

    linkedListReset(list);
    int count=0;
    while (linkedListHasNext(list))
    {
        LinkedListElement* e=linkedListNext(list);
        assertIntEquals(objects[count], *((int*)e->object));
        count++;
    }
    assertIntEquals(4, count);

    linkedListDestroyLastElement(list, false);
    linkedListDestroyLastElement(list, false);
    linkedListDestroyLastElement(list, false);
    assertIntEquals(1, linkedListLength(list));
    linkedListReset(list);
    count=0;
    while (linkedListHasNext(list))
    {
        LinkedListElement* e=linkedListNext(list);
        assertIntEquals(objects[count], *((int*)e->object));
        count++;
    }
    assertIntEquals(1, count);

    linkedListDestroyLastElement(list, false);
    assertIntEquals(0, linkedListLength(list));
    linkedListReset(list);
    assertIntEquals(0, linkedListHasNext(list));

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
    testLinkedListNext();
    testLinkedListNextReverse();
    testLinkedListSwap();
    testLinkedListDestroyLastElement();
    moduleTestWrapUp();
}