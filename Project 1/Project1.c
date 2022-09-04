#include "Project1Node.c"
// commented out node struct for referance

// struct node
// {
//     int data;
//     struct node *next;
// };

void testMerge()
{
    struct node *left = node(2);
    left = appendData(left, 4);
    left = appendData(left, 5);
    left = appendData(left, 8);

    struct node *right = node(1);
    right = appendData(right, 3);
    right = appendData(right, 6);
    right = appendData(right, 7);
    right = appendData(right, 9);
    right = appendData(right, 70);

    struct node *newer = merge(left, right);
    printList(newer);
}

int main()
{
    testMerge();
}
