#include <stdio.h>
#include <cs50.h>
int main(void)
{
    printf("Minutes: ");
    int a = get_int();
    printf("\nBottles: %i\n", (a * 192) / 16);
}