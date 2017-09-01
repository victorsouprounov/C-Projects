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
	srand(time(NULL));

	int i;

	int key = atoi(argv[1]);

	int waiter_id = atoi(argv[2]);


	//Attach the segment for Mystruct
	combo *Mystruct;
	Mystruct = (combo*)shmat(key, (void*)0, 0);
	if ((int)Mystruct == -1)
		perror("Attachment.");


	printf("Waiter %d is ready\n", waiter_id);

	while (1)
	{
		sem_wait(&Mystruct->response);	//Waiter waits for doorman in order to serve group

		printf("Waiter %d serves the group\n", waiter_id);

		Mystruct->serves = Mystruct->serves + 1;

		int waiter_customer_table = Mystruct->customer_table;

		int serving_time = (rand() % PERIOD) + 1;

		Mystruct->average_time_serve = Mystruct->average_time_serve + serving_time;

		for (i = 0; i < serving_time; i++)  //Ammount of time taken in order to serve the group
		{
			sleep(1);
		}

		sem_post(&Mystruct->doorman);

		sem_wait(&Mystruct->call);  //Wait for group of customers to call him to pay

		int moneyammount = rand() % MONEYAMMOUNT;  //Ammount of money 

		Mystruct->total = Mystruct->total + moneyammount;  //Calculate total restaurant's income

		printf("Waiter %d accept payment\n", waiter_id);	//Waiter accept payment

		Mystruct->tables[waiter_customer_table].isoccupied = 0;  //Waiter tells the doorman that the table is empty now

		sem_post(&Mystruct->reciept);  //Give the reciept to customers,so they can now leave

	}

	return 0;
}