#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

// functions for creating the list
int listContainsNumber(struct node *head, int num);
struct node *makeRandomListNoRepeats(int size);

void printList(struct node *head);
int count(struct node *head);
struct node *node(int num); // constructor
struct node *appendNode(struct node *head, struct node *newNode);
struct node *merge(struct node *left, struct node *right);
struct node *mergeSort(struct node *head, int size);

int main()
{
    // testMerge();
    struct node *unsortedList = makeRandomListNoRepeats(9);
    printf("%s", "Unsorted: ");
    printList(unsortedList);
    printf("%s", "Sorted: ");
    struct node *sortedList = mergeSort(unsortedList, count(unsortedList));
    printList(unsortedList);
}

struct node *node(int num)
{
    struct node *temp = malloc(sizeof(struct node));
    temp->data = num;
    temp->next = NULL;
    return temp;
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
        temp = appendNode(temp, node(num));
    }
    return temp;
}

// returns the count of a linked list
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

// function for printing a linked list
void printList(struct node *head)
{
    struct node *iterator = head;
    while (iterator != NULL)
    {
        printf("%d ", iterator->data);
        iterator = iterator->next;
    }
    printf("\n");
}

// fucntion used to append a node to the end of a list
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

// this is a function that merges two linked lists (it assumes both lists are already sorted)
struct node *merge(struct node *left, struct node *right)
{
    struct node *leftIterator = left;
    struct node *rightIterator = right;
    struct node *newHead = NULL;

    while (leftIterator != NULL && rightIterator != NULL)
    {
        struct node *temp;
        if (leftIterator->data > rightIterator->data)
        {
            temp = rightIterator;
            rightIterator = rightIterator->next;
        }
        else
        {
            temp = leftIterator;
            leftIterator = leftIterator->next;
        }
        temp->next = NULL;
        newHead = appendNode(newHead, temp);
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

// this is a function that performs merge sort on a linked list
struct node *mergeSort(struct node *head, int size)
{
    // if the list/sublist only contains 1 item, then its already sorted
    if (size == 1)
    {
        return head;
    }

    // now we are splitting the list into the two equal halves that we will be passing in
    // first we will save the head of the first half (we will change the ending of this half to be null later)
    struct node *leftHead = head;
    // now we make the iterator that will be used to go through the list to find the second half
    struct node *iterator = head;
    // we only go to the middle - 1, because we are going to set the value of the middle -1's next to be null
    for (int i = 0; i < (int)(size / 2) - 1; i++)
    {
        iterator = iterator->next;
    }
    // before we set this node to null, we should save the next node as the head of the second list
    struct node *rightHead = iterator->next;
    // now we end the first list by setting the final nodes next value to null
    iterator->next = NULL;

    // Uncomment for outputting the left and right heads during the algorithm
    /*

        printf("%s","Left head: ");
        printf("%d\n",leftHead->data);

        printf("%s","Right head: ");
        printf("%d\n",rightHead->data);
    */

    // recursion!
    struct node *sortedLeft = mergeSort(leftHead, count(leftHead));
    struct node *sortedRight = mergeSort(rightHead, count(rightHead));

    // now we do the merge part of merge sort
    struct node *sortedList = merge(sortedLeft, sortedRight);

    return sortedList;
}