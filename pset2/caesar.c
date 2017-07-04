#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Error, inccorect amount of comand line arguments\n");
        return 1;
    }
    printf("plaintext: ");
    string a = get_string();

    int k = atoi(argv[1]);
    printf("ciphertext: ");
    for (int i = 0, j = strlen(a); i < j; i++)
    {
        if (isalpha((int)a[i]))
        {
            if(islower(a[i]))
            {
                int b = (((int)a[i] + k -97) % 26) + 97;
                printf("%c",(char)b);
            }
            if (isupper(a[i]))
            {
                int c = (((int)a[i] + k - 65) % 26) + 65;
                printf("%c",(char)c); 
            }
        }
        else
        {
            printf("%c",a[i]);
        }
    }
    printf("\n");
}