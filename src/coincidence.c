#include <stdio.h>

#include "enigma.h"
#include "toolbox.h"


int                 ioc_charCount[MAX_POSITIONS];

// Permutations of rotors/waltzen
int                 ioc_waltzenIndices[8]  ={0, 1, 2, 3, 4, 5, 6, 7};
char                ioc_waltzen[8][4]      ={"I", "II", "III", "IV", "V", "VI", "VII", "VIII"};
LinkedList*         ioc_permutations;


/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
float ioc_indexOfCoincidence(Enigma* enigma)
{
    int     c;
    float   ioc;
    
    c=0;
    while (c<MAX_POSITIONS)
    {
        ioc_charCount[c]=countConvertedChar(enigma, c);
        c++;
    }
    
    ioc=0.0;
    c=0;
    while (c<MAX_POSITIONS)
    {
        ioc+=ioc_charCount[c]*(ioc_charCount[c]-1);
        c++;
    }
    ioc/=enigma->textSize*(enigma->textSize-1);
    
    return ioc;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void ioc_decodeText(char* text)
{
    Enigma*     enigma;
    int         r1, r2, r3;
    int         g1, g2, g3;
    char        waltzen[5][5]={"I", "II", "III", "IV", "V"};
    int         indices[5]={0, 1, 2, 3, 4};
    LinkedList* permutations;
    int*        permutation;
    int         w;
    float       ioc;
    float       iocMax;
    
    
    permutations=createLinkedList();
    permute(permutations, indices, 5, 3, 0);
    
    enigma=createEnigmaM3(); 

    placeUmkehrWaltze(enigma, "UKW B");
    
    clearSteckerBrett(enigma);
//    placeSteckers(enigma, "AT BG DV EW FR HN IQ JX KZ LU");
    

    setText(enigma, text);

    iocMax=0.0;
    w=0;
    while (w<linkedListLength(permutations))
    {
        permutation=(int*)elementAt(permutations, w);

        placeWaltze(enigma, 1, waltzen[permutation[0]]);
        placeWaltze(enigma, 2, waltzen[permutation[1]]);
        placeWaltze(enigma, 3, waltzen[permutation[2]]);

        printf("Trying %s %s %s\n", waltzen[permutation[0]], waltzen[permutation[1]], waltzen[permutation[2]]);

        r1=1;

        r2=1;
        while (r2<=MAX_POSITIONS)
        {
            r3=1;
            while (r3<=MAX_POSITIONS)
            {
                
                g1=1;
                while (g1<=MAX_POSITIONS)
                {
                    g2=1;
                    while (g2<=MAX_POSITIONS)
                    {
                        g3=1;
                        while (g3<=MAX_POSITIONS)
                        {
                    
                
                            setRingStellung(enigma, 1, r1);
                            setRingStellung(enigma, 2, r2);
                            setRingStellung(enigma, 3, r3);

                            setGrundStellung(enigma, 1, g1);
                            setGrundStellung(enigma, 2, g2);
                            setGrundStellung(enigma, 3, g3);

                            encodeDecode(enigma);
                            
                            ioc=ioc_indexOfCoincidence(enigma);
                            if (ioc>iocMax)
                            {
                                iocMax=ioc;
                                printf("%s %s %s R %d %d %d G %d %d %d - %f\n",
                                       waltzen[permutation[0]], waltzen[permutation[1]], waltzen[permutation[2]],
                                       r1, r2, r3,
                                       g1, g2, g3,
                                       iocMax);
                            }
                            g3++;
                        }
                        g2++;
                    }
                    g1++;
                }
             
                r3++;
            }
            r2++;
        }


        w++;
    }
    destroyEnigma(enigma);    

}