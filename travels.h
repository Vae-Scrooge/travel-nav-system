#ifndef TRAVELS_H
#define TRAVELS_H

#include "global.h"


void DFSTraverse(ALGraph graph);
void DFS(ALGraph graph,int i);
int isedg(ALGraph graph,const char * e1,const char * e2);
void shortPath(ALGraph graph,int pathMatrix[][MAXNUM],double shortpath[][MAXNUM]);
void printPath(ALGraph graph,int pathMatrix[][MAXNUM],double shortpath[][MAXNUM],int i,int j);
void minDistance(ALGraph graph,int pathMatrix[][MAXNUM],double shortpath[][MAXNUM]);
int islooptest(ALGraph graph);
int dijkstraShortestPath(ALGraph graph, int start, int end, double *distance, int prev[]);
int getlength(ALGraph graph,int i,int j);

#endif
