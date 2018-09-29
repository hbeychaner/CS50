#include <stdio.h>
#include <cs50.h>

int main(void)
{
int height;
int i;
int j;

    do
    {
    height = get_int("Please enter an integer between 0 and 23 (inclusive): ");
    }
    while (height < 0 || height > 23);

    printf("Thank you for your contribution to Mario's exercise. I will now draw a pyramid for him of the height you specified.\n");
    int spaces = height;
    int stars = 0;
    while(spaces >= 0)
    {
    for(i = 0; i <= spaces; i++)
    {
        printf(" ");
    }
    for(j = 0; j <= stars; j++)
    {
        printf("*");
    }
    printf("\n");
    spaces--;
    stars++;
    }
}