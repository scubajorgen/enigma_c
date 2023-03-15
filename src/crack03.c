#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "adfgx.h"
#include "toolbox.h"

char    topTenKeys[10][26];
double  topTenScore[10];


char sixth[]=
"AGFFGDGXXGXDFGADXAFFADAAFDFGGFFDDDGGAXAF"
"DAFFDFFGDFDDDDGFDDAAAFDGGGGXXXGGDDXXGAAD"
"GDADDFDADFDXFFXXDAFGXDADGGDXXAXFFDXAFDXD"
"DGXAAFXFAXDDDDDXFDXGDXGAXXDFFXDXAAXXGXXX"
"GGAFGFAADFGGFFDAAGDFADAFFGGADXFAFGXDFFFG"
"ADFGDXAADGGAFGAXXDGXXGAGAGGDGXXGXGGFDGDF"
"AFAFFDGAXDFGGDAADGAAGAADFAFGGGFDDAADGGFF"
"AAFGDGXXGXAGXXGAXGDDFFGXGXXFXAAFGXFADDXA"
"ADXGXFFXDDGAFDFDAXADFDDAGFFDDAGDAAADADAA"
"DDXDDGDDFGXGFGDGGDXGFAAXADDAXXGAGGDADFXF"
"DAGFFDDDXXDGFXXDGADFDXGDGAFDAGGAAFDDGFGD"
"XDGDAGGFGFGXXGGGXFDAAFGDDXDFGXAGGDAFDGAF"
"GGXFGDADAADAGGFDFAGGAAXFDGGGGXDXXXXXGGGG"
"ADFFGXAGXGGDXGADXADGAXXGXDAGGGFXFXFXDAGD"
"XAAXXXFGFGGDGXXGFADXAXGGXAXDFDAX";

char sixthTest[]=
"DXGGGFGAGFFGXAGAXGAGDFGG";

char sixthTestKeyword[]="KEYWORD";


char sixthTest2[]=
"AGDGGGDGGFAXAXFFGGAXGFGG";

char sixthTestKeyword2[]="A";

char sixthTest3[]=
"GADDGGXDDDFGFDXAGGXDAAGAXFGXDX";

char sixthTestKeyword3[]="PILOTEN";

char sixthTest4[]=
"AGGAGFFDADFAGAAAAGAAAAGDAAGAGGXFAAAAGFXFADFAAGAADAAADFXFAFAFADAAFAAAFA"
"GDAFAGADGGFXGDAXADFGDAGXXFGXXXDGXAFXGFAGGGFGDFAFGDDGXFXXFGXDFAAFXFXGFG"
"FGFXXAFXFGXGGGXXGGXFAXGAFXXXAFFGXGGDDDXXFAXFXGGDXXGXGDAAAFGGAGDGGDGXXX"
"AGGGFAGFGXFDDAXFXXFAGDDGDDAFAADFFAAAFFGGDGDXDAGDFFAFAGFGAAFADDAADXDAFA"
"FGFFGAGFAADAXDFFDAAXDAAFDDFDDXGG";

char sixthTest5[]=
"AFGDDDFGDAFGFGGDDAGFADAAFAAGGAGGFGGDDDXFDXAXFFXXFAAAAFGDXAXGXFGGXFGDGG"
"DAGAXAGDAFDGGFDGAGFAAGAFAFDGDGGAAADAGFDFDGFAAAAAFFAAAGFAAADAAAGAAADGGX"
"GGXXFFGGXFGGXXFXAXAFFGADFAGGFGGGAGFXGFXGGADFGFGADGGXDGXDDDXXAGFAXDXXXF"
"FFAGFDGXGDAGXGFAGGXGAAFAXDGXDGFDGFAAFGFAGGAAAAGXAGDXAFDAAXXAFAFAFAGAAG"
"AAGGAGAFGADGFAFAFDFGAXFFGDFFAAAFGXFGXGXDGXXXXGXXGXAGFFGXADXGXXXGAFGXGX"
"FGFXGAGADFAXDFAFFXFDDFAGAGXDFXDGGXGXXXDDGGGGGFGGADGXDFAGAAFFGXGXGXDXGA"
"FFAAFAADXDADXFAFFAAGFFDGFGDFDDGAFAFAAGGAFFGDFAGAGDGAADFGAXAFGFAGAD";

char sixthTestKeyword5[]=
"VLIEGTUSORAMPJ";

char sixthTest6[]=
"GXGFXGXFGFXDFDGGDGFGGDGGFAGAXGXAADXFGGDGFGDDXGAAAGGGGXXGGGFXAGGADAGFADDXD"
"AGAAGAAFFDGXFAFGDDADAFAGXFAAFAFADFAFDAAGGDFDDFGFGGADADFAGAAFFGDFGAFFGAFDG"
"GADAGFXAAAAGAFAAADGGGGAAAFFGFFAAGFGADAAGGFDGDFGXFXGGGXDDGAFXFXXAGXADXFDGG"
"AGGAFXFXAAADXAXXFGAFXXDFGXGXXFGFXXXDFXXGFFGXAGGXGGAFXXGFXGGGXFXXFGXFXDFAA"
"GDFXDFDFGGAXXXGFGADGXGGGGFXAXGXFXADXXGFGGDGXFXGGAFAAADAGXDFFDGXFAFAAXGGGF"
"XGDDGDDXDFXAAFDAAFGAGAFDAAAGDDAAGFGDDGAFDDGFADADDAAFXFXXAGGDGFFAADFAAGAGA"
"FGGGAFADGFGGDGAFAAAAFGDAGDFXGGAGFFFAFAAAAFAXAADGFGAAGAGAADDG";

char sixthTestKeyword6[]=
"VLIEGTUS";

char sixthTest7[]=
"AFFXFFGGAGXXGGGAFGFDFFAGAXFGAGDGDXGXFGFGAFDDAXFDDAAXGGFGDGGGDXAAGGDAFDFFD"
"GGGADAGXXDAAGGDGDGGFAFGDGGGGFGXGAGAFADAFFXDDADGGAFAGGGAFDXAXAAGGAFAXFGGFA"
"GDFFXGFGFAXAAFXFGAAFFAAAAAGGXGGGDXGXFDADAGAGGXDAGXFAGGDXDAAXAADAGDFFGAXAA"
"DAFXFXGDFFAFADAXAGGGFXXAGFFGFGGAXFFDGDAXDGXGFGFFGDXAXGFDGAXDXAGAGFXFDGFGG"
"FDADAXDGGAAXGGFGGFGAAFAFXFFXFGGXXAGAGFDFFDDGXFGGFAFAGXXFGDDFAAGFXDGADFGAG"
"GFADXXDAFAFGDXAFDGXXAXDDGXGFGAXXFDAXAAAAAGFXAXGFFAFAXDXAXDGDFDFFFGDAAGGAF"
"DAFGGFXGAAGAAAAAADGAXGXFAGGGGAGXADFGFGAGDXDDAAAADDADAAGGFXAX";

char sixthTestKeyword7[]=
"KCDBENGOIJALMQFPH";

char sixthTest8[]=
"AGGAGDGAGGXFXFGFDDAFAGDGGGFXGXGX";

char sixthTestKeyword8[]=
"ADCB";

char seventhTest[]="UWQJHNQIUSSWWHMGMSPARTLIXQDJRAHELIMYTFFLFNTFIJROFBDZEGAPLSTGBXOQTMDMVBTWWZAOODOJT";

//int permElements[]     ={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
int permElements[]     ={'V', 'L', 'I', 'E', 'G', 'T', 'U', 'S', 'I', 'J'};

void theSixthProblem()
{
    char*   cipher=sixthTest6;

    int     keySize;
    int     i, j;
    int     iMax, jMax;
    int     depth;
    char    swap;
    float   ioc;
    float   iocMax;
    float   iocMaxMax;
    int     done;
    char*   plain;
    char    key[27];
    char    keyCopy[27];
//    char    maxKey[27];
//    char    maxMaxKey[27];
    
    adfgxAlloc(26, strlen(cipher));
    adfgxNormalizeCipher(cipher, strlen(cipher));
    
    iocMaxMax=0;
    keySize  =8;
    while (keySize<=8)
    {
        printf("Keysize %d\n", keySize);
        i=0;
        while (i<keySize)
        {
            key[i]='A'+i;
            i++;
        }
//strcpy(key, "GEOMARIK");        
//strcpy(key, "VLIEGTUS");        
        key[keySize]='\0';
        strncpy(keyCopy, key, 26);
       
        iMax=-1;
        jMax=-1;        
        // Start with unswapped key
        plain=adfgxDecrypt(keyCopy, strlen(keyCopy));
        iocMax=adfgxIndexOfCoincidence();       
        printf("Key %s iocMax %f\n", keyCopy, iocMax);

        depth=0;
        done=0;
        while (depth<keySize/2 && !done)
        {
            done=1;
            i=0;
            while (i<keySize-1)
            {
                j=i+1;
                while (j<keySize)
                {
                    
                    swap=keyCopy[i];
                    keyCopy[i]=keyCopy[j];
                    keyCopy[j]=swap;
                    
                    
                    plain=adfgxDecrypt(keyCopy, strlen(keyCopy));
                    
                    ioc=adfgxIndexOfCoincidence();
                    printf("%d %d %s %f\n", i, j, keyCopy, ioc);
                    if (ioc>iocMax)
                    {
                        iocMax=ioc;
                        iMax=i;
                        jMax=j;
                        done=0;
                        printf(">>>%s\n", keyCopy);
                    }
                    swap=keyCopy[i];
                    keyCopy[i]=keyCopy[j];
                    keyCopy[j]=swap;                
                    j++;
                }
                i++;
            }      
            if (!done)
            {
                swap=keyCopy[iMax];
                keyCopy[iMax]=keyCopy[jMax];
                keyCopy[jMax]=swap;    
            }                
            depth++;
        }

        if (iocMax>=iocMaxMax)
        {
            iocMaxMax=iocMax;
            plain=adfgxDecrypt(keyCopy, strlen(keyCopy));
            printf("####Absolute Max %f key %s decrypt: %s\n", iocMaxMax, keyCopy, plain);
            
        }
        keySize++;
    }
   
    adfgxFree();
}

void theSixthProblem2()
{
    char*   cipher=sixthTest6;

    int     keySize;
    int     i, j;
    float   ioc;
    float   iocMax;
    float   iocMaxMax;
    char*   plain;
    char    key[27];
//    char    keyCopy[27];
    int*            permutation;
    LinkedList*     permutations;
    
    adfgxAlloc(26, strlen(cipher));
    adfgxNormalizeCipher(cipher, strlen(cipher));
    
    
    iocMaxMax=0.0;
    iocMax=0.0;
    keySize  =2;
    while (keySize<=8)
    {
        printf("Keysize %d\n", keySize);

        
        permutations=createLinkedList();
        permute(permutations, permElements, keySize, keySize, 0);

        i=0;
        while (i<linkedListLength(permutations))
        {
            permutation=(int*)elementAt(permutations, i);

            j=0;
            while (j<keySize)
            {
                key[j]=(char)permutation[j];
                j++;
            }
            key[j]='\0';

            plain=adfgxDecrypt(key, strlen(key));
            
            ioc=adfgxIndexOfCoincidence();
//            printf("%s %f\n", key, ioc);
            if (ioc>=iocMax/* || ioc>2.0*/)
            {
                iocMax=ioc;
                printf(">>>%s %f %s\n", key, ioc, plain);
//                strncpy(keyCopy, key, 26);
            }
            
            i++;
        }

        if (iocMax>=iocMaxMax)
        {
            iocMaxMax=iocMax;
            plain=adfgxDecrypt(key, strlen(key));
            printf("####Absolute Max %f key %s decrypt: %s\n", iocMaxMax, key, plain);
        }
        
        
        destroyLinkedList(permutations);
        keySize++;
    }
   
    adfgxFree();
}


void theSixthTest()
{
    char*   cipher=sixthTest6;
    char*   key   =sixthTestKeyword6;
    char*   plain;    
    
    printf("Key   : size %d %s\n", (int)strlen(key), key);
    printf("Cipher: size %d %s\n", (int)strlen(cipher), cipher);
   
    adfgxAlloc(strlen(key), strlen(cipher));
    adfgxNormalizeCipher(cipher, strlen(cipher));
    plain=adfgxDecrypt(key, strlen(key)); 
    printf("Decrypt: %s\n\n", plain);
    
}

void theSeventhProblem()
{
    
}