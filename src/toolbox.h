

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

int         stellungToPos           (int stellung);
int         charToPos               (char theChar);

LinkedList* createLinkedList        ();
void        destroyLinkedList       (LinkedList* list);
void        addObject               (LinkedList* list, void* newObject);
void        resetLinkedList         (LinkedList* list);
void*       nextLinkedListObject    (LinkedList* list);
int         hasNext                 (LinkedList* list);
int         linkedListLength        (LinkedList* list);
void*       elementAt               (LinkedList* list, int index);
void        permute                 (LinkedList* permutations, int elements[], int elementArraySize, int number, int start);