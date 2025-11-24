/**************************************************************************************************\
* 
* linkedList.c
* Implements a generic linked list of objects. The linked list element stores a pointer to its
* object. 
*
\**************************************************************************************************/

#include <malloc.h>
#include <stdlib.h>
#include "linkedList.h"
#include "log.h"

/**************************************************************************************************\
* 
* Creates an emtpy linked list
* 
\**************************************************************************************************/
LinkedList* linkedListCreate()
{
    LinkedList* list;
    
    list                =malloc(sizeof(LinkedList));
    list->firstElement  =NULL;
    list->lastElement   =NULL;
    list->length        =0;
    list->next          =NULL;
    
    return list;
}

/**************************************************************************************************\
* 
* Returns the number of linked list elements in the linked list
* 
\**************************************************************************************************/
int linkedListLength(LinkedList* list)
{
    return list->length;
}

/**************************************************************************************************\
* 
* Returns the element from the linked list at position indicated by index
* This is an expensive method, especially for long linked lists. If parsing a linked list, use
* linkedListNextObject().
* 
\**************************************************************************************************/
void* linkedListObjectAt(LinkedList* list, int index)
{
    LinkedListElement*  element;
    void*               object;
    int                 i;
    
    object=NULL;
    
    if (index>=0 && index<list->length)
    {
        element=list->firstElement;
        i=0;
        while (i<index)
        {
            element=element->next;
            i++;
        }
        object=element->object;
    }
    else
    {
        logError("Illegal element (index %d) requested from linked list (size %d)", index, list->length);
    }
    return object;
}


/**************************************************************************************************\
* 
* Appends an new linked list element to the linked list
* 
\**************************************************************************************************/
void linkedListAppend(LinkedList* list, LinkedListElement* element)
{
    element->next    =NULL;

    // Check if list is empty
    if (list->firstElement==NULL)
    {
        // empty: add first element
        element->previous           =NULL;
        list->firstElement          =element;
        list->lastElement           =element;
        list->next                  =element;
    }
    else
    {
        LinkedListElement* listEnd  =list->lastElement;
        element->previous           =listEnd;
        listEnd->next               =element;
        list->lastElement           =element;
    }
    list->length++;
}

/**************************************************************************************************\
* 
* Insert an new linked list element to the linked list, after the 'after' element
* Before: after -> next
* After : after -> element -> next
* 
\**************************************************************************************************/
void linkedListInsertAfter(LinkedList* list, LinkedListElement* element, LinkedListElement* after)
{
    if (after==list->lastElement)
    {
        linkedListAppend(list, element);
    }
    else
    {
        LinkedListElement* next =after->next;
        after->next             =element;
        element->previous       =after;
        next ->previous         =element;
        element->next           =next;
        list->length++;
    }
}

/**************************************************************************************************\
* 
* Insert an new linked list element to the linked list, before the 'before' element
* Before: prev -> before
* After : prev -> element -> before
* 
\**************************************************************************************************/
void linkedListInsertBefore(LinkedList* list, LinkedListElement* element, LinkedListElement* before)
{
    if (before==list->firstElement)
    {
        element->previous   =NULL;
        element->next       =before;
        before ->previous   =element;
        list->firstElement  =element;
    }
    else
    {
        LinkedListElement* prev =before->previous;
        prev->next              =element;
        element->previous       =prev;
        element->next           =before;
        before->previous        =element;
    }
    list->length++;
}


/**************************************************************************************************\
* 
* Appends an object to the linked list. An element is created for the object and it is appended
* 
\**************************************************************************************************/
void linkedListAppendObject(LinkedList* list, void* newObject)
{
    LinkedListElement* newElement=malloc(sizeof(LinkedListElement));
    newElement->object  =newObject;
    linkedListAppend(list, newElement);
}


/**************************************************************************************************\
* 
* Destroy the linked list objects and the linked list itself. It does not destroy the objects 
* themselves
* 
\**************************************************************************************************/
void linkedListDestroy(LinkedList* list, bool destroyObjects)
{
    LinkedListElement* element;
    LinkedListElement* nextElement;
    
    element=list->lastElement;
    while (element!=NULL)
    {
        nextElement=element->previous;
        if (destroyObjects)
        {
            free(element->object);
        }
        free(element);
        element=nextElement;
    }
    free(list);
}

/**************************************************************************************************\
* 
* Resets the linked list pointer to the 1st linked list element
* 
\**************************************************************************************************/
void linkedListReset(LinkedList* list)
{
    list->next=list->firstElement;
}

/**************************************************************************************************\
* 
* Get next element from the list
* 
\**************************************************************************************************/
LinkedListElement* linkedListNext(LinkedList* list)
{
    LinkedListElement* nextElement =list->next;
    if (nextElement!=NULL)
    {
        list->next  =nextElement->next;
    }
    return nextElement;
}


/**************************************************************************************************\
* 
* Retrieve the object from next linked list element and increases the linked list pointer.
* 
\**************************************************************************************************/
void* linkedListNextObject(LinkedList* list)
{
    LinkedListElement* nextElement =list->next;
    void* object    =NULL;
    
    if (nextElement!=NULL)
    {
        object      =nextElement->object;
        list->next  =nextElement->next;
    }
    return object;
}

/**************************************************************************************************\
* 
* Inidicates whether there are more items in the linked list
* 
\**************************************************************************************************/
int linkedListHasNext(LinkedList* list)
{
    int linkedListHasNext;
    
    if (list->next==NULL)
    {
        linkedListHasNext=0;
    }
    else
    {
        linkedListHasNext=1;
    }
    return linkedListHasNext;
}
