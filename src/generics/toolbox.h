#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "linkedList.h"

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

void        permute                 (LinkedList* permutations, int elements[], int elementArraySize, int number, int start);
LinkedList* createPermutations      (int* arrayToChooseFrom, int numberOfElements, int numberToChoose);
LinkedList* combinePermutations     (LinkedList* list1, int permutation1Size, LinkedList* list2, int permutation2Size);
void        destroyPermutations     (LinkedList* permutations);

LinkedList* createWalzePermutations (int numberOfWalzen, int numberToChooseFrom);
void        printCipher             (char* cipher);

void        selectRandomIndices     (int* available, int arrayLength, int number, int* resultingIndices);
int         randint                 (int n);
#endif