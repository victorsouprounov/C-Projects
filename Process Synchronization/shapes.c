#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
	float	x;
	float	y;
} MyRecord;


void CircleArgs(char temp[10], char x[10], char y[10], char r[10], int cnt, int pos, int j, int i, int flag, int xflag, int yflag, int rflag, char commands[10][50], int nprocesses)
{
	for (i = 0; i <= 9; i++)
	{
		temp[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		x[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		y[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		r[i] = '\0';
	}

	cnt = 0;
	pos = 0;
	j = 0;
	flag = 0;
	xflag = 0;
	yflag = 0;
	rflag = 0;

	while (commands[nprocesses][j] != ',')
	{
		temp[pos] = commands[nprocesses][j];

		if (commands[nprocesses][j] == ' ')
		{
			cnt++;

			if (cnt == 1 && flag == 0)
			{
				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				flag = 1;
				j++;
				continue;
			}
			else if (cnt == 2 && xflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					x[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				xflag = 1;
				j++;
				continue;
			}
			else if (cnt == 3 && yflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					y[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				yflag = 1;
				rflag = 1;
				j++;
				continue;
			}

		}
		else
		{
			if (rflag == 1)
			{
				r[pos] = temp[pos];
				j++;
				pos++;
				continue;
			}
			else
			{
				j++;
				pos++;
				continue;
			}
		}

	}
}

void SemicircleArgs(char temp[10], char x[10], char y[10], char r[10], char ori[10], int cnt, int pos, int j, int i, int flag, int xflag, int yflag, int rflag, int oriflag, char commands[10][50], int nprocesses)
{

	//Get semicircle args from commands array

	for (i = 0; i <= 9; i++)
	{
		temp[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		x[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		y[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		r[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		ori[i] = '\0';
	}

	cnt = 0;
	pos = 0;
	j = 0;
	flag = 0;
	xflag = 0;
	yflag = 0;
	rflag = 0;
	oriflag = 0;

	while (commands[nprocesses][j] != ',')
	{
		temp[pos] = commands[nprocesses][j];

		if (commands[nprocesses][j] == ' ')
		{
			cnt++;

			if (cnt == 1 && flag == 0)
			{
				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				flag = 1;
				j++;
				continue;
			}
			else if (cnt == 2 && xflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					x[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				xflag = 1;
				j++;
				continue;
			}
			else if (cnt == 3 && yflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					y[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				yflag = 1;
				j++;
				continue;
			}
			else if (cnt == 4 && rflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					r[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				rflag = 1;
				oriflag = 1;
				j++;
				continue;
			}
		}
		else
		{
			if (oriflag == 1)
			{
				ori[pos] = temp[pos];
				j++;
				pos++;
				continue;
			}
			else
			{
				j++;
				pos++;
				continue;
			}
		}

	}

}

void RingArgs(char temp[10], char x[10], char y[10], char r1[10], char r2[10], int cnt, int pos, int j, int i, int flag, int xflag, int yflag, int r1flag, int r2flag, char commands[10][50], int nprocesses)
{

	//Get ring args from commands array

	for (i = 0; i <= 9; i++)
	{
		temp[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		x[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		y[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		r1[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		r2[i] = '\0';
	}

	cnt = 0;
	pos = 0;
	j = 0;
	flag = 0;
	xflag = 0;
	yflag = 0;
	r1flag = 0;
	r2flag = 0;

	while (commands[nprocesses][j] != ',')
	{
		temp[pos] = commands[nprocesses][j];

		if (commands[nprocesses][j] == ' ')
		{
			cnt++;

			if (cnt == 1 && flag == 0)
			{
				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				flag = 1;
				j++;
				continue;
			}
			else if (cnt == 2 && xflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					x[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				xflag = 1;
				j++;
				continue;
			}
			else if (cnt == 3 && yflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					y[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				yflag = 1;
				j++;
				continue;
			}
			else if (cnt == 4 && r1flag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					r1[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				r1flag = 1;
				r2flag = 1;
				j++;
				continue;
			}
		}
		else
		{
			if (r2flag == 1)
			{
				r2[pos] = temp[pos];
				j++;
				pos++;
				continue;
			}
			else
			{
				j++;
				pos++;
				continue;
			}
		}

	}


}

void SquareArgs(char temp[10], char x[10], char y[10], char r[10], int cnt, int pos, int j, int i, int flag, int xflag, int yflag, int rflag, char commands[10][50], int nprocesses)
{
	//Get square args from commands array

	for (i = 0; i <= 9; i++)
	{
		temp[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		x[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		y[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		r[i] = '\0';
	}

	cnt = 0;
	pos = 0;
	j = 0;
	flag = 0;
	xflag = 0;
	yflag = 0;
	rflag = 0;

	while (commands[nprocesses][j] != ',')
	{
		temp[pos] = commands[nprocesses][j];

		if (commands[nprocesses][j] == ' ')
		{
			cnt++;

			if (cnt == 1 && flag == 0)
			{
				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				flag = 1;
				j++;
				continue;
			}
			else if (cnt == 2 && xflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					x[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				xflag = 1;
				j++;
				continue;
			}
			else if (cnt == 3 && yflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					y[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				yflag = 1;
				rflag = 1;
				j++;
				continue;
			}

		}
		else
		{
			if (rflag == 1)
			{
				r[pos] = temp[pos];
				j++;
				pos++;
				continue;
			}
			else
			{
				j++;
				pos++;
				continue;
			}
		}

	}

}

void EllipseArgs(char temp[10], char h[10], char k[10], char a[10], char b[10], int cnt, int pos, int j, int i, int flag, int hflag, int kflag, int aflag, int bflag, char commands[10][50], int nprocesses)
{
	//Get ellipse args from commands array

	for (i = 0; i <= 9; i++)
	{
		temp[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		h[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		k[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		a[i] = '\0';
	}

	for (i = 0; i <= 9; i++)
	{
		b[i] = '\0';
	}

	cnt = 0;
	pos = 0;
	j = 0;
	flag = 0;
	hflag = 0;
	kflag = 0;
	aflag = 0;
	bflag = 0;

	while (commands[nprocesses][j] != ',')
	{
		temp[pos] = commands[nprocesses][j];

		if (commands[nprocesses][j] == ' ')
		{
			cnt++;

			if (cnt == 1 && flag == 0)
			{
				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				flag = 1;
				j++;
				continue;
			}
			else if (cnt == 2 && hflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					h[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				hflag = 1;
				j++;
				continue;
			}
			else if (cnt == 3 && kflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					k[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				kflag = 1;
				j++;
				continue;
			}
			else if (cnt == 4 && aflag == 0)
			{

				for (i = 0; i <= 9; i++)
				{
					a[i] = temp[i];
				}

				for (i = 0; i <= 9; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				aflag = 1;
				bflag = 1;
				j++;
				continue;
			}
		}
		else
		{
			if (bflag == 1)
			{
				b[pos] = temp[pos];
				j++;
				pos++;
				continue;
			}
			else
			{
				j++;
				pos++;
				continue;
			}
		}

	}


}

int main(int argc, char *argv[])
{
#pragma region Declarations
	int i, j;
	int workers = atoi(argv[4]);
	char fileName[50];
	strcpy(fileName, argv[2]);
	FILE *filepointer;
	MyRecord rec;
	long lSize;
	int numOfrecords;
	pid_t pid;
	char buffer[50], offset[15], pointstoread[15];
	int cnt, pos, flag, xflag, yflag, rflag, oriflag, r1flag, r2flag, hflag, kflag, aflag, bflag;
	char x[10], y[10], r[10], ori[2], r1[10], r2[10], h[10], k[10], a[10], b[10], temp[10];
	int fd, fd1, fd2, fd3, fd4;
	int *WorkerPoints, *Offset;
	int total = 0;
	WorkerPoints = (int *)malloc((workers + 1) * sizeof(int));
	Offset = (int *)malloc((workers + 1) * sizeof(int));
	char commands[10][50];


#pragma endregion


#pragma region Calculate number of records in file

	//Calculate number of records in file
	filepointer = fopen(argv[2], "rb");
	if (filepointer == NULL) {
		printf("Cannot open binary file\n");
		return 1;
	}
	fseek(filepointer, 0, SEEK_END);
	lSize = ftell(filepointer);
	rewind(filepointer);
	numOfrecords = (int)lSize / sizeof(rec);



#pragma endregion


#pragma region Calculate PointsToReadCount and lastworkerpoints

	//Calculate PointsToReadCount and lastworkerpoints
	int PointsToReadCount;
	int lastworkerpoints;

	PointsToReadCount = (numOfrecords + (workers - 1)) / workers;
	if (PointsToReadCount*(workers - 1) < numOfrecords)
	{
		lastworkerpoints = numOfrecords - PointsToReadCount*(workers - 1);
	}
	else if (PointsToReadCount*(workers - 1) >= numOfrecords)
	{
		PointsToReadCount = numOfrecords / workers;
		lastworkerpoints = numOfrecords - PointsToReadCount*(workers - 1);
	}


#pragma endregion


#pragma region User Input

	//Command line interface
	printf("Enter command: ");
	char buf[1000];
	gets(buf);

	//If the user types exit,then the programm terminates
	if (strcmp(buf, "exit") == 0)
	{
		return 0;
	}

	//Initialize array
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 49; j++)
		{
			commands[i][j] = '\0';
		}
	}

	//Fill the array with commands given from the user
	int kk = 0;
	i = 0;
	j = 0;
	int flagg = 0;
	while (buf[kk] != ';')
	{

		if (buf[kk] == ' ' && (flagg == 1))
		{
			flagg = 0;
			kk++;
			continue;

		}

		if (buf[kk] != ',')
		{
			commands[i][j] = buf[kk];
			j++;
			kk++;
		}
		else
		{
			commands[i][j] = buf[kk];
			flagg = 1;
			i++;
			j = 0;
			kk++;
		}

	}
	commands[i][j] = ',';

#pragma endregion

	int numberofhandlers = i + 1;

#pragma region Calculate PointsToReadCount for every worker

	//Calculate PointsToReadCount for every worker

	for (i = 0; i < workers; i++)
	{
		if (i == (workers - 1))
		{
			WorkerPoints[i] = lastworkerpoints;
		}
		else
		{
			WorkerPoints[i] = PointsToReadCount;
		}

	}

#pragma endregion


#pragma region Calculate offset for every worker
	//Calculate offset for every worker		
	total = 0;
	for (i = 0; i < workers; i++)
	{
		if (i == 0)
		{
			Offset[i] = 0;
		}
		else
		{
			total = total + WorkerPoints[i - 1] * 8;
			Offset[i] = total;
		}
	}


#pragma endregion

	//Create handlers
	int ind = 0;
	for (i = 0; i<numberofhandlers; i++)
	{
		if ((pid = fork()) < 0)
		{
			perror("fork");
		}
		else if (pid == 0)
		{
			fd = open(buf, O_RDONLY | O_CREAT | O_TRUNC | O_NONBLOCK);

			//Get handler's proccess ID
			int handlerpid;
			handlerpid = getpid();

#pragma region Get utility from commands array

			//Get utility from commands array
			int counter = 0;
			j = 0;
			while (commands[i][j] != ' ')
			{
				j++;
				counter++;
			}

			char *utility;
			utility = (char *)malloc(sizeof(counter + 1));
			j = 0;
			while (commands[i][j] != ' ')
			{
				utility[j] = commands[i][j];
				j++;
			}



#pragma endregion


			if (strcmp(utility, "circle") == 0)
			{
				int ind_i = i;
				CircleArgs(temp, x, y, r, cnt, pos, j, i, flag, xflag, yflag, rflag, commands, ind_i);

				//Create workers
				pid_t pid;
				for (i = 0; i < workers; i++)
				{
					if ((pid = fork()) < 0)
					{
						perror("fork");
					}

					//Create output file name
					sprintf(buffer, "%d_w%d.fifo", handlerpid, i + 1);

					if (pid == 0)
					{
						int g;
						g = i;
						//Create offset
						sprintf(offset, "%d", Offset[g]);

						//Create pointstoread
						sprintf(pointstoread, "%d", WorkerPoints[g]);

						//Fifo operation
						if (mkfifo(buffer, 0700) < 0) {
							printf("Error creating fifo \n");
						}
						//Execute utility
						char *arglist[] = { "circle","-i",fileName,"-o",buffer,"-a",x,y,r,"-f",offset,"-n",pointstoread,NULL };
						execv("circle", arglist);
						exit(0);
					}
					else if (pid > 0)
					{

						int status;
						pid = wait(&status);
					}
				}
			}
			else if (strcmp(utility, "semicircle") == 0)
			{
				int ind_j = i;

				SemicircleArgs(temp, x, y, r, ori, cnt, pos, j, i, flag, xflag, yflag, rflag, oriflag, commands, ind_j);

				//Create workers
				pid_t pid;
				for (i = 0; i < workers; i++)
				{
					if ((pid = fork()) < 0)
					{
						perror("fork");
					}

					//Create output file name
					sprintf(buffer, "%d_w%d.fifo", handlerpid, i + 1);

					if (pid == 0)
					{
						int l;
						l = i;

						//Create offset
						sprintf(offset, "%d", Offset[l]);

						//Create pointstoread
						sprintf(pointstoread, "%d", WorkerPoints[l]);

						//Fifo operation
						if (mkfifo(buffer, 0700) < 0) {
							printf("Error creating fifo \n");
						}


						//Execute utility
						char *arglist[] = { "semicircle","-i",fileName,"-o",buffer,"-a",x,y,r,ori,"-f",offset,"-n",pointstoread,NULL };
						execv("semicircle", arglist);
						exit(0);
					}
					else if (pid > 0)
					{
						int status;
						pid = wait(&status);
					}
				}
			}
			else if (strcmp(utility, "ring") == 0)
			{
				int ind_k = i;

				RingArgs(temp, x, y, r1, r2, cnt, pos, j, i, flag, xflag, yflag, r1flag, r2flag, commands, ind_k);

				//Create workers
				pid_t pid;
				for (i = 0; i < workers; i++)
				{
					if ((pid = fork()) < 0)
					{
						perror("fork");
					}

					//Create output file name
					sprintf(buffer, "%d_w%d.fifo", handlerpid, i + 1);

					if (pid == 0)
					{
						int l;
						l = i;

						//Create offset
						sprintf(offset, "%d", Offset[l]);

						//Create pointstoread
						sprintf(pointstoread, "%d", WorkerPoints[l]);

						//Fifo operation
						if (mkfifo(buffer, 0700) < 0) {
							printf("Error creating fifo \n");
						}

						//Execute utility
						char *arglist[] = { "ring","-i",fileName,"-o",buffer,"-a",x,y,r1,r2,"-f",offset,"-n",pointstoread,NULL };
						execv("ring", arglist);
						exit(0);
					}
					else if (pid > 0)
					{
						int status;
						pid = wait(&status);
					}
				}
			}
			else if (strcmp(utility, "square") == 0)
			{
				int ind_l = i;
				SquareArgs(temp, x, y, r, cnt, pos, j, i, flag, xflag, yflag, rflag, commands, ind_l);

				//Create workers
				pid_t pid;
				for (i = 0; i < workers; i++)
				{
					if ((pid = fork()) < 0)
					{
						perror("fork");
					}

					//Create output file name
					sprintf(buffer, "%d_w%d.fifo", handlerpid, i + 1);

					if (pid == 0)
					{
						int u;
						u = i;
						//Create offset
						sprintf(offset, "%d", Offset[u]);

						//Create pointstoread
						sprintf(pointstoread, "%d", WorkerPoints[u]);

						//Fifo operation
						if (mkfifo(buffer, 0700) < 0) {
							printf("Error creating fifo \n");
						}

						//Execute utility
						char *arglist[] = { "square","-i",fileName,"-o",buffer,"-a",x,y,r,"-f",offset,"-n",pointstoread,NULL };
						execv("square", arglist);
						exit(0);
					}
					else if (pid > 0)
					{

						int status;
						pid = wait(&status);
					}
				}
			}
			else if (strcmp(utility, "ellipse") == 0)
			{
				int ind_m = i;

				EllipseArgs(temp, h, k, a, b, cnt, pos, j, i, flag, hflag, kflag, aflag, bflag, commands, ind_m);

				//Create workers
				pid_t pid;
				for (i = 0; i < workers; i++)
				{
					if ((pid = fork()) < 0)
					{
						perror("fork");
					}

					//Create output file name
					sprintf(buffer, "%d_w%d.fifo", handlerpid, i + 1);

					if (pid == 0)
					{
						int t;
						t = i;

						//Create offset
						sprintf(offset, "%d", Offset[t]);

						//Create pointstoread
						sprintf(pointstoread, "%d", WorkerPoints[t]);

						//Fifo operation
						if (mkfifo(buffer, 0700) < 0) {
							printf("Error creating fifo \n");
						}

						//Execute utility
						char *arglist[] = { "ellipse","-i",fileName,"-o",buffer,"-a",h,k,a,b,"-f",offset,"-n",pointstoread,NULL };
						execv("ellipse", arglist);
						exit(0);
					}
					else if (pid > 0)
					{
						int status;
						pid = wait(&status);
					}
				}
			}

		}
		else if (pid > 0)
		{
			int status;
			pid = wait(&status);
			printf("Handler with PID %ld exited with status 0x%x.\n", (long)pid, status);

		}

	}



	return 0;
}
