#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXNUM 100
#define INF  32767

typedef struct stCNode{
	int index;
	struct stCNode * next;
	int length;
}CNode;  // Edge node information

typedef struct stHNode{
	char data[10];
	CNode * first;
}HNode,RoadList[MAXNUM];   // Vertex information

typedef struct stGraph{
	RoadList roadlist;
	int nodenum;
	int edgnum;
}ALGraph;  // Adjacency-list graph

int locate(ALGraph graph, const char *e);
int readString(char *buffer, size_t size);
int readInt(int *value);

#endif
