#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

typedef struct dllist
{
    int val;
    struct dllist* prev;
    struct dllist* next;
} dllist;

dllist* insert(dllist* head, int val);
void destroy(dllist* target);

int main(void)
{
    
}

// create a linked list when it doesnt already exist
dllist* insert(dllist* head, int val)
{
    //dynamicaly allocate space for new dllnode
    dllist *node = malloc(sizeof(dllist));
    //check to make sure we didnt run out of memory
    if (node == NULL)
    {
        fprintf(stderr, "Ran out of memory");
    }
    //populate and insert the node at the beginning of the linked list
    node->val = val;
    node->next = head;
    node->prev = NULL;
    //fix the prev pointer of the old head of the linked list
    head->prev = node;
    //return a pointer ot the new head of the linked list
    return node;
}
//search through a linked list to find an element

//insert a new node into the linked list

//delete a single element from a linked list
void destroy(dllist* target)
{
    //fix surrouning nodes to "skip over" target
    target->prev->next = target->next;
    target->next->prev = target->prev;
    //free target
    free(target);
}
//delete an entire linked list

