#include <stdio.h>
#include <sys/wait.h>	     
#include <sys/types.h>	    
#include <sys/socket.h>	     
#include <netinet/in.h>	     
#include <netdb.h>	         
#include <unistd.h>	    	
#include <stdlib.h>	     
#include <ctype.h>	        
#include <signal.h>   
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXBUFF 1024
#define  BUFFSIZE 256
#define perror2(s, e) fprintf(stderr, "%s: %s\n", s, strerror(e))
#define BUFF_SIZE   2  



typedef struct Datatable
{
	char *address;
	int port;
	char *filename; 
	int delay;

}Datatable;

struct thread_args
{
	struct Datatable* arg1;
	int arg2;

};

struct worker_arg
{
	char* filename;

};

typedef struct BufferData
{
	char *filename;
	char *original_name;
	char *address;
	int port;

}BufferData;

typedef struct
{
	BufferData buf[BUFF_SIZE];
	int in;					
	int out;				
	sem_t full;				
	sem_t empty;				
	pthread_mutex_t mutex;    

} sbuf_t;

sbuf_t shared;
int bufferdatacounter = 0;
int datatablecounter = 0;

void *MirrorManager(void *arg)
{

	struct sockaddr_in  servadd;
	struct hostent *hp;
	int    sock1, n_read, err,i, item, index;
	char   buffer[BUFFSIZE], temp[20];

	struct thread_args *args = arg;
	struct Datatable* dt = args->arg1;
	index = args->arg2;

	#pragma region List Request
	
			if ((sock1 = socket(PF_INET, SOCK_STREAM, 0)) == -1)
			{
				perror("socket");
				exit(EXIT_FAILURE);
			}
	
			if ((hp = gethostbyname(dt[index].address)) == NULL)
			{
				herror("gethostbyname");
				exit(1);
			}
	
			memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);
			servadd.sin_port = htons(dt[index].port);
			servadd.sin_family = AF_INET;
	
			if (connect(sock1, (struct sockaddr*) &servadd, sizeof(servadd)) != 0)
			{
				perror("connect");
				exit(EXIT_FAILURE);
			}
	
			if (write_all(sock1, "LIST\n", strlen("LIST\n")) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			//-----------------------

			if (write_all(sock1, dt[index].address, strlen(dt[index].address)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			if (write_all(sock1, "\n", 1) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			//-----------------------

			char strdel[10];
			sprintf(strdel, "%d", dt[index].delay);

			if (write_all(sock1, strdel, strlen(strdel)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			if (write_all(sock1, "\n", 1) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}


			//-----------------------
	
			if (write_all(sock1, dt[index].filename, strlen(dt[index].filename)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			if (write_all(sock1, "\n", 1) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
	
			for (i = 0; i < BUFFSIZE; i++)
			{
				buffer[i] = '\0';
			}
	
			char b[1];
			int cnt = 0;
			while ((n_read = read(sock1, b, 1)) > 0)
			{
				if (b[0] == '\n')
				{
					buffer[cnt] = ';';
				}
				else
				{
					buffer[cnt] = b[0];
				}
	
				cnt++;
			}
	
			close(sock1);
	
	
	#pragma endregion


	#pragma region Insert Data 

	int linecnt = 0;
	while (buffer[linecnt] != '\0')
	{
		linecnt++;
	}

	for (i = 0; i <= 19; i++)
	{
		temp[i] = '\0';
	}
			
	int x = 0;
	int pos = 0;
	while (x < linecnt)
	{
			
		if (buffer[x] == ';')
		{
			pos = 0;

			sem_wait(&shared.empty);

			pthread_mutex_lock(&shared.mutex);

			//Insert Data
			char address[20];
			for (i = 0; i <= 19; i++)
			{
				address[i] = '\0';
			}
			strcpy(address, dt[index].address);

			char fileName[100];
			sprintf(fileName, "%s/%s", dt[index].filename, temp);
			
			shared.buf[shared.in].filename = malloc(sizeof(fileName));
			shared.buf[shared.in].original_name = malloc(sizeof(temp));
			shared.buf[shared.in].address = malloc(sizeof(address));
			strcpy(shared.buf[shared.in].filename, fileName);
			strcpy(shared.buf[shared.in].original_name, temp);
			strcpy(shared.buf[shared.in].address, address);
			shared.buf[shared.in].port = dt[index].port;

			//printf("Producing: %s | %s | %s | %d\n", shared.buf[shared.in].filename, shared.buf[shared.in].original_name, shared.buf[shared.in].address, shared.buf[shared.in].port);

			bufferdatacounter++;

			shared.in = (shared.in + 1) % BUFF_SIZE;

			fflush(stdout);

			pthread_mutex_unlock(&shared.mutex);

			sem_post(&shared.full);

			for (i = 0; i <= 19; i++)
			{
				temp[i] = '\0';
			}

			x++;
			
		}
		else
		{
			temp[pos] = buffer[x];
			pos++;
			x++;
		}
			
	}


#pragma endregion

	return NULL;
}

void *Worker(void *arg)
{
	BufferData item;

	struct worker_arg *args = arg;

	mkdir(args->filename, 0777);

	while (1)
	{
		sem_wait(&shared.full);

		pthread_mutex_lock(&shared.mutex);

		item = shared.buf[shared.out];

		bufferdatacounter--;

		//printf("   Consuming: %s | %s | %d\n", item.filename, item.address, item.port);

		#pragma region Fetch

		struct sockaddr_in  servadd;
		struct hostent *hp;
		int    sock1;

		if ((sock1 = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			exit(EXIT_FAILURE);
		}

			if ((hp = gethostbyname(item.address)) == NULL)
			{
				herror("gethostbyname");
				exit(1);
			}
		
			memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);
			servadd.sin_port = htons(item.port);
			servadd.sin_family = AF_INET;
		
			if (connect(sock1, (struct sockaddr*) &servadd, sizeof(servadd)) != 0)
			{
				perror("connect");
				exit(EXIT_FAILURE);
			}
		
			if (write_all(sock1, "FETCH\n", strlen("FETCH\n")) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			//-----------------------------------------------------------

			if (write_all(sock1, item.address, strlen(item.address)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			if (write_all(sock1, "\n", 1) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}


			//-----------------------------------------------------------

			
			if (write_all(sock1, item.filename, strlen(item.filename)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
		
			if (write_all(sock1, "\n", 1) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}

			char subfolder[100];
			sprintf(subfolder, "%s/%s_%d/",args->filename,item.address, item.port);
			mkdir(subfolder, 0777);
			
			char openptr[100];
			sprintf(openptr, "%s/%s_%d/", args->filename,item.address,item.port);
			strcat(openptr, item.original_name);

			FILE* fp = fopen(openptr, "w");
		
			if (fp)
			{
				int done = 0;
		
				while (!done)
				{
					char buf[50] = { 0 };
		
					int bytesRcvd = recv(sock1, buf, sizeof(buf), 0);
		
					if (bytesRcvd > 0)
					{
						fwrite(buf, sizeof(char), bytesRcvd, fp);
					}
					else if (bytesRcvd == 0)
					{
						done = 1;
					}
					else if (bytesRcvd < 0)
					{
						perror("Failed to get data from the server.");
						done = 1;
					}
				}
		
				fclose(fp);
			}
			else
			{
				perror("Client failed to open file.");
			}
		
			close(sock1);
		
		
		#pragma endregion

		shared.out = (shared.out + 1) % BUFF_SIZE;

		fflush(stdout);

		pthread_mutex_unlock(&shared.mutex);

		sem_post(&shared.empty);

	}

	return NULL;
}

int write_all(int fd, void *buff, size_t size) {
	int sent, n;
	for (sent = 0; sent < size; sent += n)
	{
		if ((n = write(fd, buff + sent, size - sent)) == -1)
			return -1;
	}
	return sent;
}

void main(int argc, char *argv[])
{

	#pragma region Variables - Declarations

	struct Datatable *Datatable = NULL;
	struct BufferData *BufferData = NULL;
	int Port, sock, newsock, i, j, num, linecounter, x, pos, counter, addressflag, portflag, filenameflag, port1, delay1;
	char address[20], port[20], filename[20], delay[20], temp[20];
	struct sockaddr_in server, client;
	int err;
	socklen_t clientlen;

	struct sockaddr *serverptr = (struct sockaddr *)&server;
	struct sockaddr *clientptr = (struct sockaddr *)&client;
	struct hostent *rem;

	//signal(SIGCHLD, Handler);


#pragma endregion

	#pragma region Socket Creation

	Port = atoi(argv[2]);

	// Create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(Port);

	// Bind socket to address
	if (bind(sock, serverptr, sizeof(server)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Listen for connections
	if (listen(sock, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Mirror Server: Listening for connections to port %d\n", Port);

	// Accept connection
	if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}


#pragma endregion

	#pragma region Read Data From Mirror Initiator

	char line[MAXBUFF];
	for (i = 0; i < MAXBUFF; i++)
	{
		line[i] = '\0';
	}

	linecounter = 0;
	num = 0;
	char buf[1];
	while (read(newsock, buf, 1) > 0)
	{
		if (buf[0] == ',')
		{
			num++;
		}
		line[linecounter] = buf[0];
		linecounter++;

	}

#pragma endregion

	#pragma region Create Data Table

	Datatable = (struct Datatable *)calloc(num + 1, sizeof(struct Datatable));
	datatablecounter = num + 1;

	x = 0;
	j = 0;
	while (x < linecounter)
	{
		pos = 0;
		counter = 0;

		addressflag = 0;
		portflag = 0;
		filenameflag = 0;

		for (i = 0; i <= 19; i++)
		{
			address[i] = '\0';
		}

		for (i = 0; i <= 19; i++)
		{
			port[i] = '\0';
		}

		for (i = 0; i <= 19; i++)
		{
			filename[i] = '\0';
		}

		for (i = 0; i <= 19; i++)
		{
			delay[i] = '\0';
		}

		while (line[x] != ',' && (x != linecounter))
		{
			temp[pos] = line[x];

			if (line[x] == ':')
			{
				temp[pos] = '\0';
				counter++;

				if (counter == 1 && addressflag == 0)
				{
					for (i = 0; i <= 19; i++)
					{
						address[i] = temp[i];
					}

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					addressflag = 1;
					x++;
					continue;
				}
				else if (counter == 2 && portflag == 0)
				{
					for (i = 0; i <= 19; i++)
					{
						port[i] = temp[i];
					}

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					portflag = 1;
					x++;
					continue;
				}
				else if (counter == 3 && filenameflag == 0)
				{
					for (i = 0; i <= 19; i++)
					{
						filename[i] = temp[i];
					}

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					filenameflag = 1;
					x++;
					continue;
				}

			}
			else
			{
				if (filenameflag == 1)
				{
					delay[pos] = temp[pos];
					x++;
					pos++;
					continue;
				}
				else
				{
					x++;
					pos++;
					continue;
				}
			}
		}

		x++;

		port1 = atoi(port);
		delay1 = atoi(delay);

		Datatable[j].address = malloc(sizeof(address));
		Datatable[j].filename = malloc(sizeof(filename));
		strcpy(Datatable[j].address, address);
		strcpy(Datatable[j].filename, filename);
		Datatable[j].delay = delay1;
		Datatable[j].port = port1;
		j++;
	}

	printf("\nRequests: \n");
	for (i = 0; i < j; i++)
	{
		printf("%d. %s | %d | %s | %d \n",i+1, Datatable[i].address, Datatable[i].port, Datatable[i].filename, Datatable[i].delay);
	}

#pragma endregion

	#pragma region Create Threads

	pthread_t idP, idC;
	int index;

	sem_init(&shared.full, 0, 0);
	sem_init(&shared.empty, 0, BUFF_SIZE);
	pthread_mutex_init(&shared.mutex, NULL);

	for (index = 0; index < datatablecounter; index++)
	{
		struct thread_args *args = malloc(sizeof *args);
		if (args != NULL)
		{
			args->arg1 = Datatable;
			args->arg2 = index;

		}

		pthread_create(&idP, NULL, MirrorManager, args);
	}

	struct worker_arg *arg = malloc(sizeof *arg);
	if (arg != NULL)
	{
		arg->filename = malloc(sizeof(argv[4]));
		strcpy(arg->filename, argv[4]);
	}
	pthread_create(&idC, NULL, Worker, arg);

	pthread_exit(NULL);


#pragma endregion

	printf("\nClosing connection.\n");
	close(newsock);

}

