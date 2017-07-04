#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = 0;
    while(true)
    {
        printf("Height: ");
        height = get_int();
        if (height <= 23 && height >= 0)
            break;
    }
    
    int blocks = 2;
    int spaces = height - 1;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < spaces; j++){
            printf(" ");
        }
        spaces = spaces - 1;
        for(int j = 0; j < blocks; j++){
            printf("#");
        }
        blocks = blocks + 1;
        printf("\n");
    }
}