#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

typedef struct sllist
{
    int val;
    struct sllist* next;
}
sllnode;

sllnode* create(int val);
bool find(sllnode* head, int val);
sllnode* insert(sllnode* head, int val);
void destroy(sllnode* head);
void print_list(sllnode * head);

int main (void)
{
    sllnode* node = create(6);
    printf("%p\n",node->next);
    printf("%i\n",node->val);
    
    bool exist = find(node,6);
    printf("%i\n",exist);
    exist = find(node,9);
    printf("%i\n",exist);
    node = insert(node, 9);
    exist = find(node,9);
    printf("%i\n",exist);
    destroy(node);
    printf("node destroyed");
}

//create a sl list
sllnode* create(int val)
{
    //dynamicaly allocate space for a new sllnode
    sllnode *root = malloc(sizeof (sllnode));
    //check to make sure we dont run out of memory
    if (root == NULL)
    {
        fprintf(stderr, "Ran out of Memory\n");
    }
    //initialize the nodes val field
    root->val = val;
    //initialize the nodes next field
    root->next = NULL;
    //return a pointer to the newly created sllnode
    return root;
}

//search a sl list
bool find(sllnode* head, int val)
{
    //create a traversal pointer pointing to list head
    sllnode* trav = head;
    //if current nodes val field is what were looking for report success
    for(trav = head; trav != NULL; trav=trav->next)
    {
        if (trav->val == val)
        {
            return true;
        }
    }
    //if you've reached the end of the list, report failure
    return false;
}

//insert a node into the li list
sllnode* insert(sllnode* head, int val)
{
    //dynamically allocate space for a new node
    sllnode* new_node = malloc(sizeof(sllnode));
    //check memory run out
    if (new_node == NULL)
    {
        fprintf(stderr, "Ran out of Memory\n");
    }    
    //populate and insert node at the beginning of the linked list
    new_node->val = val;
    new_node->next = head;
    //return a pointer to the new head of the linked list
    head = new_node;
    return head;
}

// delete an entire linked list
void destroy(sllnode* head)
{
    sllnode* tmp;
    while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
 }

//delete single element from linked list

void print_list(sllnode * head) {
    sllnode * current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}
