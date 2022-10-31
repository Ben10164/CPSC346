/**
 * Your name and section 
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];
/*define semaphores and mutex*/


int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
    /* Acquire Empty Semaphore */

    /*
    P(empty)
    buf[nextitem] = item
    if(++nextin >= BSIZE)
        nextin = 0
    V(occupied)
    */
	
	/* Acquire mutex lock to protect buffer */
	

	/* Release mutex lock and full semaphore */
	

	return 0;
}

int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */

    /*
    char item;
    P(occupied); // the condition to start
    item = buf[nextout];
    if(++nextout >= BSIZE)
        nextout = 0;
    V(empty);
    return item;
    */

	/* Acquire mutex lock to protect buffer */
 

	/* Release mutex lock and empty semaphore */
	 

	return 0;
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	/*call atoi to get arguments */

 

	/* Create the producer and consumer threads */
 

	for(j = 0; j < consumerThreads; j++)
	{
 

	}

	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
		 

	while(TRUE)
	{


	}

}

void *consumer(void *param)
{
	 
	 

	while(TRUE)
	{
        sleep(); // change the length it sleeps and find one that works
        int item = rand();
        insert_item(item);

	}
}
