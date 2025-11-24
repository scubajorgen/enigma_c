#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

typedef struct LinkedListElement
{
    struct LinkedListElement* next;
    struct LinkedListElement* previous;
    void* object;
}
LinkedListElement;    

typedef struct LinkedList
{
    int                 length;
    LinkedListElement*  next;
    LinkedListElement*  firstElement;
    LinkedListElement*  lastElement;
}
LinkedList;

LinkedList*         linkedListCreate        ();
void                linkedListDestroy       (LinkedList* list, bool destroyObjects);
void                linkedListAppendObject  (LinkedList* list, void* newObject);
void                linkedListReset         (LinkedList* list);
void*               linkedListNextObject    (LinkedList* list);
LinkedListElement*  linkedListNext          (LinkedList* list);
void                linkedListAppend        (LinkedList* list, LinkedListElement* element);
void                linkedListInsertAfter   (LinkedList* list, LinkedListElement* element, LinkedListElement* after);
void                linkedListInsertBefore  (LinkedList* list, LinkedListElement* element, LinkedListElement* before);
int                 linkedListHasNext       (LinkedList* list);
int                 linkedListLength        (LinkedList* list);
void*               linkedListObjectAt      (LinkedList* list, int index);

#endif
