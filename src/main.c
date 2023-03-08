#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigma.h"
#include "exampleEnigma.h"
#include "exampleTuring.h"
#include "exampleCoincidence.h"
#include "crack.h"


int main()
{
    char input[10];
    int  i;
    
    printf("#####################################################################################\n");
    printf("# ENIGMA SIMULATION\n");
    printf("#####################################################################################\n");

	printf(" 0.     Enigma M3 example                       \n");
	printf(" 1.     Enigma M4 \"Shark\" example               \n");

	printf("10.     Turing crib circles/loops               \n");
	printf("11.     Turing example                          \n");

	printf("20.     Gillogly IoC example 1  - original      \n");
	printf("21.     Gillogly IoC example 1A - no solution   \n");
	printf("22.     Gillogly IoC example 1B - solution      \n");
	printf("23.     Gillogly IoC example 1C - ??            \n");
	printf("24.     Gillogly IoC example 2A english         \n");
	printf("25.     Gillogly IoC example 7A                 \n");
	
    printf("30.     The third problem                       \n");
	printf("31.     The fourth problem                      \n");
	printf("32.     The fifth problem                       \n");

	printf("40-49.  Engima Challenge message 1-10   \n");


    fgets(input, 10, stdin);
    i=atoi(input);

    switch(i)
    {
        case 0: 
            // Simple encode/decode example on Engima M3 v
            enigmaExampleM3();           
            break;
        case 1: 
            // Simple encode/decode example on Engima M4 v
            enigmaExampleM4();           
            break;

        case 10:
            // The Turing method: find the crib loops and show they work v
            turingProve();
            break;        
        case 11: 
            // The Turing method: working example -> readable message, not all steckersettings are found
            turingExample();
            break;    

        case 20: 
            // The James Gillogly method example v
            iocExample00();
            break;
        case 21: 
            // 2nd IoC example, 8 steckers, won't work
            iocExample01A();
            break;
        case 22:  
            // METHOD_IOC_R3 method, works
            iocExample01B();
            break;
        case 23:  
            // METHOD_IOC_R2R3 method, ?
            iocExample01C();
            break;
        case 24:  
            // Some long english text
            iocExample02A();
            break;
        case 25:  
            // 
            iocExample07A();
            break;
        case 30:
            theThirdProblem();
            break;
        case 31:
            theFourthProblem();
            break;
        case 32:
            theFifthProblem();
            break;
        case 33:
            theSixthProblem();
            break;
        case 34:
            iocExampleNgram();
            break;    
        case 40:
            message01();
            break;
        case 41:
            message02();
            break;
        case 42:
            message03();
            break;
        case 43:
            message04();
            break;
        case 44:
            message05();
            break;
        case 45:
            message06();
            break;
        case 46:
            message07();
            break;
        case 47:
            message08();
            break;
        case 48:
            message09();
            break;
        case 49:
            message10();
            break;
    }
    return 0;
}
