/**
 * Author info and version control
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define NUMBER_OF_THREADS 3

void *sorter(void *params); /* thread that performs basic sorting algorithm */
void *merger(void *params); /* thread that performs merging of results */

int list[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};

int result[SIZE];

typedef struct
{
    int from_index;
    int to_index;
} parameters;

int main(int argc, const char *argv[])
{
    int i;

    pthread_t workers[NUMBER_OF_THREADS];

    // for testing purposes we will first print out the list
    printf("Unsorted list: ");
    for (i = 0; i < SIZE; i++)
    {
        printf("%d ", list[i]);
    }
    printf("\n");

    /* establish the first sorting thread */
    parameters *parameterValues = (parameters *)malloc(sizeof(parameters));
    parameterValues->from_index = 0;
    parameterValues->to_index = (SIZE / 2) - 1;
    pthread_create(&workers[0], 0, sorter, parameterValues);

    /* establish the second sorting thread */
    parameters *parameterValues2 = (parameters *) malloc (sizeof(parameters));
    parameterValues2->from_index = (SIZE / 2); // should be an integer rounded down
    parameterValues2->to_index = SIZE - 1;
    pthread_create(&workers[1], 0, sorter, parameterValues2);

    /* now wait for the 2 sorting threads to finish */
    pthread_join(workers[0], NULL);
    pthread_join(workers[1], NULL);

    /* establish the merge thread */
    // we are gonna reuse parameterValues since the job relating to it is guaranteed to be done (since we waited for the threads to finish)
    parameterValues->from_index = 0;        // start of first half
    parameterValues->to_index = (SIZE / 2); // start of second half
    pthread_create(&workers[2], 0, merger, parameterValues);

    /* wait for the merge thread to finish */
    pthread_join(workers[2], NULL);

    /* output the sorted array */
    printf("Sorted list: ");
    for (i = 0; i < SIZE; i++)
    {
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
    parameters *p = (parameters *)params;

    // Sorting algorithm here. It can be any sorting algorithm.
    // I will be using bubble sort
    int i, j, temp;
    for (i = p->from_index; i < p->to_index; i++)
    {
        for (j = p->from_index; j < p->to_index; j++)
        {
            if (list[j] > list[j + 1])
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
    parameters *p = (parameters *)params;

    // reuse �parameters� to hold the first index in each half
    // merge two sorted sublist to the arry result
    int i = p->from_index;
    int j = p->to_index;
    int k = 0;

    while (i < (p->to_index) && j < SIZE)
    {
        if (list[i] < list[j])
        {
            result[k] = list[i];
            i++;
        }
        else
        {
            result[k] = list[j];
            j++;
        }
        k++;
    }
    if(i < (p->to_index))
    {
        while(i < (p->to_index))
        {
            result[k] = list[i];
            i++;
            k++;
        }
    }
    else
    {
        while(j < SIZE)
        {
            result[k] = list[j];
            j++;
            k++;
        }
    }


    pthread_exit(0);
}
