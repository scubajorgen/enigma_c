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
LinkedList* createLinkedList()
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
* nextLinkedListObject().
* 
\**************************************************************************************************/
void* elementAt(LinkedList* list, int index)
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
* Appends an object to the linked list. An element is created for the object and it is appended
* 
\**************************************************************************************************/
void addObject(LinkedList* list, void* newObject)
{
    LinkedListElement* newElement;
    LinkedListElement* listEnd;
    
    newElement=malloc(sizeof(LinkedListElement));
    
    newElement->object=newObject;
    newElement->next        =NULL;
    
    // Check if list is empty
    if (list->firstElement==NULL)
    {
        // empty: add first element
        newElement->previous    =NULL;
        list->firstElement      =newElement;
        list->lastElement       =newElement;
        list->next              =newElement;
    }
    else
    {
        listEnd                 =list->lastElement;
        newElement->previous    =listEnd;
        listEnd->next           =newElement;
        list->lastElement       =newElement;
    }
    list->length++;
}


/**************************************************************************************************\
* 
* Destroy the linked list objects and the linked list itself. It does not destroy the objects 
* themselves
* 
\**************************************************************************************************/
void destroyLinkedList(LinkedList* list)
{
    LinkedListElement* element;
    LinkedListElement* nextElement;
    
    element=list->lastElement;
    while (element!=NULL)
    {
        nextElement=element->previous;
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
void resetLinkedList(LinkedList* list)
{
    list->next=list->firstElement;
}

/**************************************************************************************************\
* 
* Retrieve the object from next linked list element and increases the linked list pointer.
* 
\**************************************************************************************************/
void* nextLinkedListObject(LinkedList* list)
{
    LinkedListElement*  nextElement;
    void*               object;
    
    nextElement =list->next;
    object      =NULL;
    
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
int hasNext(LinkedList* list)
{
    int hasNext;
    
    if (list->next==NULL)
    {
        hasNext=0;
    }
    else
    {
        hasNext=1;
    }
    return hasNext;
}
