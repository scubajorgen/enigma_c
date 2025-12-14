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
    list->isReverse     =false;
    
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
* This function swaps two elements in the linked list. It adjusts all points
* Lot of pointer magic...
* 
\**************************************************************************************************/
void linkedListSwap(LinkedList* list, LinkedListElement* element1, LinkedListElement* element2)
{
    LinkedListElement* prv1=element1->previous;
    LinkedListElement* nxt1=element1->next;
    LinkedListElement* prv2=element2->previous;
    LinkedListElement* nxt2=element2->next; 

    // ELEMENT 1
    if (prv1==NULL)
    {
        if (prv2==element1)  // = nxt2==element2
        {
            element1->previous  =element2;
            element2->next      =element1;
        }
        else
        {
            element1->previous  =prv2;
            prv2->next          =element1;
        }
        list->firstElement=element2;
    }
    else if (prv2==element1)
    {
        element1->previous      =element2;
        element2->next          =element1;
    }
    else
    {
        element1->previous      =prv2;
        if (prv2!=NULL)
        {
            prv2->next          =element1;
        }
        else
        {
            list->firstElement  =element1;
        }
    }

    if (nxt1==NULL)
    {
        if (nxt2==element1)
        {
            element1->next      =element2;
            element2->previous  =element1;
        }
        else
        {
            element1->next      =nxt2;
            nxt2->previous      =element1;
        }
        list->lastElement       =element2;
    }
    else if (nxt2==element1)
    {
        element1->next          =element2;
        element2->previous      =element1;
    }
    else
    {
        element1->next          =nxt2;
        if (nxt2!=NULL)
        {
            nxt2->previous      =element1;
        }
        else
        {
            list->lastElement   =element1;
        }
    }

    // ELEMENT 2
    if (prv2==NULL)
    {
        if (prv1==element2)  // = nxt1==element1
        {
            element2->previous  =element1;
            element1->next      =element2;
        }
        else
        {
            element2->previous  =prv1;
            prv1->next          =element2;
        }
        list->firstElement      =element1;
    }
    else if (prv1==element2)
    {
        element2->previous      =element1;
        element1->next          =element2;
    }
    else
    {
        element2->previous      =prv1;
        if (prv1!=NULL)
        {
            prv1->next          =element2;
        }
        else
        {
            list->firstElement  =element2;
        }
    }

    if (nxt2==NULL)
    {
        if (nxt1==element2)
        {
            element2->next      =element1;
            element1->previous  =element2;
        }
        else
        {
            element2->next      =nxt1;
            nxt1->previous      =element2;
        }
        list->lastElement=element1;
    }    
    else if (nxt1==element2)
    {
        element2->next          =element1;
        element1->previous      =element2;
    }
    else
    {
        element2->next              =nxt1;
        if (nxt1!=NULL)
        {
            nxt1->previous      =element2;
        }
        else
        {
            list->lastElement=element2;
        }
    }
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
* Resets the linked list pointer to the 1st linked list element for normal iterating
* 
\**************************************************************************************************/
void linkedListReset(LinkedList* list)
{
    list->next      =list->firstElement;
    list->isReverse =false;
}

/**************************************************************************************************\
* 
* Resets the linked list pointer to the last linked list element for reverse iterating
* 
\**************************************************************************************************/
void linkedListResetReverse(LinkedList* list)
{
    list->next      =list->lastElement;
    list->isReverse =true;
}

/**************************************************************************************************\
* 
* Get next element from the list
* 
\**************************************************************************************************/
LinkedListElement* linkedListNext(LinkedList* list)
{
    LinkedListElement* nextElement=list->next;
    if (!list->isReverse)
    {
        if (nextElement!=NULL)
        {
            list->next  =nextElement->next;
        }
    }
    else
    {
        if (nextElement!=NULL)
        {
            list->next  =nextElement->previous;
        }
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
    void* object=NULL;
    LinkedListElement* nextElement=linkedListNext(list);
    if (nextElement!=NULL)
    {
        object=nextElement->object;
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

/**************************************************************************************************\
* 
* Remove last element from LinkedList and destroy it
* 
\**************************************************************************************************/
void linkedListDestroyLastElement(LinkedList* list, bool destroyObject)
{
    LinkedListElement* last         =list->lastElement;
    if (last!=NULL)
    {
        LinkedListElement* beforeLast   =last->previous;
        if (beforeLast!=NULL)
        {
            list->lastElement   =beforeLast;
            beforeLast->next    =NULL;
            list->length--;
        }
        else
        {
            list->firstElement  =NULL;
            list->lastElement   =NULL;
            list->length--;
        }
        if (destroyObject)
        {
            free(last->object);
        }
        free(last);
    }
    else
    {
        logWarning("Removing last element from list: list is empty, nothing removed");
    }
}