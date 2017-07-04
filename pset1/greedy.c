#include <stdio.h>
#include <cs50.h>
#include<math.h>

//File returns the amount of coins that are used to give back some change
//File works by geting the amount of change, converting it to cents, then chiping away at it
//with 25,10,5,1 cent coins
int main(void)
{
    
    float change = 0;
    int num_coins = 0;
    
    do 
    {
        printf("How much change is owed?  ");
        change = GetFloat();
        change *= 100;
        change = round(change);

    }
    while (change < 0);

    while (change >= 25)
    {
        change -= 25;
        num_coins ++;
    }
    
    while (change >= 10)
    {
        change -= 10;
        num_coins ++;
    }

    while (change >= 05)
    {
        change -= 05;
        num_coins ++;
    }
    
    while (change > 0)
    {
        change -= 1;
        num_coins ++;
    }

    printf("%d\n", num_coins);
    
}
