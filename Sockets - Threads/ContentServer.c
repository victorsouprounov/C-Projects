#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <string.h>
#include  <ctype.h>

void sanitize(char *str)
{
	char *src, *dest;
	for (src = dest = str; *src; src++)
		if (*src == '/' || isalnum(*src))
			*dest++ = *src;
	*dest = '\0';
}



int main(int argc, char *argv[])
{
	struct sockaddr_in myaddr; 
	int	c, lsock, csock, port;
	FILE	*sock_fp, *pipe_fp;	    
	char    dirname[BUFSIZ], command[BUFSIZ], func[BUFSIZ], filename[BUFSIZ], buf_address[BUFSIZ], buf_delay[BUFSIZ], fetch_adrr[BUFSIZ];
	int i;

	port = atoi(argv[2]);

	if ((lsock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);
	myaddr.sin_family = AF_INET;

	if (bind(lsock, (struct sockaddr *)&myaddr, sizeof(myaddr)))
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(lsock, 5) != 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Running...\n");

	while (1)
	{ 

		if ((csock = accept(lsock, NULL, NULL)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		if ((sock_fp = fdopen(csock, "r+")) == NULL)
		{
			perror("fdopen");
			exit(EXIT_FAILURE);
		}

		if (fgets(func, BUFSIZ, sock_fp) == NULL)
		{
			perror("reading func");
			exit(EXIT_FAILURE);
		}

		if (strcmp(func, "LIST\n") == 0)
		{

			#pragma region LIST

			if (fgets(buf_address, BUFSIZ, sock_fp) == NULL)
			{
				perror("reading address");
				exit(EXIT_FAILURE);
			}

			//----------------------------------------------------------


			if (fgets(buf_delay, BUFSIZ, sock_fp) == NULL)
			{
				perror("reading delay");
				exit(EXIT_FAILURE);
			}

			int Delay = atoi(buf_delay);

			//-----------------------------------------------------------

			if (fgets(dirname, BUFSIZ, sock_fp) == NULL)
			{
				perror("reading dirname");
				exit(EXIT_FAILURE);
			}

			sanitize(dirname);
			snprintf(command, BUFSIZ, "ls %s", dirname);

			if ((pipe_fp = popen(command, "r")) == NULL)
			{
				perror("popen");
				exit(EXIT_FAILURE);
			}

			while ((c = getc(pipe_fp)) != EOF)
			{
				if (c != 10 || c != 13)
				{
					putc(c, sock_fp);
				}
			}

			pclose(pipe_fp);
			fclose(sock_fp);


#pragma endregion

		}
		else if (strcmp(func, "FETCH\n") == 0)
		{

			#pragma region FETCH

			if (fgets(fetch_adrr, BUFSIZ, sock_fp) == NULL)
			{
				perror("reading address");
				exit(EXIT_FAILURE);
			}

			//----------------------------------------------------------

			if (fgets(filename, BUFSIZ, sock_fp) == NULL)
			{
				perror("reading filename");
				exit(EXIT_FAILURE);
			}

			char *filearr;
			int num = strlen(filename) - 1;
			filearr = malloc(num * sizeof(char));
		
			strncpy(filearr, filename, num);

			if (csock > 0)
			{

				FILE* fp = fopen(filearr, "r");

				if (fp)
				{

					char buf[50];
					int  bytesRead = 0;
					int  error = 0;

					while (!error && (bytesRead = fread(buf, sizeof(char), sizeof(buf), fp)) > 0)
					{
						int bytesWritten = 0;

						while (bytesWritten != bytesRead)
						{
							int ret = send(csock, buf + bytesWritten, bytesRead - bytesWritten, 0);

							if (ret > 0)
							{
								bytesWritten += ret;
							}
							else if (ret < 0)
							{
								error = 1;
								break;

							}
						}
					}

					fclose(fp);

				}
				else
				{
					perror("Server failed to open file.");
				}

				close(csock);
			}
			else
			{
				perror("accept() failed.");
			}


#pragma endregion

		}
		else if (strcmp(func, "END\n") == 0)
		{
			break;
		}

	}

	return 0;
}

