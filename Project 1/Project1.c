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
    struct node *newNode = malloc(sizeof(struct node)); // malloc is used to allocate the memory with a size of the node struct
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

int listContainsNumber(struct node *head, int num)
{
    struct node *iterator = head;
    while (iterator != NULL)
    {
        if (iterator->data == num)
        {
            return 1;
        }
        iterator = iterator->next;
    }
    return 0;
}

struct node *makeRandomListNoRepeats(int size)
{
    int i;
    struct node *temp = malloc(sizeof(struct node));

    for (i = 0; i < size - 1; i++)
    {
        int num = rand() % size;
        while (listContainsNumber(temp, num) == 1) // making sure that the new number is not already in the list
        {
            num = rand() % size;
        }
        temp = append(temp, num);
    }
    return temp;
}

// this function will be used when dividing the list in halves during the merge sorting
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

void printList(struct node *head)
{
    struct node *iterator = head;
    while (iterator != NULL)
    {
        printf("%d\n", iterator->data);
        iterator = iterator->next;
    }
}

int main()
{
    struct node *head = makeRandomListNoRepeats(10);
    printList(head);
}