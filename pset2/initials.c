#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string a = get_string();
    if (a != NULL)
    {
        //prints first letter in upper case
        printf("%c", toupper(a[0]));
        for (int i = 0, j = strlen(a); i < j; i++)
        {
            //Finds the space -ascii 32- adds one, then capitalizes 
            if ((int)a[i] == 32)
            {
                printf("%c", toupper(a[(int)i + 1]));
            }
        }
        printf("\n");
    }

}