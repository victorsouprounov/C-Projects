#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUFF 1024
#define PERMS   0666

extern int errno;

int main(int argc, char *argv[])
{
	int fd, nwrite;


	char jms_in[80];
	strcpy(jms_in, "/tmp/");
	strcat(jms_in, argv[2]);

	printf("Enter command: ");
	char buf[80];
	gets(buf);

	while (strcmp(buf, "shutdown") != 0)
	{

		if ((fd = open(jms_in, O_WRONLY | O_NONBLOCK)) < 0)
		{
			perror("FIFO open error.");
		}

		if ((nwrite = write(fd, buf, sizeof(buf))) == -1)
		{
			perror("Error in writing.");
		}

		close(fd);

		printf("Enter command: ");
		gets(buf);
	}

	if ((fd = open(jms_in, O_WRONLY | O_NONBLOCK)) < 0)
	{
		perror("FIFO open error.");
	}

	if ((nwrite = write(fd, buf, sizeof(buf))) == -1)
	{
		perror("Error in writing.");
	}

	close(fd);

	return 0;

}