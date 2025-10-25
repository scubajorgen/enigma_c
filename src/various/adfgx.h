

void    adfgxAlloc              (int keySize, int cipherSize);
void    adfgxFree               ();
void    adfgxNormalizeCipher    (char* cipher, int cipherLength);
char*   adfgxDecrypt            (char* key, int keyLength);
float   adfgxIndexOfCoincidence ();