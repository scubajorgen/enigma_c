#include <stdio.h>
#include <malloc.h>
#include <string.h>

char* adfgxPlain;
int * adfgxNormPlain;
int * adfgxTransposed;
int * adfgxNormCipher;
int * adfgxKey;
int * adfgxKeyOrder;
int * adfgxKeyIndex;
int * adfgxRowsPerCol;
int * adfgxCumRowsPerCol;
char adfgxMatrix[5][5]=
{
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'},
};
int adfgxNormCipherLength;
int adfgxMaxCipherLength;

void quicksort(int* source, int* target, int first, int last)
{
   int  i, j;
   int  pivot, temp; 

   if(first<last)
   {
      pivot=first;
      i=first;
      j=last;

      while(i<j)
      {
         while(source[i]<=source[pivot]&&i<last)
            i++;
         while(source[j]>source[pivot])
            j--;
         if(i<j)
         {
            temp=source[i];
            source[i]=source[j];
            source[j]=temp;
            temp=target[i];
            target[i]=target[j];
            target[j]=temp;
        }
      }

      temp=source[pivot];
      source[pivot]=source[j];
      source[j]=temp;
      temp=target[pivot];
      target[pivot]=target[j];
      target[j]=temp;
      quicksort(source, target, first,j-1 );
      quicksort(source, target, j+1  ,last);

   }
}

void adfgxAlloc(int keySize, int cipherSize)
{
    int rows;
    
    
    rows=cipherSize/keySize;
    if (cipherSize/keySize!=0)
    {
        rows++;
    }
    adfgxMaxCipherLength=rows*keySize;
    
    adfgxPlain              =malloc(adfgxMaxCipherLength)+1;
    adfgxPlain[cipherSize/2]=0;
    adfgxNormPlain          =malloc(adfgxMaxCipherLength/2*sizeof(int));
    adfgxTransposed         =malloc(adfgxMaxCipherLength*sizeof(int));
    adfgxNormCipher         =malloc(adfgxMaxCipherLength*sizeof(int));
    adfgxKey                =malloc(keySize*sizeof(int));
    adfgxKeyIndex           =malloc(keySize*sizeof(int));
    adfgxKeyOrder           =malloc(keySize*sizeof(int));
    adfgxRowsPerCol         =malloc(keySize*sizeof(int));
    adfgxCumRowsPerCol      =malloc(keySize*sizeof(int));
}

void adfgxFree()
{
/*    
    free(adfgxPlain);
    free(adfgxNormPlain);
    free(adfgxTransposed);
    free(adfgxNormCipher);
    free(adfgxKeyIndex);
    free(adfgxKeyOrder);
    free(adfgxRowsPerCol);
    free(adfgxCumRowsPerCol);
*/  
}

char* adfgxDecrypt(char* key, int keyLength)
{
    int     rows, row, col;
    int     i;
    // The column order e.g. T,A,G -> 2,0,1
    i=0;
    while (i<keyLength)
    {
        adfgxKey     [i]=key[i];
        adfgxKeyIndex[i]=i;
        i++;
    }
    quicksort(adfgxKey, adfgxKeyIndex, 0, keyLength-1);

    i=0;
    while (i<keyLength)
    {
        adfgxKeyOrder[adfgxKeyIndex[i]]=i;
        i++;
    }
    
    // The number of rows per column
    i=0;
    while (i<keyLength)
    {
        adfgxRowsPerCol[i]=adfgxNormCipherLength/keyLength;
        if (adfgxKeyIndex[i]<adfgxNormCipherLength%keyLength)
        {
            adfgxRowsPerCol[i]++;
        }

        if (i==0)
        {
            adfgxCumRowsPerCol[i]=0;
        }
        else
        {
            adfgxCumRowsPerCol[i]=adfgxCumRowsPerCol[i-1]+adfgxRowsPerCol[i-1];
        }
        i++;
    }
    
    rows=adfgxNormCipherLength/keyLength;
    if (adfgxNormCipherLength%keyLength!=0)
    {
        rows++;
    }
    
    row=0;
    while (row<rows)
    {
        col=0;
        while (col<keyLength)
        {
            adfgxTransposed[row*keyLength+col]=adfgxNormCipher[adfgxCumRowsPerCol[adfgxKeyOrder[col]]+row];
            col++;
        }
        row++;
    }
    
    i=0;
    while (i<adfgxNormCipherLength)
    {
        row=adfgxTransposed[i];
        col=adfgxTransposed[i+1];
        adfgxNormPlain[i/2]=row*5+col;
        adfgxPlain[i/2]=adfgxMatrix[row][col];
        i+=2;
    }
    adfgxPlain[i/2]='\0';
    return adfgxPlain;
}

void adfgxNormalizeCipher(char* cipher, int cipherLength)
{
    int i;
    i=0;
    while (i<cipherLength)
    {
        switch(cipher[i])
        {
            case 'A':
                adfgxNormCipher[i]=0;
                break;
            case 'D':
                adfgxNormCipher[i]=1;
                break;
            case 'F':
                adfgxNormCipher[i]=2;
                break;
            case 'G':
                adfgxNormCipher[i]=3;
                break;
            case 'X':
                adfgxNormCipher[i]=4;
                break;
        }
        i++;
    }
    adfgxNormCipherLength=cipherLength;
}

void adfgxDenormalizeCipher(int* cipher)
{
    int i;
    i=0;
    while (i<adfgxNormCipherLength)
    {
        switch(cipher[i])
        {
            case 0:
                adfgxPlain[i]='A';
                break;
            case 1:
                adfgxPlain[i]='D';
                break;
            case 2:
                adfgxPlain[i]='F';
                break;
            case 3:
                adfgxPlain[i]='G';
                break;
            case 4:
                adfgxPlain[i]='X';
                break;
        }
        i++;
    }
}


/**************************************************************************************************\
* 
* Calculates the index of coincidence 
* TODO: transform to int calculation for speed
* 
\**************************************************************************************************/
float adfgxIndexOfCoincidence()
{
    int     c;
    int     i;
    float   ioc;
    int     iocCharCount[25];
    int     plainTextSize;
    
    plainTextSize=adfgxNormCipherLength/2;
    
    c=0;
    while (c<25)
    {
        iocCharCount[c]=0;
        c++;
    }

    i=0;
    while (i<plainTextSize)
    {
        iocCharCount[adfgxNormPlain[i]]++;
        i++;
    }
    
    ioc=0.0;
    c=0;
    while (c<25)
    {
        ioc+=iocCharCount[c]*(iocCharCount[c]-1);
        c++;
    }
    ioc/=plainTextSize*(plainTextSize-1);
    ioc*=25.0;
    
    return ioc;
}

void adfgxDump()
{
    int i;
    i=0;
    while (i<adfgxNormCipherLength)
    {
        printf("%d %d - ", adfgxNormCipher[i], adfgxNormCipher[i+1]);
        i+=2;
    }
    printf("\n");
}

void adfgxDumpPlain()
{
    int i;
    i=0;
    while (i<adfgxNormCipherLength/2)
    {
        printf("%d ", adfgxNormPlain[i]);
        i++;
    }
    printf("\n");
}