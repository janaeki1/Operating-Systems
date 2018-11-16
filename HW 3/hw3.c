
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


buffer_item buffer[BUFFER_SIZE]; //buffer
int counter; //counter for buffer
sem_t empty, full; //empty and full semaphores
pthread_mutex_t mutex; //mutex lock

pthread_t pid; //ID for producer threads
pthread_t cid; //ID for consumer threads

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
	if(counter < BUFFER_SIZE) {
    	buffer[counter] = item; //add item to buffer
      	counter++; //increment counter
   	}
   	
	return 0;
}

int remove_item(buffer_item *item)
{
	if(counter > 0) {
    	*item = buffer[(counter-1)]; //remove item from buffer
      	counter--; //decrement counter
	}
	
	return 0;
}


int main(int argc, char *argv[])
{
	/* Get command line arguments argv[1],argv[2],argv[3] */
	int sleepTime, prod, cons;
	int i;
	
	if (argc != 4) {
		fprintf(stderr,"Usage: ./hw3 <sleep time> <# of producer threads> <# of consumer threads>\n");
		return -1;
	}
	
	sleepTime = atoi(argv[1]); //user input for sleep time
	prod = atoi(argv[2]); //user input for number of producer threads
	cons = atoi(argv[3]); //user input for number of consumer threads
	
	if (prod < 2) {
		fprintf(stderr,"# of producer threads must be larger than 1\n");
		return -1;
	}
	if (cons < 2) {
		fprintf(stderr,"# of consumer threads must be larger than 1\n");
		return -1;
	}
	
	/* Initialize buffer related synchronization tools */
   sem_init(&empty, 0, BUFFER_SIZE); //create empty semaphore
   sem_init(&full, 0, 0); //create full semaphore
   pthread_mutex_init(&mutex, NULL); //create mutex lock
   counter = 0; //initialize counter for buffer
	
	/* Create producer threads based on the command line input */
	for(i = 0; i < prod; i++) {
    	pthread_create(&pid, NULL, producer, NULL);
    }
	
	/* Create consumer threads based on the command line input */
	for(i = 0; i < cons; i++) {
    	pthread_create(&cid, NULL, consumer, NULL);
    }

	/*Join threads*/
	for(i = 0; i < prod; i++) {
		pthread_join(pid,NULL);
	}
	for(i = 0; i < cons; i++) {
		pthread_join(cid,NULL);
	}

	/* Sleep for user specified time based on the command line input */
	sleep(sleepTime);
	
	return 0;
}

void *producer(void *param)
{
	/* producer thread that calls insert_item() */
	buffer_item produced_number;
	
	while(1) {
		/* sleep for a random period of time */
		int random = rand() / 10000000;
		sleep(random);
		
		/* generate a random number */
		produced_number = rand();
		
		sem_wait(&empty); //acquire empty semaphore
		pthread_mutex_lock(&mutex); //acquire mutex lock
		
		if (insert_item(produced_number)){
			fprintf(stderr, "report error condition");
		}
		else{
			printf("Producer %u produced %d \n",(unsigned int)pthread_self(), produced_number);
		}
		
      pthread_mutex_unlock(&mutex); //release mutex lock
      sem_post(&full); //release full semaphore
	}
}

void *consumer(void *param)
{
	/* consumer thread that calls remove_item() */
	buffer_item consumed_number;
	
	while(1) {
	/* sleep for a random period of time */
		int random = rand() / 10000000;
		sleep(random);
		
		sem_wait(&full); //acquire full semaphore
		pthread_mutex_lock(&mutex); //acquire mutex lock
		
		if (remove_item(&consumed_number)){
			fprintf(stderr, "report error condition");
		}			
		else{
			printf("Consumer %u consumed %d \n",(unsigned int)pthread_self(), consumed_number);
		}
		
		pthread_mutex_unlock(&mutex); //release mutex lock
		sem_post(&empty); //release empty semaphore
	}
}
