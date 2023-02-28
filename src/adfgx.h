

void    adfgxAlloc              (int keySize, int cypherSize);
void    adfgxFree               ();
void    adfgxNormalizeCypher    (char* cypher, int cypherLength);
char*   adfgxDecrypt            (char* key, int keyLength);
float   adfgxIndexOfCoincidence ();