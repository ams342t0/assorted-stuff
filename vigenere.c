// A very simple implementation of Vigenère Cipher


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char table[26][26];


/*  
    Build the table, aka Vigenère Square
    The top row consist of letters of the alphabet in order, 
    each succeeding row consist of the letters of the previous row rotated left once
*/ 

void create_table()
{
    int ch,s=65;
    
    for (int row=0;row<26;row++)
    {
        ch = s;    
        for (int col = 0;col < 26;col++)
        {
            if (ch > 90) ch = 65;
            table[row][col]=ch;
            ch = ch + 1;
        }
        s=s+1;
        printf("\n\r");
        
    }
}


void print_table()
{
    for(int r=0;r<26;r++)
    {
        for(int c=0;c<26;c++)
            printf("%c",table[r][c]);
        
        printf("\n");
    }
}




void encode(char *text,char *key,char **cipher)
{
    int l=strlen(text);
    *cipher = (char*)malloc(l+1);
    memset((char*)*cipher,0,l+1);

    for (int i=0;i<l;i++)   
    {
        (*cipher)[i] = table[text[i]-'A'][key[i]-'A'];
    }
}



void decode(char *cipher,char *key,char **text)
{
    int l=strlen(cipher);
    int k;
    *text = (char*)malloc(l+1);
    memset((char*)*text,0,l+1);

    for (int i=0;i<l;i++)   
    {
        int trow = key[i];
        for(k=0;k<26;k++) if (table[trow-'A'][k] == cipher[i]) break;
        (*text)[i] = k+'A';
    }
}


void genkey(char *text,char *kw, char **key)
{
    int tl = strlen(text);
    int kwl = strlen(kw);
    int o;
    
    if (kwl > tl) *key=NULL;
    
    *key = (char*)malloc(tl+1);
    memset((char*)*key,0,tl+1);
    
    o = 0;
        
    while ((tl - o) >= kwl)
    {
        strncpy((*key)+o,kw,tl-o);
        o+=kwl;
    }
    
    if ((tl - o) < kwl)
    {
        strncpy((*key)+o,kw,tl-o);
    }
}

int main(int argc,char **argv)
{

    if (argc < 3) return -1;
    
    char *text=argv[1],*kw=argv[2],*key,*cipher;
    create_table();
    genkey(text,kw,&key);
    encode(text,key,&cipher);
    decode(cipher,key,&text);
    printf("\nCipher: %s\n"
             "Key   : %s\n",cipher,kw);
    printf(  "Text  : %s\n",text);
    
    return 0;
}