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
Simply call 'make clean', 'make'.
It delivers two executable files:
* enigma, which executes a brute force example
* test, which execute module tests


## Usage of the software

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

## Turing method

The software implements the method used by Alan Turing to crack the German encoded messages. It assumes a piece of plain text (the crib) that corresponds to part of the cypher text. The software creates the letter links (the menu) and finds all loops in it. It then finds the rotor settings and start position that fullfills the loops.
Refer to http://www.rutherfordjournal.org/article030108.html for a good description.

The Turing Bombe crack:

    turingBombe("CYPHERTEXT", "CRIB", 1);

Note:
* Pass the cypher text and the crib as uppercase! 
* Crib length shall not exceed cypher text length. 
* Crib size should not exceed 26 characters or the loop number will explode. Space is not allocated dynamically, so arrays will get out of bounds
* The crib start must correspond to position 0 of the cypher. 
* To use multi core processors increase the number of threads (3rd parameter) to 2, 3 or 4. The routine parses all 60 permutations of 5 rotors and subdivides the work amongst the threads.

Or simply, for a working example:

    turingExample();

The software results in all rotor settings that result in the loops defined by the cypher en crib.

## Index of Coincidence - James Gillogly
James Gillogly presented a method for finding the rotor settings and the steckers using the 'Index of Coincidence'.
It uses the fact that letter frequency in plain text isn't random. He uses the index of coincidence as measure of 'non-randomness'. The method first tries to find the rotorsettings that result in the largest IoC value. It then uses a 'hill-climbing' technique to estimate the steckers that improves the IoC. 
See the article http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm
The method described is implemented as 

    iocExample();

## Bgrams, Trigrams, Ngrams
As an alternative for finding the steckers a method that scores the decoded text using trigrams. The method is described by https://cryptocellar.org/pubs/bgac.pdf.
After finding the rotor settings and first steckers using the Gillogly method, the method succeeds better in fining the final steckers. It scores each trigram in the decoded text with the chance of this trigram occuring in average plain text. The more the decoded text approaches plain text, the higher the score. 
Whereas the Gillogly requires large cyphers, this ngram method performs better for shorter cyphers. The method is implemented in 

    exampleNgram();

A quick method is
 
    ngramTest();
