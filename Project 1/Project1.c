#include "Project1Node.c"
// commented out node struct for referance 

// struct node
// {
//     int data;
//     struct node *next;
// };

int main()
{
    struct node *head = makeRandomListNoRepeats(10);
    printList(head);
}