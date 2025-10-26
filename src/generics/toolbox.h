#ifndef TOOLBOX_H
#define TOOLBOX_H

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

// Permutations of rotors/walzen
extern int                 walzenIndices[8];
extern char                walzen[8][5];


int         ipow                    (int base, int exp);
int         stellungToPos           (int stellung);
int         charToPos               (char theChar);
char        posToChar               (int position);
int         posToStellung           (int position);
int         charToStellung          (int theChar);
int         stellungToChar          (int stellung);

LinkedList* createLinkedList        ();
void        destroyLinkedList       (LinkedList* list);
void        addObject               (LinkedList* list, void* newObject);
void        resetLinkedList         (LinkedList* list);
void*       nextLinkedListObject    (LinkedList* list);
int         hasNext                 (LinkedList* list);
int         linkedListLength        (LinkedList* list);
void*       elementAt               (LinkedList* list, int index);
void        permute                 (LinkedList* permutations, int elements[], int elementArraySize, int number, int start);
LinkedList* createRotorPermutations (int numberOfWalzen, int numberToChooseFrom);
void        printCipher             (char* cipher);
void        selectRandomIndices     (int* available, int arrayLength, int number, int* resultingIndices);

#endif