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

	int key = atoi(argv[1]);
	int i;


	//Attach the segment for Mystruct
	combo *Mystruct;
	Mystruct = (combo*)shmat(key, (void*)0, 0);
	if ((int)Mystruct == -1)
		perror("Attachment.");


	printf("Doorman is ready\n");

	while (1)
	{
		sem_wait(&Mystruct->customer);	//Doorman waits for group of customers

		printf("Doorman is searching for table\n");

		int serving_time = (rand() % TIME) + 1;

		for (i = 0; i <serving_time; i++)  //Ammount of time taken in order to serve the group
		{
			sleep(1);
		}

		int searcher = 0;
		for (i = 0; i < NUM_OF_TABLES; i++)  //Checks for available table
		{
			if ((Mystruct->current_group <= Mystruct->tables[i].number) && (Mystruct->tables[i].isoccupied == 0))
			{
				searcher = 1;
				Mystruct->tables[i].isoccupied = 1;
				Mystruct->customer_table = i;
				break;
			}

		}

		if (searcher == 0)  //Not available table
		{
			printf("Not available table\n");
			Mystruct->isfull = 1;
			sem_post(&Mystruct->doorman);

		}
		else  //Group sit in table
		{

			Mystruct->isfull = 0;
			printf("\n");
			for (i = 0; i < NUM_OF_TABLES; i++)
			{

				printf("Table %d has capacity of %d seats\n", i + 1, Mystruct->tables[i].number);
				if (Mystruct->tables[i].isoccupied == 1)
				{
					printf("Table %d occupied: YES\n", i + 1);
				}
				else
				{
					printf("Table %d occupied: NO\n", i + 1);
				}

			}
			printf("\n");

			sem_post(&Mystruct->response);  //Waiter comes to serve the group
		}


	}
	return 0;
}