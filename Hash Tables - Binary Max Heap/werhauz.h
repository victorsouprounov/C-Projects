#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LISTSIZE 4
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2


typedef struct cdrnode {
	char* cdrid;
	char* origin_number;
	char* dest_number;
	char* date;
	char* init_time;
	int duration;
	int type;
	int tarrif;
	int fault_cond;

}cdrnode;


typedef struct listnode {
	cdrnode List[LISTSIZE];
	struct listnode* next;

}listnode;


typedef struct snode {
	char *number;
	listnode *listinfo;

}snode;


typedef struct node {
	struct snode *basketnode;
	struct node *next;
}node;


struct hash {
	struct node *head;
	int count;
}hash;


typedef struct topdest {
	char *code;
	int counter;

}topdest;

typedef struct namesdict {
	char *name;

}namesdict;

typedef struct charges {
	int type;
	int tarrif;
	float cost;
}charges;


typedef struct heapnode {
	char *number;
	float data;
} heapnode;

typedef struct maxHeap {
	int size;
	heapnode *elem;
} maxHeap;



struct hash *hashTable = NULL;

struct hash *hashTable2 = NULL;

topdest array[50];

namesdict *c1dictionary = NULL;

namesdict *c2dictionary = NULL;

struct charges *ChargesTable = NULL;