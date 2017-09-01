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

int main(int argc, char *argv[])
{
	int sec = atoi(argv[1]);
	int fw = open(argv[2], O_APPEND | O_WRONLY);
	
	printf("JOB STARTS...\n");
	sleep(sec);
	printf("JOB END.\n");

	dup2(fw, 1);
	printf("Demo message\n");

	close(fw);

	return 0;
}