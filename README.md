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
* **enigma**, which executes a brute force example
* **test**, which execute module tests


## Usage of the software

Typical usage:
```    
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
    
    placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
    
    encodeDecode(enigma);

    result=toString(enigma);
    destroyEngima(enigma);
```    
Note that the rotors positions are numbered from left to right, starting with 1 as shown below. Note that in *these interface functions* for the letters ASCII characters can be used (like 'A', 'B', 'C', ... 'Z', 'a', 'b', ... 'z') as well as digits (1, 2, ... 26).

![](images/positions.png)

In the code the internal representation is optimized for performance. Rotor numbering is the other way round (0 for the rightmost rotor, numbering up to the left) and 0..25 for letters. Refer to the code for more information.

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
It uses the fact that letter frequency in plain text isn't random. He uses the index of coincidence as measure of 'non-randomness'. The Gillogly method consists of following steps:
1. Find the rotors and Grundstellung that result in the largest IoC value, assuming a fixed Ringstellung of 1-1-1. 
1. Find the Ringstellung with the highes IoC value, first by varying the Ringstellung of Rotor 3, then of Rotor 2 (and changing the Grundstellung accordingly). 
1. Find the steckers using a  'hill-climbing' technique.

The 1st step is the most time consuming. 

In this software the 2nd step has been improved. In the original method Ringstellung and Grundstellung are simply done not taking into account the workings of the Enigma. In this software the working of the Enigma is simulated when changing Ringstellung and Grundstellung.

See [the original article](http://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm) (it is enclosed in the /documents folder as well).
The method described is implemented as 

    iocExample00();

The implementation distributes the work over a number of threads, so the cores of multi core processors can be used to speed up the work.

## Bgrams, Trigrams, Ngrams
As an alternative for finding the steckers a method that scores the decoded text using trigrams. The method is described by https://cryptocellar.org/pubs/bgac.pdf.
After finding the rotor settings and first steckers using the Gillogly method, the method succeeds better in fining the final steckers. It scores each trigram in the decoded text with the chance of this trigram occuring in average plain text. The more the decoded text approaches plain text, the higher the score. 
Whereas the Gillogly requires large cyphers, this ngram method performs better for shorter cyphers. The method is implemented in 

    exampleNgram();

A quick method is
 
    ngramTest();

## Code structure
![](images/code.png)
In green the core files
In purple the cracking methods
In grey internal files
In blue example and test files

### Inner workings
The green files in the picture above implement the Enigma simulation.

The current state of the machine is defined by the ```Enigma``` structure. Using the various functions an instance of this structure can be configured.

The main function is encodeDecode().

```
void encodeDecode(Enigma* enigma)
{
    int charIndex;
    
    charIndex=0;
    while (charIndex<enigma->textSize)
    {
        advance(enigma);

        enigma->conversion[charIndex]=encodeCharacter(enigma, enigma->text[charIndex]);
        
        charIndex++;
    }
}
```

It basically boils down to two functions: ```advance()``` to advance the rotors and ```encodeCharacter()``` for encoding/decoding a character of the text given the new rotor position. The ```advance()``` and its brother ```reverse()``` can be used to advance and reverse the rotors; these also come in handy in various decryption algorithms. Both functions simulate the 'double step' of the middle rotor, which is shown in the image below. 

![](images/doublestep.png)


