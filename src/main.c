/**************************************************************************************************\
* 
* Main function. Simply presents a menu with a number of examples and cracks.
*
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crack.h"
#include "enigma.h"
#include "exampleEnigma.h"
#include "exampleTuring.h"
#include "exampleCoincidence.h"
#include "log.h"

int main()
{
    char input[10];
    int  i;
    
    printf("#####################################################################################\n");
    printf("# ENIGMA SIMULATION\n");
    printf("#####################################################################################\n");

	printf(" 0.     Enigma M3 example                               \n");
	printf(" 1.     Enigma M4 \"Shark\" example                     \n");
	printf(" 2.     Enigma M3 example - random settings             \n");
	printf(" 3.     Enigma M4 \"Shark\" example - random settings   \n");

	printf("10.     Turing crib circles/loops                       \n");
	printf("11-13.  Turing examples                                 \n");

	printf("20.     Gillogly IoC example 0  - original example 7 st - IOC     \n");
	printf("21.     Gillogly IoC example 1  - 8 steckers            - IOC     \n");
	printf("22.     Gillogly IoC example 1  - 8 steckers            - IOC_R3  \n");
	printf("23.     Gillogly IoC example 1  - 8 steckers            - NGRAM   \n");
	printf("24.     Gillogly IoC example 2  - 10 steckers           - IOC     \n");
	printf("25.     Gillogly IoC example 3  - long English          - IOC     \n");
	printf("26.     Gillogly IoC example 4  - short English         - IOC     \n");
	printf("27.     Gillogly IoC example 7 Geocache GC6ZZBB     \n");
	printf("28.     Gillogly IoC example 7 Geocache GC6ZZBB log \n");
	printf("29.     Gillogly IoC example NGRAMS                 \n");
	
    printf("30.     The third problem                           \n");
	printf("31.     The fourth problem                          \n");
	printf("32.     The sixth problem                          \n");

	printf("41-50.  Engima Challenge message 1-10   \n");


    fgets(input, 10, stdin);
    i=atoi(input);
    printf("Selection: %d\n", i);

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
        case 2: 
            // Simple encode/decode example on Engima M3 - Random settings v
            enigmaExampleRandomM3();           
            break;
        case 3: 
            // Simple encode/decode example on Engima M4 - Random settings v
            enigmaExampleRandomM4();           
            break;

        case 10:
            // The Turing method: find the crib loops and show they work v
            turingProve();
            break;        
        case 11: 
            // The Turing method: working example -> readable message, not all steckersettings are found
            turingExample();
            break;    
        case 12: 
            // The Turing method
            turingExample2();
            break;    
        case 13: 
            // The Turing method
            turingExample3();
            break;    

        case 20: 
            // The James Gillogly method example v
            iocExample00();
            break;
        case 21: 
            // 2nd IoC example, METHOD_IOC, 8 steckers
            iocExample01Ioc();
            break;
        case 22:  
            // 2nd IoC example, METHOD_IOC_R3 method, works
            iocExample01IocR3();
            break;
        case 23:  
            // 2nd IoC example, METHOD_NGRAM method, works
            iocExample01Ngram2();
            break;
        case 24:  
            // 3rd example, 10 steckes
            iocExample02Ioc();
            break;
        case 25:  
            // Some long english text, piece of cake for METHOD_IOC
            iocExample03Ioc();
            break;
        case 26:  
            // Some short english text, METHOD_IOC_R2R3 finds a close solution
            iocExample04Ioc();
            break;
        case 27:  
            // Test; 7A does not succeed
            iocExample07A();
            break;
        case 28:  
            // Test; 7A does not succeed
            iocExample07B();
            break;
        case 29:  
            // Test; 7A does not succeed
            // TO DO
            // iocExampleNgram();
            break;
        case 30:
            theThirdProblem();
            break;
        case 31:
            theFourthProblem();
            break;
        case 32:
            theSixthProblem();
            break;
        case 33:
            // TO DO
            //iocExampleNgram();
            break;    
        case 41:
            message01();
            break;
        case 42:
            message02();
            break;
        case 43:
            message03();
            break;
        case 44:
            message04();
            break;
        case 45:
            message05();
            break;
        case 46:
            message06();
            break;
        case 47:
            message07();
            break;
        case 48:
            message08();
            break;
        case 49:
            message09();
            break;
        case 50:
            message10_step01();
            break;
        case 51:
            message10_step02();
            break;
        case 52:
            message10_exp();
            break;
        case 53:
            message10_step01Limited();
            break;
        case 99:
//            ngramTest6();
            break;
        default:
            logError("Invalid selection %d", i);
            break;
    }
    return 0;
}
