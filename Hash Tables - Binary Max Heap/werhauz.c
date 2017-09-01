#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "werhauz.h"


float CalculateTotalHeapCost(maxHeap *hp)
{
	int i;
	float cost = 0;
	for (i = 0; i < hp->size; i++)
	{
		cost = cost + hp->elem[i].data;
	}




	return cost;
}

heapnode* postorderTraversal(maxHeap *hp, int i, float temp)
{
	if (hp->elem[i].data > temp)
	{
		temp = hp->elem[i].data;
	}

	if (LCHILD(i) < hp->size) {
		postorderTraversal(hp, LCHILD(i), temp);
	}
	if (RCHILD(i) < hp->size) {
		postorderTraversal(hp, RCHILD(i), temp);
	}

	return hp->elem;
}

heapnode* FindNextMax(maxHeap *hp, int i, float temp,float prevmax)
{

	if (hp->elem[i].data > temp && hp->elem[i].data < prevmax)
	{
		temp = hp->elem[i].data;
	}

	if (LCHILD(i) < hp->size) {
		FindNextMax(hp, LCHILD(i), temp,prevmax);
	}
	if (RCHILD(i) < hp->size) {
		FindNextMax(hp, RCHILD(i), temp,prevmax);
	}

	return hp->elem;

}

void Top(maxHeap *hp,int percentage)
{
	float totalheapcost;
	totalheapcost = CalculateTotalHeapCost(hp);
	printf("Total heap cost is: %.1f\n", totalheapcost);

	float total = ((float)percentage / 100) * totalheapcost;
	printf("%d%% of %.1f is: %.1f\n",percentage,totalheapcost,total);
	printf("\n");

	heapnode *tempor;
	tempor = postorderTraversal(hp, 0, 0);

	if (tempor->data >= total)
	{

		printf("[%s]\n", tempor->number);
	}
	else
	{
		float counter;
		counter = tempor->data;

		while (counter <= total)
		{
			float prevmax = tempor->data;

			tempor = FindNextMax(hp, 0, 0, prevmax);

			printf("[%s]\n", tempor->number);


			counter = counter + tempor->data;
			if (counter > total)
			{
				tempor = FindNextMax(hp, 0, 0, tempor->data);
				counter = counter + tempor->data;
			}

		}


	}



}

void insertNode(maxHeap *hp, char* origin_number, float data)
{

	if (hp->size)
	{
		hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(heapnode));
	}
	else {
		hp->elem = malloc(sizeof(heapnode));
	}

	heapnode nd;
	nd.data = data;
	nd.number = strdup(origin_number);

	int flag = 0;

	int i = hp->size;

	while (i && data > hp->elem[PARENT(i)].data)
	{
		hp->elem[i] = hp->elem[PARENT(i)];
		i = PARENT(i);

	}

	if (hp->size != 0)
	{
		if (strcmp(origin_number, hp->elem[PARENT(i)].number) == 0)
		{
			flag = 1;

		}
	}

	if (flag == 1)
	{
		hp->elem[PARENT(i)].data = hp->elem[PARENT(i)].data + data;
	}
	else
	{
		hp->elem[i] = nd;
		hp->size++;

	}

}

void InsertIntoMaxHeap(maxHeap *hp, char* origin_number, int duration_, int type_, int tarrif_, int fault_cond_, int chargestablelines)
{
	int i;

	if (fault_cond_ >= 200 & fault_cond_ <= 299)
	{

		float tempcost;
		int flag = 0;
		for (i = 0; i < chargestablelines; i++)
		{
			if (ChargesTable[i].type == type_ && ChargesTable[i].tarrif == tarrif_)
			{
				if (type_ == 0 && tarrif_ == 0)
				{
					flag = 1;
				}

				tempcost = ChargesTable[i].cost;
				break;

			}

		}

		float heapcost;
		if (flag == 1)
		{
			heapcost = tempcost;
		}
		else
		{
			heapcost = duration_ * tempcost;

		}

		insertNode(hp, origin_number, heapcost);

	}



}

void Indist(char *caller1, char *caller2, int hashentries, int hashentries2, int bucketsize)
{

	int i, j, k, found;
	int jobdone = 0;

	//1. Find the subscribers that caller1 had contact and the subscribers that caller2 had contact.

#pragma region FindSubs

	int cntr = 0;
	int cntr2 = 0;

	c1dictionary = (struct namesdict *)calloc(1, sizeof(struct namesdict));
	c2dictionary = (struct namesdict *)calloc(1, sizeof(struct namesdict));

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, caller1) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								if (cntr == 0)
								{
									c1dictionary = (struct namesdict *)calloc(1, sizeof(struct namesdict));
								}
								else
								{
									c1dictionary = realloc(c1dictionary, sizeof(struct namesdict) * 1);
								}

								c1dictionary[cntr].name = strdup(ptr->List[k].dest_number);

								cntr++;

							}

						}

						ptr = ptr->next;

					}

				}
				else if (strcmp(temp->basketnode[j].number, caller2) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								if (cntr2 == 0)
								{
									c2dictionary = (struct namesdict *)calloc(1, sizeof(struct namesdict));
								}
								else
								{
									c2dictionary = realloc(c2dictionary, sizeof(struct namesdict) * 1);
								}

								c2dictionary[cntr].name = strdup(ptr->List[k].dest_number);

								cntr2++;

							}

						}

						ptr = ptr->next;

					}

				}

			}

			temp = temp->next;
		}

	}


#pragma endregion

	for (i = 0; i < cntr; i++)
	{
		printf("Look: %s\n", c1dictionary[i].name);
	}

	for (i = 0; i < cntr2; i++)
	{
		printf("See: %s\n", c2dictionary[i].name);
	}

}

void Delete(char *cdrid, char *origin_number, int hashentries, int bucketsize)
{
	int i, j, k, found;
	int jobdone = 0;

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (strcmp(ptr->List[k].cdrid, cdrid) == 0)
							{
								free(ptr->List[k].cdrid);
								free(ptr->List[k].date);
								free(ptr->List[k].dest_number);
								free(ptr->List[k].init_time);
								break;
							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}


}

int Hash(const char* word, int hashentries)
{
	unsigned int hash = 0;
	int i;
	for (i = 0; word[i] != '\0'; i++)
	{
		hash = 31 * hash + word[i];
	}
	return hash % hashentries;
}

void Insert(char* cdrid, char* origin_number, char* dest_number, char* date, char* init_time, int duration, int type, int tarrif, int fault_cond, int bucketsize, int hashentries)
{
	int j, k;
	int hashIndex = Hash(origin_number, hashentries);

	if (hashTable[hashIndex].head == NULL)
	{

		hashTable[hashIndex].count = hashIndex;

		hashTable[hashIndex].head = malloc(1 * sizeof(struct node));

		hashTable[hashIndex].head->next = NULL;

		hashTable[hashIndex].head->basketnode = malloc(bucketsize * sizeof(struct snode));

		hashTable[hashIndex].head->basketnode[0].number = malloc(sizeof(origin_number + 1));

		hashTable[hashIndex].head->basketnode[0].listinfo = malloc(sizeof(struct listnode));

		hashTable[hashIndex].head->basketnode[0].listinfo->next = NULL;

		//Fill Data

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].dest_number = malloc(sizeof(dest_number + 1));

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].cdrid = malloc(sizeof(cdrid + 1));

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].date = malloc(sizeof(date + 1));

		strcpy(hashTable[hashIndex].head->basketnode[0].listinfo->List[0].cdrid, cdrid);

		strcpy(hashTable[hashIndex].head->basketnode[0].listinfo->List[0].dest_number, dest_number);

		strcpy(hashTable[hashIndex].head->basketnode[0].listinfo->List[0].date, date);

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].init_time = malloc(sizeof(init_time + 1));

		strcpy(hashTable[hashIndex].head->basketnode[0].listinfo->List[0].init_time, init_time);

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].duration = duration;

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].type = type;

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].tarrif = tarrif;

		hashTable[hashIndex].head->basketnode[0].listinfo->List[0].fault_cond = fault_cond;

		strcpy(hashTable[hashIndex].head->basketnode[0].number, origin_number);

	}
	else
	{

		node *checkptr;
		int found = 0;
		checkptr = hashTable[hashIndex].head;
		while (checkptr != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{
				if (checkptr->basketnode[j].number != NULL)
				{
					if (strcmp(checkptr->basketnode[j].number, origin_number) == 0)
					{
						found = 1;
						break;
					}
				}

			}

			if (found == 1)
			{
				break;
			}
			else
			{
				checkptr = checkptr->next;
			}

		}

		if (found == 0)
		{
			node *temp;
			temp = hashTable[hashIndex].head;
			int inserted = 0;
			while (temp != NULL)
			{

				for (j = 0; j < bucketsize / sizeof(struct snode); j++)
				{

					if (temp->basketnode[j].number == NULL)
					{

						temp->basketnode[j].number = malloc(sizeof(origin_number + 1));

						temp->basketnode[j].listinfo = malloc(sizeof(struct listnode));

						temp->basketnode[j].listinfo->List[0].dest_number = malloc(sizeof(dest_number + 1));

						temp->basketnode[j].listinfo->List[0].cdrid = malloc(sizeof(cdrid + 1));

						temp->basketnode[j].listinfo->List[0].date = malloc(sizeof(date + 1));

						temp->basketnode[j].listinfo->List[0].init_time = malloc(sizeof(init_time + 1));

						strcpy(temp->basketnode[j].listinfo->List[0].cdrid, cdrid);

						strcpy(temp->basketnode[j].listinfo->List[0].dest_number, dest_number);

						strcpy(temp->basketnode[j].listinfo->List[0].date, date);

						strcpy(temp->basketnode[j].listinfo->List[0].init_time, init_time);

						temp->basketnode[j].listinfo->List[0].duration = duration;

						temp->basketnode[j].listinfo->List[0].type = type;

						temp->basketnode[j].listinfo->List[0].tarrif = tarrif;

						temp->basketnode[j].listinfo->List[0].fault_cond = fault_cond;

						strcpy(temp->basketnode[j].number, origin_number);

						inserted = 1;
						break;
					}

				}

				if (inserted == 1)
				{
					break;

				}

				temp = temp->next;
			}

		}
		else
		{
			node *temp;
			temp = hashTable[hashIndex].head;
			int inserted = 0;
			int insertednode = 0;
			while (temp != NULL)
			{

				for (j = 0; j < bucketsize / sizeof(struct snode); j++)
				{

					if (strcmp(temp->basketnode[j].number, origin_number) == 0)
					{
						listnode *ptr;

						ptr = temp->basketnode[j].listinfo;

						insertednode = 0;

						while (ptr != NULL)
						{

							for (k = 0; k < LISTSIZE; k++)
							{

								if (ptr->List[k].cdrid == NULL)
								{

									ptr->List[k].dest_number = malloc(sizeof(dest_number + 1));

									ptr->List[k].cdrid = malloc(sizeof(cdrid + 1));

									ptr->List[k].date = malloc(sizeof(date + 1));

									ptr->List[k].init_time = malloc(sizeof(init_time + 1));

									strcpy(ptr->List[k].cdrid, cdrid);

									strcpy(ptr->List[k].dest_number, dest_number);

									strcpy(ptr->List[k].date, date);

									strcpy(ptr->List[k].init_time, init_time);

									ptr->List[k].duration = duration;

									ptr->List[k].type = type;

									ptr->List[k].tarrif = tarrif;

									ptr->List[k].fault_cond = fault_cond;

									insertednode = 1;

									if (k == LISTSIZE - 1)
									{
										listnode *temp;

										temp = malloc(sizeof(struct listnode));

										temp->next = NULL;

										ptr->next = temp;

									}

									break;

								}

							}

							if (insertednode == 1)
							{
								break;
							}

							ptr = ptr->next;

						}

					}

					if (insertednode == 1)
					{
						break;
					}

				}

				if (insertednode == 1)
				{
					break;
				}

				temp = temp->next;
			}

		}

	}

}

void Insert2(char* cdrid, char* origin_number, char* dest_number, char* date, char* init_time, int duration, int type, int tarrif, int fault_cond, int bucketsize, int hashentries)
{
	int j, k;
	int hashIndex = Hash(dest_number, hashentries);

	if (hashTable2[hashIndex].head == NULL)
	{

		hashTable2[hashIndex].count = hashIndex;

		hashTable2[hashIndex].head = malloc(1 * sizeof(struct node));

		hashTable2[hashIndex].head->next = NULL;

		hashTable2[hashIndex].head->basketnode = malloc(bucketsize * sizeof(struct snode));

		hashTable2[hashIndex].head->basketnode[0].number = malloc(sizeof(dest_number + 1));

		hashTable2[hashIndex].head->basketnode[0].listinfo = malloc(sizeof(struct listnode));

		hashTable2[hashIndex].head->basketnode[0].listinfo->next = NULL;

		//Fill Data

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].origin_number = malloc(sizeof(origin_number + 1));

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].cdrid = malloc(sizeof(cdrid + 1));

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].date = malloc(sizeof(date + 1));

		strcpy(hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].cdrid, cdrid);

		strcpy(hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].origin_number, origin_number);

		strcpy(hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].date, date);

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].init_time = malloc(sizeof(init_time + 1));

		strcpy(hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].init_time, init_time);

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].duration = duration;

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].type = type;

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].tarrif = tarrif;

		hashTable2[hashIndex].head->basketnode[0].listinfo->List[0].fault_cond = fault_cond;

		strcpy(hashTable2[hashIndex].head->basketnode[0].number, dest_number);

	}
	else
	{

		node *checkptr;
		int found = 0;
		checkptr = hashTable2[hashIndex].head;
		while (checkptr != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{
				if (checkptr->basketnode[j].number != NULL)
				{
					if (strcmp(checkptr->basketnode[j].number, dest_number) == 0)
					{
						found = 1;
						break;
					}
				}

			}

			if (found == 1)
			{
				break;
			}
			else
			{
				checkptr = checkptr->next;
			}

		}

		if (found == 0)
		{
			node *temp;
			temp = hashTable2[hashIndex].head;
			int inserted = 0;
			while (temp != NULL)
			{

				for (j = 0; j < bucketsize / sizeof(struct snode); j++)
				{

					if (temp->basketnode[j].number == NULL)
					{

						temp->basketnode[j].number = malloc(sizeof(dest_number + 1));

						temp->basketnode[j].listinfo = malloc(sizeof(struct listnode));

						temp->basketnode[j].listinfo->List[0].origin_number = malloc(sizeof(origin_number + 1));

						temp->basketnode[j].listinfo->List[0].cdrid = malloc(sizeof(cdrid + 1));

						temp->basketnode[j].listinfo->List[0].date = malloc(sizeof(date + 1));

						temp->basketnode[j].listinfo->List[0].init_time = malloc(sizeof(init_time + 1));

						strcpy(temp->basketnode[j].listinfo->List[0].cdrid, cdrid);

						strcpy(temp->basketnode[j].listinfo->List[0].origin_number, origin_number);

						strcpy(temp->basketnode[j].listinfo->List[0].date, date);

						strcpy(temp->basketnode[j].listinfo->List[0].init_time, init_time);

						temp->basketnode[j].listinfo->List[0].duration = duration;

						temp->basketnode[j].listinfo->List[0].type = type;

						temp->basketnode[j].listinfo->List[0].tarrif = tarrif;

						temp->basketnode[j].listinfo->List[0].fault_cond = fault_cond;

						strcpy(temp->basketnode[j].number, dest_number);

						inserted = 1;
						break;
					}

				}

				if (inserted == 1)
				{
					break;

				}

				temp = temp->next;
			}

		}
		else
		{
			node *temp;
			temp = hashTable2[hashIndex].head;
			int inserted = 0;
			int insertednode = 0;
			while (temp != NULL)
			{

				for (j = 0; j < bucketsize / sizeof(struct snode); j++)
				{

					if (strcmp(temp->basketnode[j].number, dest_number) == 0)
					{
						listnode *ptr;

						ptr = temp->basketnode[j].listinfo;

						insertednode = 0;

						while (ptr != NULL)
						{

							for (k = 0; k < LISTSIZE; k++)
							{

								if (ptr->List[k].cdrid == NULL)
								{

									ptr->List[k].origin_number = malloc(sizeof(origin_number + 1));

									ptr->List[k].cdrid = malloc(sizeof(cdrid + 1));

									ptr->List[k].date = malloc(sizeof(date + 1));

									ptr->List[k].init_time = malloc(sizeof(init_time + 1));

									strcpy(ptr->List[k].cdrid, cdrid);

									strcpy(ptr->List[k].origin_number, origin_number);

									strcpy(ptr->List[k].date, date);

									strcpy(ptr->List[k].init_time, init_time);

									ptr->List[k].duration = duration;

									ptr->List[k].type = type;

									ptr->List[k].tarrif = tarrif;

									ptr->List[k].fault_cond = fault_cond;

									insertednode = 1;

									if (k == LISTSIZE - 1)
									{
										listnode *temp;

										temp = malloc(sizeof(struct listnode));

										temp->next = NULL;

										ptr->next = temp;

									}

									break;

								}

							}

							if (insertednode == 1)
							{
								break;
							}

							ptr = ptr->next;

						}

					}

					if (insertednode == 1)
					{
						break;
					}

				}

				if (insertednode == 1)
				{
					break;
				}

				temp = temp->next;
			}

		}

	}

}

void PrintHashtable1(int hashentries, int bucketsize)
{
	int i, j, k, tab;

	printf("[Hashtable 1]\n");
	printf("\n");

	for (i = 0; i < hashentries; i++)
	{

		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{

			printf("Node %d: \n", i);
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}
				printf("\t");
				printf("[%s]\n", temp->basketnode[j].number);

				listnode *ptr;

				ptr = temp->basketnode[j].listinfo;

				int cnt = 1;
				while (ptr != NULL)
				{
					cnt++;
					for (k = 0; k < LISTSIZE; k++)
					{
						if (ptr->List[k].cdrid != NULL)
						{
							for (tab = 0; tab <= cnt; tab++)
							{
								printf("\t");
							}

							printf("%s|", ptr->List[k].cdrid);
							printf("%s|", ptr->List[k].dest_number);
							printf("%s|", ptr->List[k].date);
							printf("%s|", ptr->List[k].init_time);
							printf("%d|", ptr->List[k].duration);
							printf("%d|", ptr->List[k].type);
							printf("%d|", ptr->List[k].tarrif);
							printf("%d\n", ptr->List[k].fault_cond);
						}

					}

					ptr = ptr->next;

				}

			}

			temp = temp->next;
		}

	}

}

void PrintHashtable2(int hashentries, int bucketsize)
{
	int i, j, k, tab;

	printf("[Hashtable 2]\n");
	printf("\n");

	for (i = 0; i < hashentries; i++)
	{

		node *temp;
		temp = hashTable2[i].head;

		while (temp != NULL)
		{

			printf("Node %d: \n", i);
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}
				printf("\t");
				printf("[%s]\n", temp->basketnode[j].number);

				listnode *ptr;

				ptr = temp->basketnode[j].listinfo;

				int cnt = 1;
				while (ptr != NULL)
				{
					cnt++;
					for (k = 0; k < LISTSIZE; k++)
					{
						if (ptr->List[k].cdrid != NULL)
						{
							for (tab = 0; tab <= cnt; tab++)
							{
								printf("\t");
							}

							printf("%s|", ptr->List[k].cdrid);
							printf("%s|", ptr->List[k].origin_number);
							printf("%s|", ptr->List[k].date);
							printf("%s|", ptr->List[k].init_time);
							printf("%d|", ptr->List[k].duration);
							printf("%d|", ptr->List[k].type);
							printf("%d|", ptr->List[k].tarrif);
							printf("%d\n", ptr->List[k].fault_cond);
						}

					}

					ptr = ptr->next;

				}

			}

			temp = temp->next;
		}

	}

}

void TopDest(char *origin_number, int hashentries, int bucketsize)
{
	int i, j, k, found;
	int jobdone = 0;

	for (i = 0; i < 50; i++)
	{
		array[i].code = malloc(3 * sizeof(char));
		array[i].counter = 0;
	}

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{
								char code[3];
								strncpy(code, ptr->List[k].dest_number, 3);

								found = 0;
								int posit = 0;
								for (i = 0; i < 50; i++)
								{
									if (strcmp(array[i].code, code) == 0)
									{
										found = 1;
										posit = i;
										break;
									}
								}

								if (found == 1)
								{
									array[posit].counter++;
								}
								else
								{
									for (i = 0; i < 50; i++)
									{
										if (array[i].counter == 0)
										{
											strcpy(array[i].code, code);
											array[i].counter++;
											break;
										}
									}

								}

							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}

	//Find max
	int maximum = array[0].counter;
	int position = 0;
	for (i = 0; i < 50; i++)
	{
		if (array[i].counter > maximum)
		{
			maximum = array[i].counter;
			position = i;
		}
	}

	printf("Country code: %s, Calls made: %d \n", array[position].code, maximum);
	printf("\n");

}

void Find(char *origin_number, char* time1, char* time2, int hashentries, int bucketsize)
{

	int i, j, k, found;
	int jobdone = 0;

	char *hours1;
	char *hours2;
	char *mins1;
	char *mins2;
	char *search = ":";

	hours1 = strtok(time1, search);
	mins1 = strtok(NULL, search);
	hours2 = strtok(time2, search);
	mins2 = strtok(NULL, search);

	int h1 = atoi(hours1);
	int h2 = atoi(hours2);
	int min1 = atoi(mins1);
	int min2 = atoi(mins2);

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								char *hours3;
								char *mins3;
								hours3 = strtok(ptr->List[k].init_time, search);
								mins3 = strtok(NULL, search);

								int hour = atoi(hours3);
								int min = atoi(mins3);

								if ((h2 - hour) > 0 && (hour - h1) > 0)
								{
									printf("%s|", ptr->List[k].cdrid);
									printf("%s|", ptr->List[k].dest_number);
									printf("%s|", ptr->List[k].date);
									printf("%s|", ptr->List[k].init_time);
									printf("%d|", ptr->List[k].duration);
									printf("%d|", ptr->List[k].type);
									printf("%d|", ptr->List[k].tarrif);
									printf("%d\n", ptr->List[k].fault_cond);
								}
								else if ((h2 - hour) == 0)
								{
									if ((min2 - min) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].dest_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);
									}
								}
								else if ((hour - h1) == 0)
								{

									if ((min - min1) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].dest_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);

									}
								}

							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}


}

void Find2(char *origin_number, char* time1, char* time2, int hashentries, int bucketsize)
{

	int i, j, k, found;
	int jobdone = 0;

	char *hours1;
	char *hours2;
	char *mins1;
	char *mins2;
	char *search = ":";

	hours1 = strtok(time1, search);
	mins1 = strtok(NULL, search);
	hours2 = strtok(time2, search);
	mins2 = strtok(NULL, search);

	int h1 = atoi(hours1);
	int h2 = atoi(hours2);
	int min1 = atoi(mins1);
	int min2 = atoi(mins2);

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable2[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								char *hours3;
								char *mins3;
								hours3 = strtok(ptr->List[k].init_time, search);
								mins3 = strtok(NULL, search);

								int hour = atoi(hours3);
								int min = atoi(mins3);

								if ((h2 - hour) > 0 && (hour - h1) > 0)
								{
									printf("%s|", ptr->List[k].cdrid);
									printf("%s|", ptr->List[k].origin_number);
									printf("%s|", ptr->List[k].date);
									printf("%s|", ptr->List[k].init_time);
									printf("%d|", ptr->List[k].duration);
									printf("%d|", ptr->List[k].type);
									printf("%d|", ptr->List[k].tarrif);
									printf("%d\n", ptr->List[k].fault_cond);
								}
								else if ((h2 - hour) == 0)
								{
									if ((min2 - min) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].origin_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);
									}
								}
								else if ((hour - h1) == 0)
								{

									if ((min - min1) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].origin_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);

									}
								}

							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}


}

void FindWithYear(char* origin_number, char* time1, char* time2, char* year1, char* year2, int hashentries, int bucketsize)
{
	int i, j, k, found;
	int jobdone = 0;

	char *hours1;
	char *hours2;
	char *mins1;
	char *mins2;
	char *search = ":";

	hours1 = strtok(time1, search);
	mins1 = strtok(NULL, search);
	hours2 = strtok(time2, search);
	mins2 = strtok(NULL, search);

	int h1 = atoi(hours1);
	int h2 = atoi(hours2);
	int min1 = atoi(mins1);
	int min2 = atoi(mins2);

	char day1[3];
	char month1[3];
	char y1[5];

	memcpy(day1, &year1[0], 2);
	day1[2] = '\0';

	memcpy(month1, &year1[2], 2);
	month1[2] = '\0';

	memcpy(y1, &year1[4], 4);
	y1[4] = '\0';

	char day2[3];
	char month2[3];
	char y2[5];

	memcpy(day2, &year2[0], 2);
	day2[2] = '\0';

	memcpy(month2, &year2[2], 2);
	month2[2] = '\0';

	memcpy(y2, &year2[4], 4);
	y2[4] = '\0';

	int d1 = atoi(day1);
	int m1 = atoi(month1);
	int yy1 = atoi(y1);

	int d2 = atoi(day2);
	int m2 = atoi(month2);
	int yy2 = atoi(y2);

	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								char *hours3;
								hours3 = strtok(ptr->List[k].init_time, search);

								char mins3[3];
								memcpy(mins3, &ptr->List[k].init_time[3], 2);
								mins3[2] = '\0';

								int hour = atoi(hours3);
								int min = atoi(mins3);

								char day3[3];
								char month3[3];
								char y3[5];

								memcpy(day3, &ptr->List[k].date[0], 2);
								day3[2] = '\0';

								memcpy(month3, &ptr->List[k].date[2], 2);
								month3[2] = '\0';

								memcpy(y3, &ptr->List[k].date[4], 4);
								y3[4] = '\0';

								int d3 = atoi(day3);
								int m3 = atoi(month3);
								int yy3 = atoi(y3);


								if ((yy2 - yy3) > 0 && (yy3 - yy1) > 0)
								{
									if ((h2 - hour) > 0 && (hour - h1) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].dest_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);
									}
									else if ((h2 - hour) == 0)
									{
										if ((min2 - min) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].dest_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
									}
									else if ((hour - h1) == 0)
									{

										if ((min - min1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].dest_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);

										}
									}

								}
								else if ((yy2 - yy3) == 0)
								{

									if ((m2 - m3) > 0 && (m3 - m1) > 0)
									{

										if ((h2 - hour) > 0 && (hour - h1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].dest_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
										else if ((h2 - hour) == 0)
										{
											if ((min2 - min) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
										}
										else if ((hour - h1) == 0)
										{

											if ((min - min1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);

											}
										}

									}
									else if ((m2 - m3) == 0)
									{
										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) > 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}

									}
									else if ((m3 - m1) == 0)
									{

										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) == 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}


									}

								}
								else if ((yy3 - yy1) == 0)
								{

									if ((m2 - m3) > 0 && (m3 - m1) > 0)
									{

										if ((h2 - hour) > 0 && (hour - h1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].dest_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
										else if ((h2 - hour) == 0)
										{
											if ((min2 - min) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
										}
										else if ((hour - h1) == 0)
										{

											if ((min - min1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);

											}
										}

									}
									else if ((m2 - m3) == 0)
									{

										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) > 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}

									}
									else if ((m3 - m1) == 0)
									{

										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) == 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}


									}

								}

							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}



}

void FindWithYear2(char* origin_number, char* time1, char* time2, char* year1, char* year2, int hashentries, int bucketsize)
{
	int i, j, k, found;
	int jobdone = 0;

	char *hours1;
	char *hours2;
	char *mins1;
	char *mins2;
	char *search = ":";

	hours1 = strtok(time1, search);
	mins1 = strtok(NULL, search);
	hours2 = strtok(time2, search);
	mins2 = strtok(NULL, search);

	int h1 = atoi(hours1);
	int h2 = atoi(hours2);
	int min1 = atoi(mins1);
	int min2 = atoi(mins2);

	char day1[3];
	char month1[3];
	char y1[5];

	memcpy(day1, &year1[0], 2);
	day1[2] = '\0';

	memcpy(month1, &year1[2], 2);
	month1[2] = '\0';

	memcpy(y1, &year1[4], 4);
	y1[4] = '\0';

	char day2[3];
	char month2[3];
	char y2[5];

	memcpy(day2, &year2[0], 2);
	day2[2] = '\0';

	memcpy(month2, &year2[2], 2);
	month2[2] = '\0';

	memcpy(y2, &year2[4], 4);
	y2[4] = '\0';

	int d1 = atoi(day1);
	int m1 = atoi(month1);
	int yy1 = atoi(y1);

	int d2 = atoi(day2);
	int m2 = atoi(month2);
	int yy2 = atoi(y2);


	for (i = 0; i < hashentries; i++)
	{
		node *temp;
		temp = hashTable2[i].head;

		while (temp != NULL)
		{
			for (j = 0; j < bucketsize / sizeof(struct snode); j++)
			{

				if (temp->basketnode[j].number == NULL)
				{
					continue;
				}

				if (strcmp(temp->basketnode[j].number, origin_number) == 0)
				{

					listnode *ptr;
					ptr = temp->basketnode[j].listinfo;

					while (ptr != NULL)
					{

						for (k = 0; k < LISTSIZE; k++)
						{
							if (ptr->List[k].cdrid != NULL)
							{

								char *hours3;
								hours3 = strtok(ptr->List[k].init_time, search);

								char mins3[3];
								memcpy(mins3, &ptr->List[k].init_time[3], 2);
								mins3[2] = '\0';

								int hour = atoi(hours3);
								int min = atoi(mins3);

								char day3[3];
								char month3[3];
								char y3[5];

								memcpy(day3, &ptr->List[k].date[0], 2);
								day3[2] = '\0';

								memcpy(month3, &ptr->List[k].date[2], 2);
								month3[2] = '\0';

								memcpy(y3, &ptr->List[k].date[4], 4);
								y3[4] = '\0';

								int d3 = atoi(day3);
								int m3 = atoi(month3);
								int yy3 = atoi(y3);


								if ((yy2 - yy3) > 0 && (yy3 - yy1) > 0)
								{
									if ((h2 - hour) > 0 && (hour - h1) > 0)
									{
										printf("%s|", ptr->List[k].cdrid);
										printf("%s|", ptr->List[k].origin_number);
										printf("%s|", ptr->List[k].date);
										printf("%s|", ptr->List[k].init_time);
										printf("%d|", ptr->List[k].duration);
										printf("%d|", ptr->List[k].type);
										printf("%d|", ptr->List[k].tarrif);
										printf("%d\n", ptr->List[k].fault_cond);
									}
									else if ((h2 - hour) == 0)
									{
										if ((min2 - min) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].origin_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
									}
									else if ((hour - h1) == 0)
									{

										if ((min - min1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].origin_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);

										}
									}

								}
								else if ((yy2 - yy3) == 0)
								{
									if ((m2 - m3) > 0 && (m3 - m1) > 0)
									{

										if ((h2 - hour) > 0 && (hour - h1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].origin_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
										else if ((h2 - hour) == 0)
										{
											if ((min2 - min) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
										}
										else if ((hour - h1) == 0)
										{

											if ((min - min1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);

											}
										}

									}
									else if ((m2 - m3) == 0)
									{
										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) > 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}

									}
									else if ((m3 - m1) == 0)
									{

										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) == 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}


									}

								}
								else if ((yy3 - yy1) == 0)
								{

									if ((m2 - m3) > 0 && (m3 - m1) > 0)
									{

										if ((h2 - hour) > 0 && (hour - h1) > 0)
										{
											printf("%s|", ptr->List[k].cdrid);
											printf("%s|", ptr->List[k].origin_number);
											printf("%s|", ptr->List[k].date);
											printf("%s|", ptr->List[k].init_time);
											printf("%d|", ptr->List[k].duration);
											printf("%d|", ptr->List[k].type);
											printf("%d|", ptr->List[k].tarrif);
											printf("%d\n", ptr->List[k].fault_cond);
										}
										else if ((h2 - hour) == 0)
										{
											if ((min2 - min) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
										}
										else if ((hour - h1) == 0)
										{

											if ((min - min1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);

											}
										}

									}
									else if ((m2 - m3) == 0)
									{
										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) > 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].origin_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].origin_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}

									}
									else if ((m3 - m1) == 0)
									{

										if ((d2 - d3) > 0 && (d3 - d1) > 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}
										else if ((d2 - d3) == 0)
										{

											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}
										}
										else if ((d3 - d1) == 0)
										{
											if ((h2 - hour) > 0 && (hour - h1) > 0)
											{
												printf("%s|", ptr->List[k].cdrid);
												printf("%s|", ptr->List[k].dest_number);
												printf("%s|", ptr->List[k].date);
												printf("%s|", ptr->List[k].init_time);
												printf("%d|", ptr->List[k].duration);
												printf("%d|", ptr->List[k].type);
												printf("%d|", ptr->List[k].tarrif);
												printf("%d\n", ptr->List[k].fault_cond);
											}
											else if ((h2 - hour) == 0)
											{
												if ((min2 - min) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);
												}
											}
											else if ((hour - h1) == 0)
											{

												if ((min - min1) > 0)
												{
													printf("%s|", ptr->List[k].cdrid);
													printf("%s|", ptr->List[k].dest_number);
													printf("%s|", ptr->List[k].date);
													printf("%s|", ptr->List[k].init_time);
													printf("%d|", ptr->List[k].duration);
													printf("%d|", ptr->List[k].type);
													printf("%d|", ptr->List[k].tarrif);
													printf("%d\n", ptr->List[k].fault_cond);

												}
											}

										}


									}

								}

							}

						}

						ptr = ptr->next;

					}
					jobdone = 1;
					break;

				}

			}

			if (jobdone == 1)
			{
				break;
			}

			temp = temp->next;
		}

		if (jobdone == 1)
		{
			break;
		}

	}



}


int main(int argc, char *argv[])
{
	int hashentries, hashentries2, i, j;

	int duration_, type_, tarrif_, fault_cond_, indicator;

	int command_value, pos, counter, x, flag, cdridflag, origin_numberflag, dest_numberflag, dateflag, init_timeflag, durationflag, typeflag, tarrifflag, fault_condflag, hashtablenameflag, time1flag, time2flag, year1flag, year2flag, costflag;

	char temp[20], cdrid[20], origin_number[20], dest_number[20], date[20], init_time[20], duration[20], type[20], tarrif[20], fault_cond[20], hashtablename[20], time1[20], time2[20], year1[20], year2[20], cost[20];


#pragma region CreateChargesTable

	ChargesTable = (struct charges *)calloc(5, sizeof(struct charges));

	char const* const  chargefilename = argv[10];

	printf("Starting...\n");

	FILE *fp = fopen(chargefilename, "r");
	char line1[256];
	int var = 0;
	while (fgets(line1, sizeof(line1), fp) != NULL)
	{
		int isok = 0;
		int linecounter1 = 0;
		while (line1[linecounter1] != '\n')
		{
			linecounter1++;
		}

		if (linecounter1 == 0)
		{
			break;
		}
		else
		{
			pos = 0;
			counter = 0;
			x = 0;
			typeflag = 0;
			tarrifflag = 0;
			costflag = 0;

			for (i = 0; i <= 19; i++)
			{
				type[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				tarrif[i] = '\0';
			}

			for (i = 0; i <= 19; i++)
			{
				cost[i] = '\0';
			}

			while (x < (linecounter1))
			{

				if (line1[x] == '#')
				{
					isok = 1;
					break;
				}

				if (line1[x] != ';')
				{
					temp[pos] = line1[x];
				}

				if (line1[x] == ';')
				{
					counter++;


					if (counter == 1 && typeflag == 0)
					{
						for (i = 0; i <= 19; i++)
						{
							type[i] = temp[i];
						}

						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						pos = 0;
						typeflag = 1;
						x++;
						continue;
					}
					else if (counter == 2 && tarrifflag == 0)
					{
						for (i = 0; i <= 19; i++)
						{
							tarrif[i] = temp[i];
						}

						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						pos = 0;
						tarrifflag = 1;
						x++;
						continue;
					}

				}
				else
				{
					if (tarrifflag == 1)
					{
						cost[pos] = temp[pos];
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

			if (isok == 0)
			{
				char origin_type[2];
				origin_type[0] = type[0];
				origin_type[1] = '\0';

				int type1 = atoi(origin_type);
				int tarrif1 = atoi(tarrif);
				float cost1 = atof(cost);

				ChargesTable[var].type = type1;
				ChargesTable[var].tarrif = tarrif1;
				ChargesTable[var].cost = cost1;

				var++;
			}

		}

	}

	fclose(fp);


#pragma endregion


	int bucketsize = atoi(argv[8]);

	hashentries = atoi(argv[4]);

	hashentries2 = atoi(argv[6]);

	hashTable = (struct hash *)calloc(hashentries, sizeof(struct hash));

	hashTable2 = (struct hash *)calloc(hashentries, sizeof(struct hash));

	maxHeap *hp;
	hp->size = 0;

	if (argc > 2)
	{
		char const* const fileName = argv[2];
		FILE* file = fopen(fileName, "r");
		char line[256];
		char command[10];

		while (fgets(line, sizeof(line), file) != NULL)
		{
			int linecounter = 0;
			command_value = 0;
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

				if (linecounter == 3)
				{

					for (i = 0; i < 10; i++)
					{
						command[i] = '\0';
					}

					command_value = 8;

				}
				else
				{

					for (i = 0; i < 10; i++)
					{
						command[i] = '\0';
					}

					int j = 0;
					while (line[j] != ' ')
					{
						command[j] = line[j];
						j++;
					}

				}


				if (strcmp(command, "insert") == 0)
				{
					command_value = 1;
				}

				if (strcmp(command, "delete") == 0)
				{
					command_value = 2;
				}

				if (strcmp(command, "find") == 0)
				{
					command_value = 3;
				}

				if (strcmp(command, "lookup") == 0)
				{
					command_value = 4;
				}

				if (strcmp(command, "indist") == 0)
				{
					command_value = 5;
				}

				if (strcmp(command, "topdest") == 0)
				{
					command_value = 6;
				}

				if (strcmp(command, "top") == 0)
				{
					command_value = 7;
				}

				if (strcmp(command, "print") == 0)
				{
					command_value = 9;
				}

				if (strcmp(command, "dump") == 0)
				{
					command_value = 10;
				}


#pragma endregion

				switch (command_value)
				{

				case 1:

#pragma region Insert 

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						cdrid[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						origin_number[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						dest_number[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						date[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						init_time[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						duration[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						type[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						tarrif[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						fault_cond[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					cdridflag = 0;
					origin_numberflag = 0;
					dest_numberflag = 0;
					dateflag = 0;
					init_timeflag = 0;
					durationflag = 0;
					typeflag = 0;
					tarrifflag = 0;
					fault_condflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ';')
						{
							temp[pos] = line[x];
						}


						if (line[x] == ' ' && flag == 0)
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

						if (line[x] == ';')
						{
							counter++;


							if (counter == 1 && cdridflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									cdrid[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								cdridflag = 1;
								x++;
								continue;
							}
							else if (counter == 2 && origin_numberflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									origin_number[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								origin_numberflag = 1;
								x++;
								continue;
							}
							else if (counter == 3 && dest_numberflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									dest_number[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								dest_numberflag = 1;
								x++;
								continue;
							}
							else if (counter == 4 && dateflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									date[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								dateflag = 1;
								x++;
								continue;
							}
							else if (counter == 5 && init_timeflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									init_time[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								init_timeflag = 1;
								x++;
								continue;
							}
							else if (counter == 6 && durationflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									duration[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								durationflag = 1;
								x++;
								continue;
							}
							else if (counter == 7 && typeflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									type[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								typeflag = 1;
								x++;
								continue;
							}
							else if (counter == 8 && tarrifflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									tarrif[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								tarrifflag = 1;
								x++;
								continue;
							}

						}
						else
						{
							if (tarrifflag == 1)
							{
								fault_cond[pos] = temp[pos];
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

					duration_ = atoi(duration);
					type_ = atoi(type);
					tarrif_ = atoi(tarrif);
					fault_cond_ = atoi(fault_cond);

#pragma endregion

					Insert(cdrid, origin_number, dest_number, date, init_time, duration_, type_, tarrif_, fault_cond_, bucketsize, hashentries);
					Insert2(cdrid, origin_number, dest_number, date, init_time, duration_, type_, tarrif_, fault_cond_, bucketsize, hashentries2);
					InsertIntoMaxHeap(hp, origin_number, duration_, type_, tarrif_, fault_cond_, var);

#pragma endregion				
					break;

				case 2:

#pragma region Delete

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						cdrid[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						origin_number[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					cdridflag = 0;
					origin_numberflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ' ')
						{
							temp[pos] = line[x];
						}


						if (line[x] == ' ' && flag == 0)
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

						if (line[x] == ' ')
						{
							counter++;


							if (counter == 1 && cdridflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									cdrid[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								cdridflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (cdridflag == 1)
							{
								origin_number[pos] = temp[pos];
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

					//Delete(cdrid, origin_number,hashentries,bucketsize);



#pragma endregion
					break;

				case 3:

#pragma region Find


					x = 0;
					indicator = 0;
					while (x < (linecounter))
					{
						if (line[x] == ' ')
						{
							indicator++;
						}
						x++;
					}

					if (indicator == 1)
					{

					}
					else if (indicator == 3)
					{

						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							origin_number[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time2[i] = '\0';
						}

						pos = 0;
						counter = 0;
						x = 0;
						flag = 0;
						origin_numberflag = 0;
						time1flag = 0;
						time2flag = 0;

						while (x < (linecounter))
						{

							if (line[x] != ' ')
							{
								temp[pos] = line[x];
							}


							if (line[x] == ' ' && flag == 0)
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

							if (line[x] == ' ')
							{
								counter++;


								if (counter == 1 && origin_numberflag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										origin_number[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									origin_numberflag = 1;
									x++;
									continue;
								}
								else if (counter == 2 && time1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time1flag = 1;
									x++;
									continue;
								}
							}
							else
							{
								if (time1flag == 1)
								{
									time2[pos] = temp[pos];
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

						Find(origin_number, time1, time2, hashentries, bucketsize);
					}
					else
					{
						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							origin_number[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time2[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							year1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							year2[i] = '\0';
						}

						pos = 0;
						counter = 0;
						x = 0;
						flag = 0;
						origin_numberflag = 0;
						time1flag = 0;
						time2flag = 0;
						year1flag = 0;
						year2flag = 0;

						while (x < (linecounter))
						{

							if (line[x] != ' ')
							{
								temp[pos] = line[x];
							}


							if (line[x] == ' ' && flag == 0)
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

							if (line[x] == ' ')
							{
								counter++;


								if (counter == 1 && origin_numberflag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										origin_number[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									origin_numberflag = 1;
									x++;
									continue;
								}
								else if (counter == 2 && time1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time1flag = 1;
									x++;
									continue;
								}
								else if (counter == 3 && year1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										year1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									year1flag = 1;
									x++;
									continue;
								}
								else if (counter == 4 && time2flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time2[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time2flag = 1;
									x++;
									continue;
								}
							}
							else
							{
								if (time2flag == 1)
								{
									year2[pos] = temp[pos];
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

						FindWithYear(origin_number, time1, time2, year1, year2, hashentries, bucketsize);
					}


#pragma endregion

					printf("----------------------\n");

					break;

				case 4:

#pragma region Lookup

					x = 0;
					indicator = 0;
					while (x < (linecounter))
					{
						if (line[x] == ' ')
						{
							indicator++;
						}
						x++;
					}

					if (indicator == 1)
					{

					}
					else if (indicator == 3)
					{

						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							origin_number[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time2[i] = '\0';
						}

						pos = 0;
						counter = 0;
						x = 0;
						flag = 0;
						origin_numberflag = 0;
						time1flag = 0;
						time2flag = 0;

						while (x < (linecounter))
						{

							if (line[x] != ' ')
							{
								temp[pos] = line[x];
							}


							if (line[x] == ' ' && flag == 0)
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

							if (line[x] == ' ')
							{
								counter++;


								if (counter == 1 && origin_numberflag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										origin_number[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									origin_numberflag = 1;
									x++;
									continue;
								}
								else if (counter == 2 && time1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time1flag = 1;
									x++;
									continue;
								}
							}
							else
							{
								if (time1flag == 1)
								{
									time2[pos] = temp[pos];
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

						Find2(origin_number, time1, time2, hashentries2, bucketsize);

					}
					else
					{
						for (i = 0; i <= 19; i++)
						{
							temp[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							origin_number[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							time2[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							year1[i] = '\0';
						}

						for (i = 0; i <= 19; i++)
						{
							year2[i] = '\0';
						}

						pos = 0;
						counter = 0;
						x = 0;
						flag = 0;
						origin_numberflag = 0;
						time1flag = 0;
						time2flag = 0;
						year1flag = 0;
						year2flag = 0;

						while (x < (linecounter))
						{

							if (line[x] != ' ')
							{
								temp[pos] = line[x];
							}


							if (line[x] == ' ' && flag == 0)
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

							if (line[x] == ' ')
							{
								counter++;


								if (counter == 1 && origin_numberflag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										origin_number[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									origin_numberflag = 1;
									x++;
									continue;
								}
								else if (counter == 2 && time1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time1flag = 1;
									x++;
									continue;
								}
								else if (counter == 3 && year1flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										year1[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									year1flag = 1;
									x++;
									continue;
								}
								else if (counter == 4 && time2flag == 0)
								{
									for (i = 0; i <= 19; i++)
									{
										time2[i] = temp[i];
									}

									for (i = 0; i <= 19; i++)
									{
										temp[i] = '\0';
									}

									pos = 0;
									time2flag = 1;
									x++;
									continue;
								}
							}
							else
							{
								if (time2flag == 1)
								{
									year2[pos] = temp[pos];
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
						FindWithYear2(origin_number, time1, time2, year1, year2, hashentries2, bucketsize);
					}


#pragma endregion
					printf("----------------------\n");


					break;

				case 5:

#pragma region Indist

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						cdrid[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						origin_number[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					cdridflag = 0;
					origin_numberflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ' ')
						{
							temp[pos] = line[x];
						}


						if (line[x] == ' ' && flag == 0)
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

						if (line[x] == ' ')
						{
							counter++;


							if (counter == 1 && cdridflag == 0)
							{
								for (i = 0; i <= 19; i++)
								{
									cdrid[i] = temp[i];
								}

								for (i = 0; i <= 19; i++)
								{
									temp[i] = '\0';
								}

								pos = 0;
								cdridflag = 1;
								x++;
								continue;
							}
						}
						else
						{
							if (cdridflag == 1)
							{
								origin_number[pos] = temp[pos];
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


					//printf("Caller 1 is: %s\n", cdrid);
					//printf("Caller 2 is: %s\n", origin_number);
					//Indist(cdrid, origin_number, hashentries, hashentries2, bucketsize);


#pragma endregion
					break;

				case 6:

#pragma region TopDest

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						origin_number[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					origin_numberflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ';')
						{
							temp[pos] = line[x];
						}

						if (line[x] == ' ' && flag == 0)
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
						else
						{
							if (flag == 1)
							{
								origin_number[pos] = temp[pos];
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

					TopDest(origin_number, hashentries, bucketsize);



#pragma endregion
					printf("----------------------\n");
					break;

				case 7:

#pragma region Top

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						hashtablename[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					hashtablenameflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ';')
						{
							temp[pos] = line[x];
						}

						if (line[x] == ' ' && flag == 0)
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
						else
						{
							if (flag == 1)
							{
								hashtablename[pos] = temp[pos];
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

					hashtablenameflag = atoi(hashtablename);

#pragma endregion

					Top(hp, hashtablenameflag);

#pragma endregion
					break;

				case 8:
					break;

				case 9:

#pragma region Print

#pragma region ReadParameters

					for (i = 0; i <= 19; i++)
					{
						temp[i] = '\0';
					}

					for (i = 0; i <= 19; i++)
					{
						hashtablename[i] = '\0';
					}

					pos = 0;
					counter = 0;
					x = 0;
					flag = 0;
					hashtablenameflag = 0;

					while (x < (linecounter))
					{

						if (line[x] != ';')
						{
							temp[pos] = line[x];
						}

						if (line[x] == ' ' && flag == 0)
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
						else
						{
							if (flag == 1)
							{
								hashtablename[pos] = temp[pos];
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

					if (strcmp(hashtablename, "hashtable1") == 0)
					{
						PrintHashtable1(hashentries, bucketsize);
					}
					else
					{
						PrintHashtable2(hashentries2, bucketsize);
					}




#pragma endregion
					printf("----------------------\n");
					break;

				case 10:
					break;

				}

			}

		}
	}

	return 0;

}