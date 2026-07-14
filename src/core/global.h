#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXNUM 100
#define INF  32767
#define HASH_SIZE 127

typedef struct stCNode{
	int index;
	struct stCNode * next;
	int length;
}CNode;

typedef struct stHNode{
	char data[10];
	char description[100];
	double ticketPrice;
	char openTime[20];
	CNode * first;
}HNode,RoadList[MAXNUM];

typedef struct HashEntry {
        char key[10];
        int value;
        struct HashEntry *next;
} HashEntry;

typedef struct stGraph{
	RoadList roadlist;
	int nodenum;
	int edgnum;
        HashEntry *hashTable[HASH_SIZE];
}ALGraph;

unsigned int hashString(const char *str);
void initHashTable(ALGraph *graph);
void freeHashTable(ALGraph *graph);
int hashInsert(ALGraph *graph, const char *key, int value);
void hashRemove(ALGraph *graph, const char *key);
void hashUpdateAll(ALGraph *graph);
int hashFind(ALGraph *graph, const char *key);
int locate(ALGraph graph, const char *e);
int readString(char *buffer, size_t size);
int readInt(int *value);
int readPassword(char *buffer, size_t size);

#endif
