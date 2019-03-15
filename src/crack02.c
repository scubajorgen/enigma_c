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


/*
    Plain text: 
    Nadat eerst de Poolse en nadien, tijdens de Tweede Wereldoorlog, de Britse inlichtingendienst 
    erin slaagde de Enigmacodes te breken, bleek het toestel een goudmijn van informatie over de 
    Duitse oorlogsmachine. Deze informatie, verkregen door ontcijfering van de geheime Duitse 
    berichten, kreeg de codenaam "Ultra" en speelde een uiterst belangrijke rol in het verloop 
    van de Tweede Wereldoorlog, vooral in de U-bootoorlog in de Atlantische Oceaan, de veldslagen 
    in Afrika en de landing in Normandië.
*/
// Text as encoded
char text04_test1[]=
"zujzl anzfu lwrsg fppug rnpcx qvqcm rsnvg hdbrl vblyz uzvpw bnjki "
"kwkhm rhxgt esqnq yhmzb qoclv szjtd fqavw jkfuc temvr laeqz rdjji " 
"acmdf axaqd kitei pfbpy gwlzl kbwcd vilcm wixvn cflkc cnjse nxjhs "
"nthzs oegnh nbtnm hmydy uvlqx xdioc awrcg nzzcz wjpxa sestv ihzoc "
"ysiqv gcuqe nmitv abtqm qmxan byjzq uxxzt ntmwl qciun fkkuw skiqi "
"rxmdw foyqm pscxl cskds dewyj pumrm ichsz oydtm uhqnw zliwd fkunm "
"waqay vfvrn xskcv puaqu rnikq lmgjv nzhmd avkdc smuzt fjhic omjbk "
"plukp thsvf fesdu dwurk xbdwn ptd";

// Text as ecoded, 1st 15 characters removed
char text04_test2[]=
"fppug rnpcx qvqcm rsnvg hdbrl vblyz uzvpw bnjki "
"kwkhm rhxgt esqnq yhmzb qoclv szjtd fqavw jkfuc temvr laeqz rdjji " 
"acmdf axaqd kitei pfbpy gwlzl kbwcd vilcm wixvn cflkc cnjse nxjhs "
"nthzs oegnh nbtnm hmydy uvlqx xdioc awrcg nzzcz wjpxa sestv ihzoc "
"ysiqv gcuqe nmitv abtqm qmxan byjzq uxxzt ntmwl qciun fkkuw skiqi "
"rxmdw foyqm pscxl cskds dewyj pumrm ichsz oydtm uhqnw zliwd fkunm "
"waqay vfvrn xskcv puaqu rnikq lmgjv nzhmd avkdc smuzt fjhic omjbk "
"plukp thsvf fesdu dwurk xbdwn ptd";



char textBuf[1000];

void message04()
{
    Enigma* enigma;
    int     g1;
    int     r1;
    int     count;
	int     limit;
	int     dummyChars;

    printf("Message 04\n");
    enigma=createEnigmaM3(); 



    placeWaltze(enigma, 1, "II");
    placeWaltze(enigma, 2, "IV");
    placeWaltze(enigma, 3, "I");
    placeSteckers(enigma, "bd cv el gn iz jo kw mt pr sx");
    placeUmkehrWaltze(enigma, "UKW C");
    
    setRingStellung(enigma, 2, 3);    
    setRingStellung(enigma, 3, 21);    

    setGrundStellung(enigma, 2, 'K');    
    setGrundStellung(enigma, 3, 'E');    

    
    dummyChars=0;
    while (dummyChars!=30)
    {
        // add some dummy chars
		sprintf(textBuf+dummyChars, "%s", text04);
        setText(enigma, textBuf);
        // Frequence e=18%, n=10%. Therefore we put the limit on 14%
        limit=enigma->textSize*10/100;		
		
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
				
				if (count>limit)
				{
					printf("Found %d %d: %s\n", dummyChars, count, toString(enigma));
				}
				
				r1++;
			}
			g1++;
		}
        dummyChars++;
	}
    
    
}
