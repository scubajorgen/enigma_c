# enigma_c - Enigma simulation in C
## Introduction
This code simulates the Engima M3 and M4 encryption machine the Germans used during WWII.

My first implementation was in Java (https://github.com/scubajorgen/enigma), its performance was poor for some brute forcing. 
Therefore I wrote the software also in C and optimized for performance.
On a simple low power Pentium it peforms 84000 encryptions (running on 3 threads), whereas the Java software on a I5 notebook 
performs only 1000 or so.

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
