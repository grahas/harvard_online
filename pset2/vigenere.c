#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//PROGRAM IS NOT FINNISHED RAN OUT OF TIME AND HAD TO CONTINUE PACE
int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Comand line argument error, only one command line argument accepted\n");
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Nope\n");
            return 1;
        }
    }
    
    string key = argv[1];
    printf("plaintext:");
    string plain_text = get_string();
    printf("ciphertext:");
    //encrypt the string
    for(int i = 0, j = strlen(plain_text), k = 0; i < j; i++, k++)
    {
        //reset the key if it gets to the end of the string
        if (strlen(key) == k + 1)
        {
            k = 0;
        }
        if (isupper(plain_text[i]))
        {
            printf("%c",((int)plain_text[i]+(int)key[k] % 26) + 65);
        }
        else if (islower(plain_text[i]))
        {
            printf("%c", ((int)plain_text[i]+(int)key[k] % 26) + 97);
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }
}