/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "dictionary.h"

// Make a node type for linked list.
const int ALPHABET_SIZE = 27;
typedef struct _node
{
    struct _node *children[ALPHABET_SIZE];
    bool is_word;
}
node;

node *root_ptr;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    string word_cpy = strcpy(word);
    int index[strlen(word_cpy)];
    
    for (int i = 0; i < strlen(word_cpy); i++)
    {
        // If it's a letter or apostrophe, check for node at that value.
        if (isalpha(word[i]) || word[i] == '\'')
        {
            index[i] = tolower(c) - 'a';
        }
        else if (c == '\'')
        {
            index[i] = 26;
        }
     }

    // Initialize pointer to traverse and create tries.
    node *root = malloc(sizeof(node));
    root_ptr = root;
    node *trie_ptr = root_ptr;
    if(!root_ptr)
    {
        printf("Could not allocate memory. Aborting.\n");
        fclose(dict);
        return 0;
    }

    trie_ptr = root_ptr;
    char c;
    // Start populating the trie from the dictionary.
    while (true)
    {
        // Get all chars in the file.
        int index;
        c = fgetc(dict);
        index = get_index(c);
        bool invalid = 0;

        // Check if it's EOF. If it is, add last word and return value.
        if (fgetc(dict) == EOF)
        {
            trie_ptr-> is_word = 1;
            *count_ptr = *count_ptr + 1;
                                printf("Count: %i\n", count);
            fclose(dict);
            return 1;
        }

        // Check to see if word is invalid because of non alpha, non ' character.
        if (index == 98 || index == 99)
        {
            if (index == 99)
            {
                trie_ptr = root_ptr;
                invalid = 1;
            }
            else if (index == 98 && invalid == 0)
            {
                trie_ptr->is_word = 1;
                trie_ptr = root_ptr;
                *count_ptr = *count_ptr + 1;
                                    printf("Count: %i\n", count);
            }
            else if (index == 98 && invalid != 1)
            {
                trie_ptr = root_ptr;
                invalid = 0;
            }

        }

        // Otherwise, we have a valid input character. Continue to the next branch.
        // If the branch doesn't exist, create a node. Else, traverse forward.
        else if (trie_ptr->children[index] == NULL)
        {
            node* next = calloc(1, sizeof(node));

            // Check to make sure mem allocation was successful.
            if (next == NULL)
            {
                printf("Memory could not be allocated.\n");
                return 2;
            }
            trie_ptr->children[index] = next;
            trie_ptr = next;
        }

        // Else the branch exists. Traverse forward.
        else if (trie_ptr)
        {
            trie_ptr = trie_ptr->children[index];
        }
    }
}


/**
 * Takes a character as a parameter from A-z and indexes it from 0-26
 * for position in an array of pointers.
 */

 int get_index(char c)
 {
    // If it's a letter or apostrophe, check for node at that value.
    if (isalpha(c) || c == '\'')
    {
        return (tolower(c) - 'a');
    }
    else if (c == '\'')
    {
        return 26;
    }
    else if (c == '\n')
    {
        return 98;
    }
    else return 99;
 }

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */

// Also this variable that counts words.
unsigned int count = 0;
unsigned int *count_ptr = &count;

bool load(const char* dictionary)
{
    FILE* dict = fopen(dictionary, "r");
    // Implement root node of trie.

    if (dict == NULL)
    {
        printf("Sorry. Unable to load %s.\n", dictionary);
        fclose(dict);
        return 0;
    }

    // Initialize pointer to traverse and create tries.
    node *root = malloc(sizeof(node));
    root_ptr = root;
    node *trie_ptr = root_ptr;
    if(!root_ptr)
    {
        printf("Could not allocate memory. Aborting.\n");
        fclose(dict);
        return 0;
    }

    trie_ptr = root_ptr;
    char c;
    // Start populating the trie from the dictionary.
    while (true)
    {
        // Get all chars in the file.
        int index;
        c = fgetc(dict);
        index = get_index(c);
        bool invalid = 0;

        // Check if it's EOF. If it is, add last word and return value.
        if (fgetc(dict) == EOF)
        {
            trie_ptr-> is_word = 1;
            *count_ptr = *count_ptr + 1;
                                printf("Count: %i\n", count);
            fclose(dict);
            return 1;
        }

        // Check to see if word is invalid because of non alpha, non ' character.
        if (index == 98 || index == 99)
        {
            if (index == 99)
            {
                trie_ptr = root_ptr;
                invalid = 1;
            }
            else if (index == 98 && invalid == 0)
            {
                trie_ptr->is_word = 1;
                trie_ptr = root_ptr;
                *count_ptr = *count_ptr + 1;
                                    printf("Count: %i\n", count);
            }
            else if (index == 98 && invalid != 1)
            {
                trie_ptr = root_ptr;
                invalid = 0;
            }

        }

        // Otherwise, we have a valid input character. Continue to the next branch.
        // If the branch doesn't exist, create a node. Else, traverse forward.
        else if (trie_ptr->children[index] == NULL)
        {
            node* next = calloc(1, sizeof(node));

            // Check to make sure mem allocation was successful.
            if (next == NULL)
            {
                printf("Memory could not be allocated.\n");
                return 2;
            }
            trie_ptr->children[index] = next;
            trie_ptr = next;
        }

        // Else the branch exists. Traverse forward.
        else if (trie_ptr)
        {
            trie_ptr = trie_ptr->children[index];
        }
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return count;
}

void bye_trie(node * ptr)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (ptr->children[i] != NULL)
        {
            bye_trie(ptr->children[i]);
        }
    }
    free(ptr);
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    bye_trie(root_ptr);
    return 1;
}

