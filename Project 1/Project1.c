#include <stdio.h>
#include <stdlib.h>
struct node
{
    int data;
    struct node *next;
};

struct node *append(struct node *head, int data)
{
    // First we are creating the new node that will be appended to the list
    struct node *newNode = NULL;
    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) // If the list is empty (the head is null), we will just return the new node since it will be the head of the list
    {
        head = newNode;
    }
    else // since the head is not null, we will traverse the list until we reach the end and then append the new node to the end
    {
        struct node *iterator = head;
        while (iterator->next != NULL) // contuinue traversing until we reach the end of the list (where the next node is null)
        {
            iterator = iterator->next;
        }
        // at this point we have reach the final node, so we append the new node to the end by setting the final nodes next pointer to the new node
        iterator->next = newNode;
    }
    
    // now we return the head
    return head;
}

int count(struct node *head)
{
    int count = 0;
    struct node *iterator = head;
    while (iterator != NULL)
    {
        count++;
        iterator = iterator->next;
    }
    return count;
}

int main()
{
    printf("Hello, World!");

    // node *head = malloc(sizeof(node));
    struct node *head = malloc(sizeof(struct node));
    head->data = 10;
    head->next = NULL;
    struct node *nextnode = malloc(sizeof(struct node));
    nextnode->data = 12;
    nextnode->next = NULL;
    head->next = nextnode;
}