#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// This program takes a cipher input and applies it to a message.

int main(int argc, string argv[])
{
    // Check for proper argument count. If infelicitous, return angry message.
    if (argc != 2)
    {
        printf("Invalid argument selection.");
        return 1;
    }

    // Convert key string to int array after checking if it's all alpha.
    string key = argv[1];
    int cipher_len = strlen(key);
    int cipher_arr[cipher_len];

    for (int i = 0; i < cipher_len; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Please enter a cipher using only alphabetic characters. \n");
            return 1;
        }
    }

    // Convert the cipher to an array of numbers.
    for (int i = 0; i < cipher_len; i++)
    {
        if (islower(key[i]))
        {
            cipher_arr[i] = (key[i] - 'a');
            printf("%i", cipher_arr[i]);
        }
        else
        {
            cipher_arr[i] = (key[i]) - 'A';
            printf("%i", cipher_arr[i]);
        }

    }

    // Get the string from the user, apply the cipher.
    string message = get_string("Please enter the message you'd like to encode: ");
    int cipher_pos = 0;
    for (int i = 0; i < strlen(message); i++)
    {
        // Checks if a char in the string is alpha. Splits into lower and uppercase. Each type is run through the cipher.
        if (isalpha(message[i]))
        {
            if (islower(message[i]))
            {
                message[i] = message[i] + (cipher_arr[(cipher_pos % cipher_len)]);
                cipher_pos++;
                if (message[i] > 'z')
                {
                    message[i] = message[i] - 26;
                }
            }
            else if (isupper(message[i]))
            {
                message[i] = message[i] + (cipher_arr[(cipher_pos % cipher_len)]);
                cipher_pos++;
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