#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define SKIPLIST_MAX_LEVEL 5
#define MAXVALUE 999999

typedef struct snode {
	int  studid;
	char *lastname;
	char *firstname;
	float gpa;
	int numofcourses;
	char *deprt;
	int  postcode;

	struct snode **forward;
} snode;

typedef struct skiplist {
	struct snode *header;
} skiplist;

struct node {
	struct snode hashnode;
	struct node *next;
};

struct hash {
	struct node *head;
	int count;
}hash;


struct hash *hashTable = NULL;

int eleCount = 1;


int Hash(int key, int hashentries)
{
	int result;
	result = key % hashentries;
	return result;
}

struct node * createNode(snode hashnode) {
	struct node *newnode;
	newnode = (struct node *)malloc(sizeof(struct node));
	newnode->hashnode = hashnode;
	newnode->next = NULL;
	return newnode;
}

void insertToHash(int key, snode hashnode, int hashentries) {

	int hashIndex = Hash(key, hashentries);

	struct node *newnode = createNode(hashnode);

	if (!hashTable[hashIndex].head) {
		hashTable[hashIndex].head = newnode;
		hashTable[hashIndex].count = 1;
		return;
	}

	newnode->next = (hashTable[hashIndex].head);
	hashTable[hashIndex].head = newnode;
	hashTable[hashIndex].count++;
	return;
}


static void Init(skiplist *list, snode *tail)
{
	int i;
	tail->studid = 999999;
	tail->firstname = malloc(sizeof(100));
	tail->lastname = malloc(sizeof(100));
	tail->gpa = -1;
	tail->numofcourses = -1;
	tail->deprt = malloc(sizeof(100));
	tail->postcode = -1;

	tail->forward = (snode **)malloc(sizeof(snode*) * (SKIPLIST_MAX_LEVEL));
	for (i = 0; i <= SKIPLIST_MAX_LEVEL - 1; i++) {
		tail->forward[i] = NULL;

	}

	snode *header = (snode *)malloc(sizeof(struct snode));
	header->studid = 1;
	tail->firstname = malloc(sizeof(100));
	tail->lastname = malloc(sizeof(100));
	tail->gpa = -1;
	tail->numofcourses = -1;
	tail->deprt = malloc(sizeof(100));
	tail->postcode = -1;
	list->header = header;

	header->forward = (snode **)malloc(sizeof(snode*) * (SKIPLIST_MAX_LEVEL));
	for (i = 0; i <= SKIPLIST_MAX_LEVEL - 1; i++) {
		header->forward[i] = tail;
	}
}

int Insert(skiplist *list, snode *tail, int studid, char *lastname, char *firstname, float gpa, int numofcourses, char *deprt, int postcode, int hashentries)
{
	snode *update[SKIPLIST_MAX_LEVEL];
	snode *x = list->header;
	int i, level;

	for (i = SKIPLIST_MAX_LEVEL - 1; i >= 0; i--) {

		while (x->forward[i]->studid < studid)
			x = x->forward[i];

		update[i] = x;
	}
	x = x->forward[0];


	if (x->studid == studid) {
		return 1;

	}
	else {
		level = rand() % (SKIPLIST_MAX_LEVEL);
		x = (snode *)malloc(sizeof(struct snode));
		x->studid = studid;
		x->firstname = malloc(sizeof(firstname + 2));
		x->lastname = malloc(sizeof(lastname + 2));
		x->deprt = malloc(sizeof(deprt + 2));
		strcpy(x->lastname, lastname);
		strcpy(x->firstname, firstname);
		x->gpa = gpa;
		x->numofcourses = numofcourses;
		strcpy(x->deprt, deprt);
		x->postcode = postcode;
		x->forward = (snode **)malloc(sizeof(snode*) * (level + 1));
		for (i = 0; i < level; i++) {

			x->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = x;
		}

		if (level == 0)
		{
			x->forward[0] = update[0]->forward[0];
			update[0]->forward[0] = x;

		}

	}

	//Operation for insert into hash table

	snode hashnode;
	hashnode.studid = studid;
	hashnode.lastname = lastname;
	hashnode.firstname = firstname;
	hashnode.gpa = gpa;
	hashnode.numofcourses = numofcourses;
	hashnode.deprt = deprt;
	hashnode.postcode = postcode;

	insertToHash(hashnode.postcode, hashnode, hashentries);

	return 0;
}

void Query(skiplist *list, int studid)
{
	snode *x = list->header;
	int i;
	for (i = SKIPLIST_MAX_LEVEL - 1; i >= 0; i--) {
		while (x->forward[i]->studid < studid)
			x = x->forward[i];
	}
	x = x->forward[0];

	if (x->studid == studid) {
		printf("%d %s %s %.2f %d %s %d\n", x->studid, x->lastname, x->firstname, x->gpa, x->numofcourses, x->deprt, x->postcode);
	}
	else
	{
		printf("Not found\n");
	}

}

void Modify(skiplist *list, int studid, float gpa, int numofcourses)
{
	snode *x = list->header;
	int i;
	for (i = SKIPLIST_MAX_LEVEL - 1; i >= 0; i--) {
		while (x->forward[i]->studid < studid)
			x = x->forward[i];
	}
	x = x->forward[0];

	if (x->studid == studid) {
		x->gpa = gpa;
		x->numofcourses = numofcourses;
		printf("%d %s %s %.2f %d %s %d \n", x->studid, x->lastname, x->firstname, x->gpa, x->numofcourses, x->deprt, x->postcode);
	}
	else
	{
		printf("Not found\n");
	}


}

void Delete(skiplist *list, int studid)
{
	int i;
	snode *update[SKIPLIST_MAX_LEVEL];
	snode *x = list->header;
	for (i = SKIPLIST_MAX_LEVEL - 1; i >= 0; i--) {
		while (x->forward[i]->studid < studid)
			x = x->forward[i];
		update[i] = x;
	}

	x = x->forward[0];
	if (x->studid == studid) {
		for (i = 0; i <= SKIPLIST_MAX_LEVEL - 1; i++) {
			if (update[i]->forward[i] != x)
				break;
			update[i]->forward[i] = x->forward[i];
		}
		free(x);
	}
	else
	{
		printf("Not found \n");
	}

}

float Raverage(skiplist *list, int studida, int studidb)
{
	float result;
	float sum = 0;
	int counter = 0;
	snode *x = list->header;
	int i;
	for (i = SKIPLIST_MAX_LEVEL - 1; i >= 0; i--) {
		while (x->forward[i]->studid < studida)
			x = x->forward[i];
	}
	x = x->forward[0];

	if (x->studid == studida) {

		sum = x->gpa + sum;
		counter++;

		while (x->forward[0]->studid <= studidb)
		{

			x = x->forward[0];
			sum = x->gpa + sum;
			counter++;

		}

		result = sum / counter;
		return result;

	}
	else
	{
		printf("Not found studida \n");
		return 0;
	}

}

float Average(int key) {
	struct node *myNode;
	float result;
	float sum = 0;
	int counter = 0;

	if (hashTable[key].count == 0)
	{
		printf("No records found with this key. \n ");
		return 0;
	}
	else
	{
		myNode = hashTable[key].head;
		if (!myNode)
		{
			printf("No record! \n ");
		}
		while (myNode != NULL)
		{
			sum = myNode->hashnode.gpa + sum;
			counter++;
			myNode = myNode->next;

		}

		result = sum / counter;
		return result;
	}
}

float * Sort(float *result, int counter) {
	int i;
	int j;
	float temp;

	for (i = 1; i < counter; i++) {
		for (j = 0; j < counter - 1; j++) {
			if (result[j] > result[j + 1]) {
				temp = result[j];
				result[j] = result[j + 1];
				result[j + 1] = temp;
			}
		}
	}
	return result;

}

void Taverage(int k, int key)
{
	struct node *myNode;
	float *result;
	float *data;

	if (hashTable[key].count == 0)
	{
		printf("No records found with this key. \n ");
	}
	else
	{
		myNode = hashTable[key].head;
		if (!myNode)
		{
			printf("No record! \n ");
		}

		int counter = 0;
		while (myNode != NULL)
		{
			counter++;
			myNode = myNode->next;
		}

		myNode = hashTable[key].head;
		result = (float *)calloc(counter, sizeof(float));
		int i = 0;
		while (myNode != NULL)
		{
			result[i] = myNode->hashnode.gpa;
			i++;
			myNode = myNode->next;
		}

		data = Sort(result, counter);

		if (k > counter)
		{
			printf("Invalid parameter k \n");
		}
		else
		{
			int i = counter - 1;
			int check = 0;
			while (i >= 0)
			{
				if (check == k)
				{
					break;
				}
				else
				{
					printf("%.2f ", data[i]);
					check++;
					i--;
				}

			}
			printf("\n");
		}

	}
}

void Bottom(skiplist *list, snode *tail, int k)
{
	snode *x = list->header;
	float *result;
	float *data;

	int counter = 0;
	while (x->forward[0] != tail)
	{
		counter++;
		x = x->forward[0];
	}

	x = list->header;
	result = (float *)calloc(counter, sizeof(float));
	int i = 0;

	x = x->forward[0];
	while (x->forward[0] != tail)
	{

		result[i] = x->gpa;
		i++;
		x = x->forward[0];
	}
	result[i] = x->gpa;

	data = Sort(result, counter - 1);

	if (k > counter)
	{
		printf("Invalid parameter k \n");
	}
	else
	{
		int i = 0;
		int check = 0;
		while (i <= (counter - 1))
		{
			if (check == k)
			{
				break;
			}
			else
			{
				printf(" %.2f ", data[i]);
				check++;
				i++;
			}

		}
		printf("\n");
	}

}

void CoursesToTake(int key, char *deprt)
{
	struct node *myNode;
	int  sum = 0;
	int flag = 0;

	if (hashTable[key].count == 0)
	{
		printf("No records found with this key. \n ");
	}
	else
	{
		myNode = hashTable[key].head;
		if (!myNode)
		{
			printf("No record! \n ");
		}
		while (myNode != NULL)
		{
			if (strcmp(myNode->hashnode.deprt, deprt) == 0)
			{
				flag = 1;
				printf("%d %s %s %.2f %d %s %d \n", myNode->hashnode.studid, myNode->hashnode.lastname, myNode->hashnode.firstname, myNode->hashnode.gpa, myNode->hashnode.numofcourses, myNode->hashnode.deprt, myNode->hashnode.postcode);
				sum = myNode->hashnode.numofcourses + sum;
			}
			myNode = myNode->next;

		}

		if (flag == 1)
		{
			printf("Total courses: %d\n", sum);
		}
		else
		{
			printf("Not found \n");
		}


	}


}

void Find(skiplist *list, snode *tail, float gpa)
{
	snode *x = list->header;
	int flag = 0;
	x = x->forward[0];
	while (x != tail)
	{
		if (x->numofcourses == 52 && x->gpa > gpa)
		{
			flag = 1;
			printf("%d %s %s %.2f %d %s %d \n", x->studid, x->lastname, x->firstname, x->gpa, x->numofcourses, x->deprt, x->postcode);
		}
		x = x->forward[0];
	}

	if (flag == 0)
	{
		printf("Not found \n");
	}
}

void Percentile(skiplist *list, snode *tail, int key)
{
	float  percentage;
	int flag = 0;
	snode *x = list->header;
	int counter = 0;
	x = x->forward[0];
	while (x != tail)
	{
		counter++;
		x = x->forward[0];
	}

	struct node *myNode;
	int cnt = 0;

	if (hashTable[key].count == 0)
	{
		printf("No records found with this key. \n ");
	}
	else
	{
		myNode = hashTable[key].head;
		if (!myNode)
		{
			printf("No record! \n ");
		}
		while (myNode != NULL)
		{
			flag = 1;
			cnt++;
			myNode = myNode->next;
		}
	}

	if (flag == 1)
	{
		if (counter != 0)
		{
			percentage = (float)(100 * cnt) / counter;
			printf("Percentile:%.0f", percentage);
			printf("%%");
			printf("\n");
		}

	}
	else
	{
		printf("Not found \n");
	}

}

int Exists(int *array, int postcode, int counter)
{
	int i;
	for (i = 0; i < counter; i++) {
		if (array[i] == postcode)
			return 1;
	}
	return 0;
}

void Percentiles(skiplist *list, snode *tail, int hashentries)
{
	snode *x = list->header;
	int key;
	x = x->forward[0];
	while (x != tail)
	{
		printf("\n");
		printf("Postcode: %d \n", x->postcode);
		key = Hash(x->postcode, hashentries);
		Percentile(list, tail, key);
		x = x->forward[0];
	}

}

void Exit(skiplist *list, snode *tail)
{
	snode *x = list->header;
	x = x->forward[0];
	while (x != tail)
	{
		x = x->forward[0];
		free(x);
	}

}


int main(int argc, char *argv[])
{

	int studentidflag, firstnameflag, lastnameflag, studentidaflag, studentidbflag, gpaaflag, numofcoursesflag, kflag, postcdeflag, deprtflag, hashentries;

	int x, counter, i, pos, flag, check, key, postcode, kappa, studid, studida, studidb, numcourses;

	float gpa, average;

	char temp[20], studentid[20], firstname[20], lastname[20], studentida[20], studentidb[20], gpaa[20], numofcourses[20], kk[20], postcde[20], deprt[20];

	srand(time(NULL));

	skiplist *list;
	list = (skiplist *)malloc(sizeof(struct skiplist));

	snode *tail;
	tail = (snode *)malloc(sizeof(struct snode));

	Init(list, tail);

	if (argc > 2)
	{

		hashentries = atoi(argv[4]);
		hashTable = (struct hash *)calloc(hashentries, sizeof(struct hash));
		char const* const fileName = argv[2];
		FILE* file = fopen(fileName, "r");
		char line[256];
		char command[2];

		while (fgets(line, sizeof(line), file) != NULL)
		{
			int linecounter = 0;
			while (line[linecounter] != '\n')
			{
				linecounter++;
			}

			if (linecounter == 0)
			{
				break;
			}
			else
			{


#pragma region ReadCommand

				for (i = 0; i <= 1; i++)
				{
					command[i] = '\0';
				}

				int j = 0;
				while (line[j] != ' ')
				{
					command[j] = line[j];
					j++;
				}


#pragma endregion

				switch (command[0])
				{

				case 'i':

#pragma region Insert


					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						studentid[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						lastname[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						gpaa[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						numofcourses[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						deprt[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						postcde[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					studentidflag = 0;
					firstnameflag = 0;
					lastnameflag = 0;
					gpaaflag = 0;
					numofcoursesflag = 0;
					deprtflag = 0;
					postcdeflag = 0;

					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && studentidflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									studentid[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								studentidflag = 1;
								x++;
								continue;
							}
							else if (counter == 3 && lastnameflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									lastname[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								lastnameflag = 1;
								x++;
								continue;
							}
							else if (counter == 4 && firstnameflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									firstname[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								firstnameflag = 1;
								x++;
								continue;
							}
							else if (counter == 5 && gpaaflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									gpaa[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								gpaaflag = 1;
								x++;
								continue;
							}
							else if (counter == 6 && numofcoursesflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									numofcourses[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								numofcoursesflag = 1;
								x++;
								continue;
							}
							else if (counter == 7 && deprtflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									deprt[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								deprtflag = 1;
								x++;
								continue;
							}

						}
						else
						{
							if (deprtflag == 1)
							{
								postcde[pos] = temp[pos];
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

					studid = atoi(studentid);
					gpa = atof(gpaa);
					numcourses = atoi(numofcourses);
					postcode = atoi(postcde);
					if (Insert(list, tail, studid, lastname, firstname, gpa, numcourses, deprt, postcode, hashentries) == 0)
					{
						Query(list, studid);
					}
					else {
						printf("There is a record with this student id! \n");
					}


#pragma endregion

					break;

				case 'q':

#pragma region Query

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					studid = atoi(temp);
					if (studid >= MAXVALUE)
					{
						break;
					}

					Query(list, studid);

#pragma endregion

					break;

				case 'm':

#pragma region Modify

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						studentid[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						gpaa[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						numofcourses[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					studentidflag = 0;
					gpaaflag = 0;
					numofcoursesflag = 0;

					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && studentidflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									studentid[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								studentidflag = 1;
								x++;
								continue;
							}
							else if (counter == 3 && gpaaflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									gpaa[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								gpaaflag = 1;
								numofcoursesflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (numofcoursesflag == 1)
							{
								numofcourses[pos] = temp[pos];
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

					studid = atoi(studentid);
					gpa = atof(gpaa);
					numcourses = atoi(numofcourses);
					if (studid >= MAXVALUE)
					{
						break;
					}

					Modify(list, studid, gpa, numcourses);

#pragma endregion

					break;

				case 'd':

#pragma region Delete

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					studid = atoi(temp);
					if (studid >= MAXVALUE)
					{
						break;
					}
					Delete(list, studid);

#pragma endregion

					break;

				case 'r':

#pragma region Raverage

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						studentida[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						studentidb[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					studentidaflag = 0;
					studentidbflag = 0;

					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && studentidaflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									studentida[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								studentidaflag = 1;
								studentidbflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (studentidbflag == 1)
							{
								studentidb[pos] = temp[pos];
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

					studida = atoi(studentida);
					studidb = atoi(studentidb);
					if (studida >= MAXVALUE || studidb >= MAXVALUE)
					{
						break;
					}
					average = Raverage(list, studida, studidb);
					printf("%.2f \n", average);
#pragma endregion

					break;

				case 'a':

#pragma region Average

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					check = 0;
					for (i = 0; i <= 19; i++)
					{
						if (temp[i] != '\0')
						{
							check++;
						}
					}

					if (check > 5)
					{
						break;
					}

					postcode = atoi(temp);
					key = Hash(postcode, hashentries);
					average = Average(key);
					printf("%.2f \n", average);

#pragma endregion

					break;

				case 't':

#pragma region Taverage

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						kk[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						postcde[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					kflag = 0;
					postcdeflag = 0;

					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && kflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									kk[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								kflag = 1;
								postcdeflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (postcdeflag == 1)
							{
								postcde[pos] = temp[pos];
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

					check = 0;
					for (i = 0; i <= 19; i++)
					{
						if (postcde[i] != '\0')
						{
							check++;
						}
					}

					if (check > 5)
					{
						break;
					}

					kappa = atoi(kk);
					postcode = atoi(postcde);
					key = Hash(postcode, hashentries);
					Taverage(kappa, key);


#pragma endregion

					break;

				case 'b':

#pragma region Bottom

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					kappa = atoi(temp);
					Bottom(list, tail, kappa);


#pragma endregion

					break;

				case 'c':

#pragma region CoursesToTake

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						postcde[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						deprt[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					postcdeflag = 0;
					deprtflag = 0;

					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && postcdeflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									postcde[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								postcdeflag = 1;
								deprtflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (deprtflag == 1)
							{
								deprt[pos] = temp[pos];
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

					check = 0;
					for (i = 0; i <= 19; i++)
					{
						if (postcde[i] != '\0')
						{
							check++;
						}
					}

					if (check > 5)
					{
						break;
					}

					postcode = atoi(postcde);
					key = Hash(postcode, hashentries);
					CoursesToTake(key, deprt);

#pragma endregion

					break;

				case 'f':

#pragma region Find

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (x < (linecounter - 1))
					{
						temp[pos] = line[x];

						if (line[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					gpa = atof(temp);
					Find(list, tail, gpa);


#pragma endregion

					break;

				case 'p':
					if (command[1] == 'e')
					{
						Percentiles(list, tail, hashentries);
						printf("\n");
					}
					else
					{

#pragma region Percentile

						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						pos = 0;
						counter = 0;
						flag = 0;
						x = 0;
						while (x < (linecounter - 1))
						{
							temp[pos] = line[x];

							if (line[x] == ' ')
							{
								counter++;

								if (counter == 1 && flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}
									pos = 0;
									flag = 1;
									x++;
									continue;
								}
								else if (counter == 2 && flag == 0)
								{
									break;
								}

							}
							else
							{
								x++;
								pos++;
								continue;
							}
						}

						check = 0;
						for (i = 0; i <= 19; i++)
						{
							if (temp[i] != '\0')
							{
								check++;
							}
						}

						if (check > 5)
						{
							break;
						}

						postcode = atoi(temp);
						key = Hash(postcode, hashentries);
						Percentile(list, tail, key);

#pragma endregion

					}

					break;

				case 'e':
					Exit(list, tail);
					return 0;
				}

			}

		}
	}
	else
	{

		hashentries = 10;
		hashTable = (struct hash *)calloc(hashentries, sizeof(struct hash));
#pragma region ReadCommand
		printf("Enter command: ");
		char buf[80];
		gets(buf);
		int final = 0;

		char command[2];
		for (i = 0; i <= 1; i++)
		{
			command[i] = '\0';
		}

		int j = 0;
		while (buf[j] != ' ')
		{
			command[j] = buf[j];

			if (command[j] == 'e' && j == 0)
			{
				final = 1;
				break;
			}
			else
			{
				j++;
			}
		}


#pragma endregion

		while (final != 1)
		{
			switch (command[0])
			{

			case 'i':

#pragma region Insert


				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					studentid[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					lastname[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					gpaa[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					numofcourses[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					deprt[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					postcde[i] = '\0';
				}

				pos = 0;
				counter = 0;
				x = 0;
				flag = 0;
				studentidflag = 0;
				firstnameflag = 0;
				lastnameflag = 0;
				gpaaflag = 0;
				numofcoursesflag = 0;
				deprtflag = 0;
				postcdeflag = 0;

				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && studentidflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								studentid[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							studentidflag = 1;
							x++;
							continue;
						}
						else if (counter == 3 && lastnameflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								lastname[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							lastnameflag = 1;
							x++;
							continue;
						}
						else if (counter == 4 && firstnameflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								firstname[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							firstnameflag = 1;
							x++;
							continue;
						}
						else if (counter == 5 && gpaaflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								gpaa[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							gpaaflag = 1;
							x++;
							continue;
						}
						else if (counter == 6 && numofcoursesflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								numofcourses[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							numofcoursesflag = 1;
							x++;
							continue;
						}
						else if (counter == 7 && deprtflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								deprt[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							deprtflag = 1;
							x++;
							continue;
						}

					}
					else
					{
						if (deprtflag == 1)
						{
							postcde[pos] = temp[pos];
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

				studid = atoi(studentid);
				gpa = atof(gpaa);
				numcourses = atoi(numofcourses);
				postcode = atoi(postcde);
				if (Insert(list, tail, studid, lastname, firstname, gpa, numcourses, deprt, postcode, hashentries) == 0)
				{
					Query(list, studid);
				}
				else {
					printf("There is a record with this student id! \n");
				}


#pragma endregion

				break;

			case 'q':

#pragma region Query

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				counter = 0;
				flag = 0;
				x = 0;
				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && flag == 0)
						{
							break;
						}

					}
					else
					{
						x++;
						pos++;
						continue;
					}
				}

				studid = atoi(temp);
				if (studid >= MAXVALUE)
				{
					break;
				}

				Query(list, studid);

#pragma endregion

				break;

			case 'm':

#pragma region Modify

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					studentid[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					gpaa[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					numofcourses[i] = '\0';
				}

				pos = 0;
				counter = 0;
				x = 0;
				flag = 0;
				studentidflag = 0;
				gpaaflag = 0;
				numofcoursesflag = 0;

				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && studentidflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								studentid[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							studentidflag = 1;
							x++;
							continue;
						}
						else if (counter == 3 && gpaaflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								gpaa[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							gpaaflag = 1;
							numofcoursesflag = 1;
							x++;
							continue;
						}
					}
					else
					{
						if (numofcoursesflag == 1)
						{
							numofcourses[pos] = temp[pos];
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

				studid = atoi(studentid);
				gpa = atof(gpaa);
				numcourses = atoi(numofcourses);
				if (studid >= MAXVALUE)
				{
					break;
				}

				Modify(list, studid, gpa, numcourses);

#pragma endregion

				break;

			case 'd':

#pragma region Delete

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				counter = 0;
				flag = 0;
				x = 0;
				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && flag == 0)
						{
							break;
						}

					}
					else
					{
						x++;
						pos++;
						continue;
					}
				}

				studid = atoi(temp);
				if (studid >= MAXVALUE)
				{
					break;
				}
				Delete(list, studid);

#pragma endregion

				break;

			case 'r':

#pragma region Raverage

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					studentida[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					studentidb[i] = '\0';
				}

				pos = 0;
				counter = 0;
				x = 0;
				flag = 0;
				studentidaflag = 0;
				studentidbflag = 0;

				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && studentidaflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								studentida[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							studentidaflag = 1;
							studentidbflag = 1;
							x++;
							continue;
						}
					}
					else
					{
						if (studentidbflag == 1)
						{
							studentidb[pos] = temp[pos];
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

				studida = atoi(studentida);
				studidb = atoi(studentidb);
				if (studida >= MAXVALUE || studidb >= MAXVALUE)
				{
					break;
				}
				average = Raverage(list, studida, studidb);
				printf("%.2f \n", average);
#pragma endregion

				break;

			case 'a':

#pragma region Average

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				counter = 0;
				flag = 0;
				x = 0;
				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && flag == 0)
						{
							break;
						}

					}
					else
					{
						x++;
						pos++;
						continue;
					}
				}

				check = 0;
				for (i = 0; i <= 19; i++)
				{
					if (temp[i] != '\0')
					{
						check++;
					}
				}

				if (check > 5)
				{
					break;
				}

				postcode = atoi(temp);
				key = Hash(postcode, hashentries);
				average = Average(key);
				printf("%.2f \n", average);

#pragma endregion

				break;

			case 't':

#pragma region Taverage

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					kk[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					postcde[i] = '\0';
				}

				pos = 0;
				counter = 0;
				x = 0;
				flag = 0;
				kflag = 0;
				postcdeflag = 0;

				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && kflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								kk[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							kflag = 1;
							postcdeflag = 1;
							x++;
							continue;
						}
					}
					else
					{
						if (postcdeflag == 1)
						{
							postcde[pos] = temp[pos];
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

				check = 0;
				for (i = 0; i <= 19; i++)
				{
					if (postcde[i] != '\0')
					{
						check++;
					}
				}

				if (check > 5)
				{
					break;
				}

				kappa = atoi(kk);
				postcode = atoi(postcde);
				key = Hash(postcode, hashentries);
				Taverage(kappa, key);


#pragma endregion

				break;

			case 'b':

#pragma region Bottom

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				counter = 0;
				flag = 0;
				x = 0;
				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && flag == 0)
						{
							break;
						}

					}
					else
					{
						x++;
						pos++;
						continue;
					}
				}

				kappa = atoi(temp);
				Bottom(list, tail, kappa);


#pragma endregion

				break;

			case 'c':

#pragma region CoursesToTake

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					postcde[i] = '\0';
				}

				for (i = 0; i <= 19; i++)
				{
					deprt[i] = '\0';
				}

				pos = 0;
				counter = 0;
				x = 0;
				flag = 0;
				postcdeflag = 0;
				deprtflag = 0;

				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && postcdeflag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								postcde[i] = temp[i];
							}

							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}

							pos = 0;
							postcdeflag = 1;
							deprtflag = 1;
							x++;
							continue;
						}
					}
					else
					{
						if (deprtflag == 1)
						{
							deprt[pos] = temp[pos];
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

				check = 0;
				for (i = 0; i <= 19; i++)
				{
					if (postcde[i] != '\0')
					{
						check++;
					}
				}

				if (check > 6)
				{
					break;
				}

				postcode = atoi(postcde);
				key = Hash(postcode, hashentries);
				CoursesToTake(key, deprt);

#pragma endregion

				break;

			case 'f':

#pragma region Find

				for (i = 0; i <= 19; i++)
				{
					temp[i] = '\0';
				}

				pos = 0;
				counter = 0;
				flag = 0;
				x = 0;
				while (buf[x] != '\0')
				{
					temp[pos] = buf[x];

					if (buf[x] == ' ')
					{
						counter++;

						if (counter == 1 && flag == 0)
						{
							for (i = 0; i <= 19; i++)
							{
								temp[i] = '\0';
							}
							pos = 0;
							flag = 1;
							x++;
							continue;
						}
						else if (counter == 2 && flag == 0)
						{
							break;
						}

					}
					else
					{
						x++;
						pos++;
						continue;
					}
				}

				gpa = atof(temp);
				Find(list, tail, gpa);


#pragma endregion

				break;

			case 'p':
				if (command[1] == 'e')
				{
					Percentiles(list, tail, hashentries);
					printf("\n");
				}
				else
				{

#pragma region Percentile

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					pos = 0;
					counter = 0;
					flag = 0;
					x = 0;
					while (buf[x] != '\0')
					{
						temp[pos] = buf[x];

						if (buf[x] == ' ')
						{
							counter++;

							if (counter == 1 && flag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}
								pos = 0;
								flag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && flag == 0)
							{
								break;
							}

						}
						else
						{
							x++;
							pos++;
							continue;
						}
					}

					check = 0;
					for (i = 0; i <= 19; i++)
					{
						if (temp[i] != '\0')
						{
							check++;
						}
					}

					if (check > 5)
					{
						break;
					}

					postcode = atoi(temp);
					key = Hash(postcode, hashentries);
					Percentile(list, tail, key);

#pragma endregion

				}

				break;

			case 'e':
				Exit(list, tail);
				return 0;
			}


#pragma region ReadNewCommand
			printf("\n");
			printf("Enter command: ");
			gets(buf);

			for (i = 0; i <= 1; i++)
			{
				command[i] = '\0';
			}

			int k = 0;
			while (buf[k] != ' ')
			{
				command[k] = buf[k];

				if (command[k] == 'e' && k == 0)
				{
					final = 1;
					break;
				}
				else
				{
					k++;
				}
			}
		}

#pragma endregion


	}

}