# enigma_c - Enigma simulation in C
## Introduction
This code simulates the Engima M3 and M4 encryption machine the Germans used during WWII.
I wrote this software to study the Enigma and try to crack its cyphers. It
is not meant as a user friendly program and requires C programming skills.

My first implementation was in Java (https://github.com/scubajorgen/enigma), its performance was poor for some brute forcing. 
Therefore I wrote the software also in C and optimized for performance.
On a simple low power Pentium it peforms 84000 encryptions (running on 3 threads), whereas the Java software on a I5 notebook 
performs only 1000 or so.

## Building
Simply call make.
It delivers two executable files:
* enigma, which executes a brute force example
* test, which execute module tests

## Turing method

The software implements the method used by Alan Turing to crack the German
encoded messages. It assumes a piece of plaintext that corresponds to part
of the cypher text..

## Usage

Typical usage:
    
    enigma=createEnigmaM3();
    setText(enigma, "RCGXFEAJCT");
    placeWaltze(enigma, 1, "I");
    placeWaltze(enigma, 2, "II");
    placeWaltze(enigma, 3, "III");
    
    placeUmkehrWaltze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 17);
    setRingStellung(enigma, 3, 12);

    setGrundStellung(enigma, 1, 'A');
    setGrundStellung(enigma, 2, 'B');
    setGrundStellung(enigma, 3, 'C');

    
    clearSteckerBrett(enigma);
    
    encodeDecode(enigma);

    result=toString(enigma);
    destroyEngima(enigma);
    
Note that the rotors positions are numbered from left to right, starting with 1.
Refer to the code for more information.

The Turing Bombe crack:

    turingBombe("CYPHERTEXT", "CRIB", 1);


Pass the cypher text and the crib as uppercase! Its lenght must be smaller
than or equal to the cypher text length. The crib start must correspond to
position 0 of the cypher. To use multi core processors increase the number
of threads to 2, 3 or 4.
The routine parses all 60 permutations of 5 rotors.

Or simply, for a working example:

    turingExample();



