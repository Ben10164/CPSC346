/*
Ben Puryear
CPSC 346
Project 6
December 5th, 2022
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

typedef struct person
 {
    int month;
    int day;
	int year;
    struct person *next;
} node;

node* pptr;

// makes head node
void makeNode(node** ptr, int month, int day, int year)
{
    node* newNode = (node*) kmalloc(sizeof(node),GFP_USER);
    newNode -> month = month;
    newNode -> day = day;
    newNode -> year = year;
    newNode -> next = NULL;
    *ptr = newNode;
}

// makes node, then appends it to the end of the list
void appendNode(node** ptr, int month, int day, int year)
{
    node* newNode = (node*) kmalloc(sizeof(node),GFP_USER);
    newNode -> month = month;
    newNode -> day = day;
    newNode -> year = year;
    newNode -> next = NULL;
    node* temp = *ptr;
    while(temp -> next != NULL)
    {
        temp = temp -> next;
    }
    temp -> next = newNode;
}

// prints current node
void printNode(node* ptr)
{
    printk(KERN_INFO "month: %d\n", ptr -> month);
    printk(KERN_INFO "day: %d\n", ptr -> day);
    printk(KERN_INFO "year: %d\n", ptr -> year);
}

// prints list
void printNodes(node* ptr)
{
    while(ptr != NULL)
    {
        printNode(ptr);
        ptr = ptr -> next;
    }
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    makeNode(&pptr, 1, 1, 2021);
    appendNode(&pptr, 2, 2, 2022);
    appendNode(&pptr, 3, 3, 2023);
    appendNode(&pptr, 4, 4, 2024);
    appendNode(&pptr, 5, 5, 2025);

    printNodes(pptr);
    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
 {
    printk(KERN_INFO "Removing Module\n");
    pptr = NULL;
    printk(KERN_INFO "List has been Freed\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");