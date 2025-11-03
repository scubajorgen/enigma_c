/**************************************************************************************************\
* 
* Toolbox functions
*
\**************************************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "toolbox.h"

// Permutations of rotors/walzen
int                 walzenIndices[8]    ={0, 1, 2, 3, 4, 5, 6, 7};
char                walzen[8][5]        ={"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};

/**************************************************************************************************\
* 
* Calculates the power with integer numbers result=base ^ exp
* 
\**************************************************************************************************/
int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

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
* Converts the unified position (0-25) to a character ('A'-'Z')
* 
\**************************************************************************************************/
char posToChar(int position)
{
    char theChar;
    
    theChar='A'+(char)position; 
    
    return theChar;
}

/**************************************************************************************************\
* 
* Converts a unified position (0-25) to GrundStellung or RingStellung (1-26)
* 
\**************************************************************************************************/
int posToStellung(int position)
{
    int stellung;
    
    stellung=position+1;    
    
    return stellung;
}

/**************************************************************************************************\
* 
* Converts an text character (A-Z, a-z) to a Ring/Grundstellung (1-26)
* 
\**************************************************************************************************/
int charToStellung(int theChar)
{
    int stellung;
    if (theChar>='A' && theChar<='Z')
    {
        stellung=theChar-'A'+1;
    }
    else if (theChar>='a' && theChar<='z')
    {
        stellung=theChar-'a'+1;
    }
    else
    {
        stellung=-1;
    }     
    return stellung;
}

/**************************************************************************************************\
* 
* Converts a  Ring/Grundstellung (1-26) to a character (A-Z)
* 
\**************************************************************************************************/
int stellungToChar(int stellung)
{
    return stellung+'A'-1;
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
* Generic function to retrieve all permutations when selecting a number elements out of an array
* For example choosing 3 elements from an array of 5, gets you 5*4*3=60 permutations.
* It is implemented using a recursive permute() function
* Permutations are returned as a linked list
* arrayToChooseFrom: The array of elements to choose from 
* numberOfElements : The number of elements in the array
* numberToChoose   : The number of elements to choose from
* 
\**************************************************************************************************/
LinkedList* createPermutations(int* arrayToChooseFrom, int numberOfElements, int numberToChoose)
{
    LinkedList* permutations;
    permutations=createLinkedList();
    permute(permutations, arrayToChooseFrom, numberOfElements, numberToChoose, 0);
    return permutations;
}

/**************************************************************************************************\
* 
* This function combines to lists of permutations and creates a new list of combined permutations.
* If the first list contains N elements and the second list M, the resulting list contains N*M
* elements.
* list1: first list
* list2: 
* 
\**************************************************************************************************/
LinkedList* combinePermutations(LinkedList* list1, int permutation1Size, LinkedList* list2, int permutation2Size)
{
    LinkedList* permutations=createLinkedList();
    resetLinkedList(list1);
    while (hasNext(list1))
    {
        int* object1=(int*)nextLinkedListObject(list1);
        resetLinkedList(list2);
        while (hasNext(list2))
        {
            int* object2=(int*)nextLinkedListObject(list2);
            int* permutation=malloc((permutation1Size+permutation2Size)*sizeof(int));
            for (int i=0;i<permutation1Size; i++)
            {
                permutation[i]=object1[i];
            }
            for (int i=0;i<permutation2Size; i++)
            {
                permutation[permutation1Size+i]=object2[i];
            }
            addObject(permutations, permutation);
        }
    }
    return permutations;
}


/**************************************************************************************************\
* 
* Create the permutations of the walzen. numberOfWalzen: 3 or 4, numberToChoosFrom: 3,5,8 
* TO DO: incorporate beta and gamma walzen at the right spot
* 
\**************************************************************************************************/
LinkedList* createRotorPermutations(int numberOfWalzen, int numberToChooseFrom)
{
    LinkedList* permutations;
    permutations=createLinkedList();
    permute(permutations, walzenIndices, numberToChooseFrom, numberOfWalzen, 0);
    return permutations;
}


/**************************************************************************************************\
* 
*  This function properly destroys the permutations linked list
*  It destroys the objects. Then it destroys the linked list.
*  If objects should not be destroyed, use destroyLinkedList()
* 
\**************************************************************************************************/
void destroyPermutations(LinkedList* permutations)
{
    // First destroy the objects in the linked list
    resetLinkedList(permutations);
    while (hasNext(permutations))
    {
        int* permutation=(int*)nextLinkedListObject(permutations);
        free(permutation);
    }
    // Then destroy the list itself
    destroyLinkedList(permutations);
}


/**************************************************************************************************\
* 
* Print cipher nicely formated
* 
\**************************************************************************************************/
void printCipher(char* cipher)
{
  int i;
  int printed;
  
  printf("# ");
  i=0;
  printed=0;
  while(i<strlen(cipher))
  {
      if (cipher[i]!=' ')
      {
          printf("%c", cipher[i]);
          printed++;

      if (printed%5==0)
      {
          if (printed==50)
          {
              printf("\n# ");
              printed=0;
          }
          else
          {
            printf(" ");
          }
      }
      }
      i++;
  }
  if ((printed!=0) || (strlen(cipher)==0))
  {
    printf("\n");
  }
}

/**************************************************************************************************\
* 
* Create a random selection of indices randomly based on availability. Not very efficient, only 
* to be used for small arrays.
* available       : array of 0, 1 indicating availability, e.g. {0, 1, 1, 0, 1, 1, 1}
* arrayLength     : length of available array, e.g. 7 in this example
* number          : number of indices to select,  e.g. 3
* resultingIndices: array of resulting indices, at least of size equal to number, e.g. {2, 6, 4}
* 
\**************************************************************************************************/
void selectRandomIndices(int* available, int arrayLength, int number, int* resultingIndices)
{
    int* indices=malloc(arrayLength*sizeof(int));
    int  i;

    // Make a list of available indices
    int indexCount=0;
    for (i=0;i<arrayLength;i++)
    {
        if (available[i]>0)
        {
            indices[indexCount]=i;
            indexCount++;
        }
    }

    // Feasibility check
    if (number>indexCount)
    {
        printf("Invalid selectOf, trying to select more elements than are available\n");
        exit(0);
    }

    // Now randomly select the indicated number
    for(i=0; i<number;i++)
    {
        bool exit;
        exit=false;
        while (!exit)
        {
            int tryIndex=rand() % indexCount;
            if (indices[tryIndex]>=0)
            {
                resultingIndices[i]=indices[tryIndex];
                indices[tryIndex]=-1;
                exit=true;
            }
        }
    }
    free(indices);
}