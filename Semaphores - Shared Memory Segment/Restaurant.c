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

int main()
{

	srand(time(NULL));
	int retval, i, pid;
	char waiter_id[4];
	char customer_id[4];
	char rand_cust[4];
	int temp[4];

	int counter = 2;
	for (i = 0; i < 4; i++)
	{
		temp[i] = counter;
		counter = counter + 2;
	}

#pragma region MakeSharedMemorySegment

	//Share Memory Code
	int id = 0;
	int err = 0;

	//Make shared memory segment 
	id = shmget(IPC_PRIVATE, 1000, 0666);
	if (id == -1)
		perror("Creation");

	//Key with shared memory id
	char key[20];
	sprintf(key, "%d", id);

	//Attach struct into shared memory and initialize variables
	combo *Mystruct;
	Mystruct = (combo*)shmat(id, (void*)0, 0);
	if ((int)Mystruct == -1)
		perror("Attachment.");
	else
	{
		Mystruct->customers = 0;
		Mystruct->empty_seats = 0;
		Mystruct->isfull = 0;
		Mystruct->total = 0;
		Mystruct->serves = 0;
		Mystruct->average_time_serve = 0;


		//Initialize tables
		for (i = 0; i < NUM_OF_TABLES; i++)
		{
			int random = rand() % 3;
			Mystruct->tables[i].number = temp[random];
			Mystruct->tables[i].isoccupied = 0;
		}

		// Initialize semaphore mutex
		retval = sem_init(&(Mystruct->mutex), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore customer
		retval = sem_init(&(Mystruct->customer), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore doorman
		retval = sem_init(&(Mystruct->doorman), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore response
		retval = sem_init(&(Mystruct->response), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore call
		retval = sem_init(&(Mystruct->call), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore bar
		retval = sem_init(&(Mystruct->bar), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

		// Initialize semaphore reciept
		retval = sem_init(&(Mystruct->reciept), 1, 0);
		if (retval != 0) {
			perror(" Couldn ’t initialize . ");
			exit(3);
		}

	}

#pragma endregion

	printf("Restaurant Opened!\n");

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

	//Waiter Initialization
	for (i = 0; i < NO_OF_WAITERS; i++)
	{
		pid = fork();

		if (pid < 0) {
			printf("Error");
			exit(1);
		}
		else if (pid == 0)
		{
			//Create waiter_id
			sprintf(waiter_id, "%d", i);

			//Execute waiter program
			char *arglist[] = { "b",key,waiter_id,NULL };
			execv("b", arglist);

		}
		else
		{
			sleep(10);
		}
	}

	//Doorman Initialization
	pid = fork();

	if (pid < 0) {
		printf("Error");
		exit(1);
	}
	else if (pid == 0)
	{
		//Execute waiter program
		char *arglist[] = { "do",key,NULL };
		execv("do", arglist);

	}
	else
	{
		sleep(10);
	}


	//Customer Initialization
	for (i = 0; i <= NO_OF_CUST; i++)
	{

		//Create random group of customers
		int random_customers = (rand() % 7) + 1;
		sprintf(rand_cust, "%d", random_customers);

		pid = fork();

		if (pid < 0) {
			printf("Error");
			exit(1);
		}
		else if (pid == 0)
		{
			//Create customer_id
			sprintf(customer_id, "%d", i);

			//Execute customer program
			char *arglist[] = { "cu",rand_cust,customer_id,key,NULL };
			execv("cu", arglist);
		}
		else
		{
			sleep(10);
		}
	}


	printf("Restaurant Closed!\n");

	printf("------------------------------------\n");

	printf("Restaurant's income: %d euros\n", Mystruct->total);
	printf("Groups served: %d\n", Mystruct->serves);
	if (Mystruct->serves != 0)
	{
		printf("Average serve time: %d sec\n", Mystruct->average_time_serve / Mystruct->serves);
	}
	else
	{
		printf("Average serve time: 0 sec\n");
	}

	//Remove segment 
	err = shmctl(id, IPC_RMID, 0);
	if (err == -1)
		perror("Removal.");


	return 0;


}