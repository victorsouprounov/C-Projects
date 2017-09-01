#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


typedef struct {
	float	x;
	float	y;
} MyRecord;



int main(int argc, char *argv[])
{
	int i, j, fd, r1, r2;
	float x, y, d;
	float center_h, center_k, a, b;
	int offset, pointstoread;
	char msg[100];

	char fileName[50];
	strcpy(fileName, argv[2]);

	char writeFile[50];
	strcpy(writeFile, argv[4]);

	float center_x, center_y, r;
	center_h = atof(argv[6]);
	center_k = atof(argv[7]);
	a = atof(argv[8]);
	b = atof(argv[9]);

	FILE *filepointer;
	FILE *writeptr;

	int flag = 0;
	if (strcmp(argv[0], "ellipse") == 0)
	{
		flag = 1;

		offset = atoi(argv[11]);
		pointstoread = atoi(argv[13]);

		//Fifo operation
		fd = open(writeFile, O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK);
	}
	else
	{
		writeptr = fopen(writeFile, "w");
		if (writeptr == NULL)
		{
			printf("Cannot open file \n");
			return 1;
		}
	}

	MyRecord rec;
	long lSize;
	int numOfrecords;
	filepointer = fopen(fileName, "rb");
	if (filepointer == NULL) {
		printf("Cannot open binary file\n");
		return 1;
	}

	if (flag == 1)
	{
		fseek(filepointer, offset, SEEK_CUR);

		for (i = 0; i<pointstoread; i++)
		{
			fread(&rec, sizeof(rec), 1, filepointer);

			if ((pow(rec.x - center_h, 2) / pow(a, 2) + pow(rec.y - center_k, 2) / pow(b, 2)) <= 1)
			{
				sprintf(msg, "%.1f	%.1f\n", rec.x, rec.y);
				write(fd, msg, sizeof(msg));
			}

		}
	}
	else
	{
		fseek(filepointer, 0, SEEK_END);
		lSize = ftell(filepointer);
		rewind(filepointer);
		numOfrecords = (int)lSize / sizeof(rec);

		for (i = 0; i<numOfrecords; i++)
		{
			fread(&rec, sizeof(rec), 1, filepointer);

			if ((pow(rec.x - center_h, 2) / pow(a, 2) + pow(rec.y - center_k, 2) / pow(b, 2)) <= 1)
			{
				fprintf(writeptr, "%.1f	%.1f\n", rec.x, rec.y);
			}

		}

		fclose(writeptr);
	}


	fclose(filepointer);


	return 0;

}
