#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("This program is designed to check whether your credit card number is valid or not.\n");
    long long card_number = get_long_long("Please enter your credit card number: ");

    int valid = 0; // Initialize the condition under which the loop will run.

    while (valid == 0) // Initialize the validation loop. Loop will cease once a valid credit card number is entered.
        if (card_number > 0) // Validate the number as a positive integer.
        {
            // Now validate the checksum. Double the second to last digit, and every other digit and add them together.
            // Then, add the other digits together to the previous sum. If the resulting sum % 10 = 0, it is valid.
            int digits = 0;
            int checksum = 0;
            long long x = card_number;
            while (x > 0)
            {
                digits++;
                checksum = checksum + (x % 10);
                x = x / 10;
                if (x > 0)
                {
                    digits++;
                    checksum = checksum + (2 * (x % 10));
                    x = x / 10;
                }
            }
            if (checksum % 10 == 0)
            {
                printf("Checksum is valid. Now checking which type of card you have.\n");
                // Now, validate the type. If 15 digits and starts with 34 or 37, it's American Express.
                if (digits == 15 && (card_number / 10000000000000 == 34 || card_number / 10000000000000 == 37))
                {
                    printf("Your card is a valid AmEx card.\n");
                    valid = 1;
                }
                // If it's 16 digits and begins with 51-55, it's Master Card.
                else if (digits == 16 && card_number / 100000000000000 >= 51 && card_number / 100000000000000 <= 55)
                {
                    printf("Your card is a valid Master Card card.\n");
                    valid = 1;
                }
                // If it's 13 or 16 digits and starts with 4, it's Visa.
                else if (digits == 16 && card_number / 1000000000000000 == 4)
                {
                    printf("Your card is a valid Visa card.\n");
                    valid = 1;
                }
                else
                {
                    card_number = get_long_long("Valid checksum, but not a recognized company. Please retry: ");
                }
            }
            else
            {
                card_number = get_long_long("Invalid checksum. Please retry: ");
            }
        }
}