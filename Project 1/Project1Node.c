#include <stdio.h>
#include <stdlib.h>

// this is the node struct
struct node
{
    int data;
    struct node *next;
};

struct node *node(int num)
{
    struct node *temp = malloc(sizeof(struct node));
    temp->data = num;
    temp->next = NULL;
    return temp;
}

struct node *appendData(struct node *head, int data)
{
    // First we are creating the new node that will be appended to the list
    struct node *newNode = node(data);

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
    return head;
}

struct node *appendNode(struct node *head, struct node *newNode)
{
    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        struct node *iterator = head;
        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }
        // at this point we have reach the final node,
        // so since we are appending a node (possibly one with many nodes following it),
        // we just set the next value of the node to equal this newNode
        iterator->next = newNode;
    }
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
        temp = appendData(temp, num);
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

// this is the function that merges the two lists, it assumes both lists are already sorted
struct node *merge(struct node *left, struct node *right)
{
    struct node *leftIterator = left;
    struct node *rightIterator = right;
    struct node *newHead = NULL;

    while (leftIterator != NULL && rightIterator != NULL)
    {
        if (leftIterator->data > rightIterator->data)
        {
            newHead = appendData(newHead, rightIterator->data);
            rightIterator = rightIterator->next;
        }
        else
        {
            newHead = appendData(newHead, leftIterator->data);
            leftIterator = leftIterator->next;
        }
    }
    // at this point, one of the lists ran out of stuff to use, so we just append the other list
    if (leftIterator == NULL)
    {
        newHead = appendNode(newHead, rightIterator);
    }
    else
    {
        newHead = appendNode(newHead, leftIterator);
    }
    return newHead;
}