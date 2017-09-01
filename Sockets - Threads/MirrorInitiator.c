#include <stdio.h>
#include <sys/types.h>	  
#include <sys/socket.h>	    
#include <netinet/in.h>	     
#include <unistd.h>         
#include <netdb.h>	    
#include <stdlib.h>	     
#include <string.h>

void main(int argc, char *argv[])
{
	int port, sock, i;
	char buf[256];
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	struct hostent *rem;

	// 1.Create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}


	// 2.Find server address */
	if ((rem = gethostbyname(argv[2])) == NULL)
	{
		perror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[4]); 
	server.sin_family = AF_INET;   
	memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
	server.sin_port = htons(port); 
	
	// 3.Initiate connection
	if (connect(sock, serverptr, sizeof(server)) < 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Connecting to %s port %d\n", argv[2], port);

	// 4.Pass info to mirror server
	for (i = 0; argv[6][i] != '\0'; i++)
	{

		if (write(sock, argv[6] + i, 1) < 0)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}

	}

	// 4.Close socket and exit
	printf("Closing connection.\n");
	close(sock);
}

