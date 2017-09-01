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
#include <time.h>

#define MAXBUFF 1024
#define PERMS   0666

typedef struct jobtable
{
	int jobid;
	int pid;
	int status; // Active-0 Finished-1 Signaled-2

}jobtable;

struct jobtable *JobTable = NULL;
extern int errno;
int job_counter = 0;

void Handler(int signum)
{
	pid_t childpid;
	int status;

	while ((childpid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		if (WIFEXITED(status))	// Normal Exit
		{
			int i;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].pid == childpid)
				{
					JobTable[i].status = 1;
					break;
				}

			}
		}

		if (WIFSIGNALED(status)) //Signaled
		{

			int i;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].pid == childpid)
				{
					JobTable[i].status = 2;
					break;
				}

			}
		}
	}

	if (childpid == -1 && errno != ECHILD)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

}


int main(int argc, char *argv[])
{
	int Fd, filed, i, j, status, num, x, pos, flag, counter, jobnameflag, jobparflag, linecounter, command_value = 0;
	int job_param, cur_pid, cur_status, cur_jobID,cur_i,cnt;
	char msgbuf[MAXBUFF], command[20], tempor[20], jobname[20], jobpar[20], *path, dirinfo[MAXBUFF], filename[MAXBUFF], filename2[MAXBUFF], origin_path[MAXBUFF];
	pid_t pid, temp, return_pid, pool_pid;
	int exit_status;


	char jms_in[80];
	strcpy(jms_in, "/tmp/");
	strcat(jms_in, argv[6]);

	signal(SIGCHLD, Handler);

	while (1)
	{

#pragma region FIFO Operation

		//Create FIFO
		if ((mkfifo(jms_in, PERMS) < 0) && (errno != EEXIST))
		{
			perror("Can't create write FIFO.");
		}

		//Open FIFO
		if ((Fd = open(jms_in, O_RDONLY)) < 0)
		{
			perror("Can't open write FIFO");
		}

		//Read Data
		if (read(Fd, msgbuf, MAXBUFF) < 0)
		{
			perror("Error in reading.");
		}

#pragma endregion


#pragma region Read Command

		for (i = 0; i < 20; i++)
		{
			command[i] = '\0';
		}

		j = 0;
		while (msgbuf[j] != ' ')
		{
			command[j] = msgbuf[j];
			j++;
		}


#pragma endregion		


#pragma region Set Command Value


		if (strcmp(command, "submit") == 0)
		{
			command_value = 1;
		}

		if (strcmp(command, "status") == 0)
		{
			command_value = 2;
		}

		if (strcmp(command, "status-all") == 0)
		{
			command_value = 3;
		}

		if (strcmp(command, "show-active") == 0)
		{
			command_value = 4;
		}

		if (strcmp(command, "show-pools") == 0)
		{
			command_value = 5;
		}

		if (strcmp(command, "show-finished") == 0)
		{
			command_value = 6;
		}

		if (strcmp(command, "suspend") == 0)
		{
			command_value = 7;
		}

		if (strcmp(command, "resume") == 0)
		{
			command_value = 8;
		}

		if (strcmp(command, "shutdown") == 0)
		{
			command_value = 9;
		}


#pragma endregion


		switch (command_value)
		{

		case 1:

#pragma region Submit <job>


#pragma region Read Parameters

			linecounter = 0;
			while (msgbuf[linecounter] != '\0')
			{
				linecounter++;
			}

			for (i = 0; i <= 19; i++)
			{
				tempor[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				jobname[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				jobpar[i] = '\0';
			}

			pos = 0;
			counter = 0;
			x = 0;
			flag = 0;
			jobnameflag = 0;
			jobparflag = 0;

			while (x < (linecounter))
			{
				tempor[pos] = msgbuf[x];

				if (msgbuf[x] == ' ')
				{
					counter++;

					if (counter == 1 && flag == 0)
					{
						for (i = 0; i <= 19; i++)
						{
							tempor[i] = '\0';
						}
						pos = 0;
						flag = 1;
						x++;
						continue;
					}
					else if (counter == 2 && jobnameflag == 0)
					{
						for (i = 0; i <= 19; i++)
						{
							jobname[i] = tempor[i];
						}

						for (i = 0; i <= 19; i++)
						{
							tempor[i] = '\0';
						}

						pos = 0;
						jobnameflag = 1;
						jobparflag = 1;
						x++;
						continue;
					}
				}
				else
				{
					if (jobparflag == 1)
					{
						jobpar[pos] = tempor[pos];
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


#pragma endregion

			//Create FIFO
			if ((mkfifo("fifo_in", PERMS) < 0) && (errno != EEXIST))
			{
				perror("Can't create write FIFO.");
			}

			//Create job
			if ((pid = fork()) < 0)
			{
				perror("fork");
			}
			if (pid == 0)
			{

				if ((filed = open("fifo_in", O_WRONLY)) < 0)
					perror("child - open");

				temp = getpid();

				if (num = write(filed, &temp, sizeof(temp)) < 0)
					perror("Error in writing");

				close(filed);

				path = argv[2];
				strcpy(origin_path, path);

				time_t t = time(NULL);
				struct tm tm = *localtime(&t);
				sprintf(dirinfo, "/sdi1400311_%d_%d_%d%d%d_%d%d%d", job_counter+1,temp,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
				strcat(path, dirinfo);
				mkdir(path, 0777);

				sprintf(filename, "/stdout_%d.txt", job_counter + 1);
				strcat(path, filename);

				FILE *f = fopen(path, "w");
				if (f == NULL)
				{
					printf("Error opening file!\n");
					exit(1);
				}

				strcat(origin_path, dirinfo);
				sprintf(filename2, "/stderr_%d.txt", job_counter + 1);
				strcat(origin_path, filename2);

				FILE *f2 = fopen(origin_path, "w");
				if (f2 == NULL)
				{
					printf("Error opening file!\n");
					exit(1);
				}

				fclose(f);
				fclose(f2);


				char *arglist[] = { "job",jobpar,path,NULL };
				execv("job", arglist);
				exit(0);
			}
			else if (pid > 0)
			{
				if ((filed = open("fifo_in", O_RDONLY)) < 0)
					perror("parent - open");

				if ((num = read(filed, &temp, sizeof(temp))) < 0)
					perror("parent - read");

				close(filed);

				JobTable = (struct jobtable *)realloc(JobTable, (job_counter + 1) * sizeof(struct jobtable));

				JobTable[job_counter].jobid = job_counter + 1;
				JobTable[job_counter].pid = temp;
				JobTable[job_counter].status = 0;
				job_counter++;

				printf("JobID: %d, PID: %d\n", job_counter, temp);

				int status;
				pid = waitpid(-1, &status, WNOHANG | WUNTRACED);
			}

#pragma endregion

			break;

		case 2:

#pragma region Status <JobID>

#pragma region Read Parameters

			linecounter = 0;
			while (msgbuf[linecounter] != '\0')
			{
				linecounter++;
			}

			for (i = 0; i <= 19; i++)
			{
				tempor[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				jobpar[i] = '\0';
			}

			pos = 0;
			counter = 0;
			x = 0;
			flag = 0;
			jobparflag = 0;

			while (x < (linecounter))
			{

				if (msgbuf[x] != ' ')
				{
					tempor[pos] = msgbuf[x];
				}

				if (msgbuf[x] == ' ' && flag == 0)
				{

					for (i = 0; i <= 19; i++)
					{
						tempor[i] = '\0';
					}
					pos = 0;
					jobparflag = 1;
					x++;
					continue;

				}
				else
				{
					if (jobparflag == 1)
					{
						jobpar[pos] = tempor[pos];
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

#pragma endregion

			job_param = atoi(jobpar);

			cur_jobID = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].jobid == job_param)
				{
					cur_jobID = JobTable[i].jobid;
					cur_status = JobTable[i].status;
					break;
				}

			}

			if (cur_jobID == 0)
			{
				printf("There's no job with jobID %d\n", job_param);

			}
			else
			{

				if (cur_status == 0)
				{
					printf("JobID %d Status: Active\n", job_param);
				}
				else if (cur_status == 1)
				{
					printf("JobID %d Status: Finished\n", job_param);
				}

			}

#pragma endregion

			break;

		case 3:

#pragma region Status-all

			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 0)
				{
					printf("%d. JobID %d Status: Active\n", i + 1, JobTable[i].jobid);
				}
				else if (JobTable[i].status == 1)
				{
					printf("%d. JobID %d Status: Finished\n", i + 1, JobTable[i].jobid);
				}
				else if (JobTable[i].status == 2)
				{
					printf("%d. JobID %d Status: Suspended\n", i + 1, JobTable[i].jobid);
				}

			}

#pragma endregion

			break;

		case 4:

#pragma region Show-Active

			printf("Active jobs:\n");

			cnt = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 0)
				{
					cnt++;
					printf("%d. JobID %d\n", cnt, JobTable[i].jobid);
				}

			}

			if (cnt == 0)
			{
				printf("There are no active jobs\n");
			}



#pragma endregion

			break;

		case 5:

#pragma region Show-Pools

			printf("Pool & NumOfJobs:\n");
			pool_pid = getpid();

			cnt = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 0)
				{
					cnt++;
				}

			}

			if (cnt == 0)
			{
				printf("There are no active jobs\n");
			}
			else
			{
				printf("1. %d %d\n", pool_pid,cnt);
			}



#pragma endregion


			break;

		case 6:

#pragma region Show-Finished

			printf("Finished jobs:\n");

			cnt = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 1)
				{
					cnt++;
					printf("%d. JobID %d\n", cnt, JobTable[i].jobid);
				}

			}

			if (cnt == 0)
			{
				printf("There are no finished jobs\n");
			}



#pragma endregion

			break;

		case 7:

#pragma region Suspend <JobID>

#pragma region Read Parameters

			linecounter = 0;
			while (msgbuf[linecounter] != '\0')
			{
				linecounter++;
			}

			for (i = 0; i <= 19; i++)
			{
				tempor[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				jobpar[i] = '\0';
			}

			pos = 0;
			counter = 0;
			x = 0;
			flag = 0;
			jobparflag = 0;

			while (x < (linecounter))
			{

				if (msgbuf[x] != ' ')
				{
					tempor[pos] = msgbuf[x];
				}

				if (msgbuf[x] == ' ' && flag == 0)
				{

					for (i = 0; i <= 19; i++)
					{
						tempor[i] = '\0';
					}
					pos = 0;
					jobparflag = 1;
					x++;
					continue;

				}
				else
				{
					if (jobparflag == 1)
					{
						jobpar[pos] = tempor[pos];
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

#pragma endregion

			job_param = atoi(jobpar);

			cur_jobID = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].jobid == job_param)
				{
					cur_jobID = JobTable[i].jobid;
					cur_pid = JobTable[i].pid;
					cur_status = JobTable[i].status;
					cur_i = i;
					break;
				}

			}

			if (cur_jobID == 0)
			{
				printf("There's no job with jobID %d\n", job_param);

			}
			else
			{
				if (cur_status == 0)
				{
					kill(cur_pid, SIGSTOP);
					JobTable[cur_i].status = 2;
					printf("Sent suspend signal to JobID %d\n", job_param);
				}
				else
				{
					printf("Can't send suspend signal.JobID %d has finished\n", job_param);
				}



			}


#pragma endregion

			break;

		case 8:

#pragma region Resume <JobID>

#pragma region Read Parameters

			linecounter = 0;
			while (msgbuf[linecounter] != '\0')
			{
				linecounter++;
			}

			for (i = 0; i <= 19; i++)
			{
				tempor[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				jobpar[i] = '\0';
			}

			pos = 0;
			counter = 0;
			x = 0;
			flag = 0;
			jobparflag = 0;

			while (x < (linecounter))
			{

				if (msgbuf[x] != ' ')
				{
					tempor[pos] = msgbuf[x];
				}

				if (msgbuf[x] == ' ' && flag == 0)
				{

					for (i = 0; i <= 19; i++)
					{
						tempor[i] = '\0';
					}
					pos = 0;
					jobparflag = 1;
					x++;
					continue;

				}
				else
				{
					if (jobparflag == 1)
					{
						jobpar[pos] = tempor[pos];
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

#pragma endregion

			job_param = atoi(jobpar);

			cur_jobID = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].jobid == job_param)
				{
					cur_jobID = JobTable[i].jobid;
					cur_pid = JobTable[i].pid;
					cur_status = JobTable[i].status;
					cur_i = i;
					break;
				}

			}

			if (cur_jobID == 0)
			{
				printf("There's no job with jobID %d\n", job_param);

			}
			else
			{
				if (cur_status != 0)
				{
					kill(cur_pid, SIGCONT);
					JobTable[cur_i].status = 0;
					printf("Sent resume signal to JobID %d\n", job_param);
				}				
				else
				{
					printf("Can't send suspend signal.JobID %d has finished\n", job_param);
				}


			}




#pragma endregion
			break;

		case 9:

#pragma region Shutdown

			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 0 || JobTable[i].status == 2)
				{
					kill(JobTable[i].pid, SIGTERM);
				}

			}

			cnt = 0;
			for (i = 0; i < job_counter; i++)
			{
				if (JobTable[i].status == 0)
				{
					cnt++;
				}

			}



			printf("Served %d jobs, %d were still in progress\n", job_counter, cnt);

			return 0;


#pragma endregion
			break;

		}

		command_value = 0;

	}


}
