/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <cs50.h>
#include "dictionary.h"
#include <stdio.h>
#include <ctype.h>

unsigned int word_count = 0;
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;

node root;
node* current = &root;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node* current = &root;
    // TODO
    int index = 0;
    for (int i = 0; word[i] != '\0'; i++) //for each letter in input word
    {
        if (word[i] == '\'')
        {
            index = 26;
        }
        else
        {
            char words = tolower(word[i]);
            index = (int)words - 97;
        }
        
        //go to coreeesponding element in children
        if (current->children[index] == NULL) //word is mispelled
        {
            return false;
        }
        else //(current->children[index] != NULL)//, move to next letter
        {
            current = current->children[index];
        }
    }
        //once at end of input word
    return current->is_word;//check if is_word is true
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("File does not exist.\n");
        return false;
    }
    
    node* current = &root;
    if (current == NULL)    
    {
        //No pointer exist so create new one and link root to it
        node* node = malloc(sizeof(node));
        current = node;
    }
    int c = fgetc(fp);
    while (c != EOF)
    {
        
        //start of a new word
        if (c == 0x0A)
        {
            //in the case of a word ending
            current->is_word = true;
            //reset node
            current = &root;
            word_count++;
            c = fgetc(fp);
            continue;
        }
        
        if (c == 39)
        {
            c = 26;
        }
        else
        {
            c = c % 97;
            //c = c % 26;
        }
        
        
        if(current->children[c] == NULL)
        {
            current->children[c] = malloc(sizeof(node));
            current = current->children[c];
        }
        else if(current->children[c] != NULL)
        {
            current = current->children[c];
        }
        c = fgetc(fp);
    }
    return true;
    fclose(fp);
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }
    else 
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < 27; i++)
    {
        if (current->children[i] != NULL)
        {
            current = current->children[i];
            unload();
        }
    }
    free(current);
    return true;
}

