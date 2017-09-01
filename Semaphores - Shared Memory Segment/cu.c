#include <unistd.h>          
#include <stdio.h>           
#include <stdlib.h>          
#include <pthread.h>         
#include <semaphore.h>   
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SEGMENTSIZE sizeof ( sem_t )
#define SEGMENTPERM 0666

#define NO_OF_CUST 4
#define NO_OF_WAITERS 2
#define NUM_OF_TABLES 4
#define PERIOD 10
#define TIME 10
#define MONEYAMMOUNT 100

typedef struct tables_av
{
	int number;
	int isoccupied;

}tables_av;


typedef struct combo {

	tables_av tables[NUM_OF_TABLES];

	sem_t mutex;
	sem_t customer;
	sem_t doorman;
	sem_t response;
	sem_t call;
	sem_t reciept;
	sem_t bar;

	int isfull;
	int empty_seats;
	int customers;
	int current_group;
	int customer_table;
	int total;
	int serves;
	int average_time_serve;

}combo;

int main(int argc, char *argv[])
{
	int group = atoi(argv[1]);

	int customer_id = atoi(argv[2]);

	int key = atoi(argv[3]);

	int i;


	//Attach the segment for Mystruct
	combo *Mystruct;
	Mystruct = (combo*)shmat(key, (void*)0, 0);
	if ((int)Mystruct == -1)
		perror("Attachment.");

	if (customer_id == NO_OF_CUST)
	{
		sem_wait(&Mystruct->mutex);		//No more customers
	}

	printf("Group of %d customers enter to standing room\n", group);

	sem_post(&Mystruct->customer);	//Group of customers enter shop;

	Mystruct->current_group = group;  //Declare group to doorman

	sem_wait(&Mystruct->doorman);	//Doorman must serve the group 

	if (Mystruct->isfull == 1)
	{
		printf("Group of %d customers are leaving the restaurant\n", group);
		sem_wait(&Mystruct->mutex);
	}

	printf("Group of %d customers are having dinner\n", group);

	int dinner_time = (rand() % PERIOD) + 1;

	for (i = 0; i < dinner_time; i++)  //Ammount of time taken for dinner
	{
		sleep(1);
	}

	printf("Group of %d customers pay the waiter\n", group);

	sem_post(&Mystruct->call);  //Group of customers call the waiter to pay

	sem_wait(&Mystruct->reciept);  //Group of customers wait for the reciept 

	printf("Group of %d customers exit the restaurant\n", group);	//Group of customers exit the restaurant

	return 0;

}