#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// This program reverse engineers a password from a hash bashed on the crack() function.

int main(int argc, string argv[])
{
    int SALT_LENGTH = 3;

    // Check argument count passed in through command line. If not 2, terminate.
    if (argc != 2)
    {
        printf("Invalid command. Please specify a hash.\n");
        return 1;
    }

    // Declare variables: hash string, salt, and the key which is the hash minus the first two characters in this particular example.

    int hash_length = strlen(argv[1]);
    char key[hash_length - SALT_LENGTH + 1];

    // Assign values to the hash from the input string.
    char * hash = argv[1];


    // Assign first 2 characters of user's hash to the salt.
    char salt[3];
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';


    // Assign values to the key string. This is all characters after the first two of the hash.
    for (int i = SALT_LENGTH; i < hash_length + 1; i++)
    {
        key[i - SALT_LENGTH] = hash[i - 1];
    }
    key[hash_length - SALT_LENGTH] = '\0';

    // ASCII characters []./ etc are between z and A, so just define a string to check.
    char* range = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Define constants for "range" array index.
    int RANGE_MIN = 0;
    int RANGE_MAX = 51;

    // Nested "for" loops. First loop will check all 1-character passwords,
    // second loop checks for all 2-character passwords, and so on.
    for(int i = RANGE_MIN; i <= RANGE_MAX; i++)
    {
        // Passwords are passed as strings, which means length is one greater than the string
        // (the terminating character must be \0 for the crypt() function to run correctly.
        char password1[] = {'\0', '\0'};
        password1[0] = range[i];

        // Check for equivalency of the password and the hash. If equivalent, end program
        // and print the decrypted password.
        if (!strcmp(crypt(password1, salt), hash))
        {
            printf("Password: %s\n", password1);
            return 0;
        }
        for(int j = RANGE_MIN; j <= RANGE_MAX; j++)
        {
            char password2[] = {range[i], '\0', '\0'};
            password2[1] = range[j];
            if (!strcmp(crypt(password2, salt), hash))
            {
                printf("Password: %s\n", password2);
                return 0;
            }
            for(int k = RANGE_MIN; k <= RANGE_MAX; k++)
            {
                char password3[] = {range[i], range[j], '\0', '\0'};
                password3[2] = range[k];
                if (!strcmp(crypt(password3, salt), hash))
                {
                    printf("Password: %s\n", password3);
                    return 0;
                }
                for(int l = RANGE_MIN; l <= RANGE_MAX; l++)
                {
                    char password4[] = {range[i], range[j], range[k], '\0', '\0'};
                    password4[3] = range[l];
                    if (!strcmp(crypt(password4, salt), hash))
                    {
                        printf("Password: %s", password4);
                        return 0;
                    }
                }
            }
        }
    }
}