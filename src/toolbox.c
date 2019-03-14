


/**************************************************************************************************\
* 
* Converts an integer GrundStellung or RingStellung (A-Z, a-z, 1-26) to a unified position (0-25)
* 
\**************************************************************************************************/
int stellungToPos(int stellung)
{
    int pos;
    
    if (stellung>='A' && stellung<='Z')
    {
        pos=stellung-'A';
    }
    else if (stellung>='a' && stellung<='z')
    {
        pos=stellung-'a';
    }
    else if (stellung>=1 && stellung<=26)
    {
        pos=stellung-1;
    }
    else
    {
        pos=0;
    }    
    
    return pos;
}


/**************************************************************************************************\
* 
* Converts an text character (A-Z, a-z) to a unified position (0-25)
* 
\**************************************************************************************************/
int charToPos(char theChar)
{
    int pos;
    
    if (theChar>='A' && theChar<='Z')
    {
        pos=theChar-'A';
    }
    else if (theChar>='a' && theChar<='z')
    {
        pos=theChar-'a';
    }
    else
    {
        pos=-1;
    }    
    
    return pos;
}