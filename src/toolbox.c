#include <malloc.h>

#include "toolbox.h"


// Permutations of rotors/waltzen
int                 waltzenIndices[8]  ={0, 1, 2, 3, 4, 5, 6, 7};
char                waltzen[8][4]      ={"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
LinkedList*         permutations;



/**************************************************************************************************\
* 
* Converts an integer GrundStellung or RingStellung (A-Z, a-z, 1-26) to a unified position (0-25)
* 
\**************************************************************************************************/
int stellungToPos(int stellung)
{
    int pos;
    
    if (stellung>='A' && stellung<='Z')
    {
        pos=stellung-'A';
    }
    else if (stellung>='a' && stellung<='z')
    {
        pos=stellung-'a';
    }
    else if (stellung>=1 && stellung<=26)
    {
        pos=stellung-1;
    }
    else
    {
        pos=0;
    }    
    
    return pos;
}


/**************************************************************************************************\
* 
* Converts an text character (A-Z, a-z) to a unified position (0-25)
* 
\**************************************************************************************************/
int charToPos(char theChar)
{
    int pos;
    
    if (theChar>='A' && theChar<='Z')
    {
        pos=theChar-'A';
    }
    else if (theChar>='a' && theChar<='z')
    {
        pos=theChar-'a';
    }
    else
    {
        pos=-1;
    }    
    
    return pos;
}


/**************************************************************************************************\
* 
* 
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
* 
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
* 
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
* 
* 
\**************************************************************************************************/
void resetLinkedList(LinkedList* list)
{
    list->next=list->firstElement;
}

/**************************************************************************************************\
* 
* 
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
* 
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


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

void permute(LinkedList* permutations, int elements[], int elementArraySize, int number, int start)
{
    int*    permutation;
    int     i;
    int     temp;
    
    if (start == number) 
    {
        permutation=malloc(number*sizeof(int));
        for (i = 0; i < number; i++) 
        {
            permutation[i]=elements[i];
        }
        addObject(permutations, permutation);
    } 
    else 
    {
        for (i = start; i < elementArraySize; i++) 
        {
            temp = elements[start];
            elements[start] = elements[i];
            elements[i] = temp;

            permute(permutations, elements, elementArraySize, number, start + 1);

            temp = elements[start];
            elements[start] = elements[i];
            elements[i] = temp;
        }
    }    
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/

int linkedListLength(LinkedList* list)
{
    return list->length;
}


/**************************************************************************************************\
* 
* 
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
    return object;
}


/**************************************************************************************************\
* 
* Create the permutations of the waltzen. numberOfWaltzen: 3 or 4, numberToChoosFrom: 3,5,8 
* TO DO: incorporate beta and gamma waltzen at the right spot
* 
\**************************************************************************************************/
LinkedList* createRotorPermutations(int numberOfWaltzen, int numberToChooseFrom)
{
    LinkedList* permutations;
    permutations=createLinkedList();
    permute(permutations, waltzenIndices, numberToChooseFrom, numberOfWaltzen, 0);
    return permutations;
}