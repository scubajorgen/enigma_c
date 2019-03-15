#include <stdio.h>

#include "enigma.h"
#include "crack.h"

char text04[]=
"KGBJN TWBQY FFJWQ KKCTN ZJVRK"  
"BWPQO FZQTB LCYCM WCWTR XSGKA"
"WIZEZ KFIWC KPEYB OBUBW VUHBO"
"BKEGF WGGSQ WUMIO BKHSF TXAGY"
"XPKAX AOJQJ ANZKZ REKYX TXWWR"
"HJHST EJAJS QFZMZ FLTSE QXBAZ"
"WDSJR WHVGF KIXLM PUYIN NQSAW"
"QHXAJ QJCGU CQUFI HWAFN AAFPR"
"ZSMTR KYLUG AOZKY NMXFC HQQEV"
"MTTIN CHTSW CYCRZ FBKMB VSHEK"
"XDYCY PWSZJ WVZAK IRMSQ DZKTF"
"DDEUX WKXMN PDMKD RKASA ORATL"
"JAEHW INMVR SWASF";

void message04()
{
    Enigma* enigma;
    int     g1;
    int     r1;
    int     count;
    
    enigma=createEnigmaM3(); 
    setText(enigma, text04);
    placeWaltze(enigma, 1, "II");
    placeWaltze(enigma, 2, "IV");
    placeWaltze(enigma, 3, "I");
    placeSteckers(enigma, "bd cv el gn iz jo kw mt pr sx");
    placeUmkehrWaltze(enigma, "UKW C");
    
    setRingStellung(enigma, 2, 3);    
    setRingStellung(enigma, 3, 21);    

    setGrundStellung(enigma, 2, 'K');    
    setGrundStellung(enigma, 3, 'E');    
    
    g1=1;
    while (g1<=MAX_POSITIONS)
    {
        r1=1;
        while (r1<=MAX_POSITIONS)
        {
            setRingStellung (enigma, 1, r1);    
            setGrundStellung(enigma, 1, g1);

            encodeDecode(enigma);
            
            count=countLetter(enigma, 'E');
            
            if (count>43)
            {
                printf("Found %d: %s\n", count, toString(enigma));
            }
            
            r1++;
        }
        g1++;
    }
    
    
    
}
