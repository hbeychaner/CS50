#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// This program takes a cipher and applies it to a message.

int main(int argc, string argv[])
{
    // Check for proper argument count. If infelicitous, return angry message.
    if (argc != 2)
    {
        printf("Invalid argument selection.");
        return 1;
    }

    // Convert key string to int. Make sure it's positive.

    int key = atoi(argv[1]);
    if (key < 0)
    {
        printf("Please enter a positive integer.");
        return 1;
    }
    // Get the string from the user, apply the cipher.
    string message = get_string("Please enter the message you'd like to encode: ");

    for (int i = 0; i < strlen(message); i++)
    {
        // Checks if a char in the string is alpha. Splits into lower and uppercase. Each type is run through the cipher.
        if (isalpha(message[i]))
        {
            if (islower(message[i]))
            {
                message[i] = message[i] + (key % 26);
                if (message[i] > 'z')
                {
                    message[i] = message[i] - 26;
                }
            }
            else
            {
                message[i] = message[i] + (key % 26);
                if (message[i] > 'Z')
                {
                    message[i] = message[i] - 26;
                }
            }
        }
    }
    // String has been redefined. Print the ciphered message.
    printf("Here is your ciphered message: \n%s\n", message);
}