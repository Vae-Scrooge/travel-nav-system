#ifndef GRAPH_H
#define GRAPH_H

#include "global.h" 

void createGraph(ALGraph * graph);
void printGraph(ALGraph * graph);
void createGuideGraph(ALGraph * graph,ALGraph * guidgraph);

void initGraph(ALGraph *graph);
void freeGraph(ALGraph *graph);
int addEdge(ALGraph *graph, int from, int to, int length);
int graphAddSpot(ALGraph *graph, const char *name);
int graphRenameSpot(ALGraph *graph, const char *oldName, const char *newName);
int graphDeleteSpot(ALGraph *graph, const char *name);
int graphUpdateSpotInfo(ALGraph *graph, const char *name, const char *description, double ticketPrice, const char *openTime);
void graphPrintSpotInfo(ALGraph *graph, const char *name);
int graphAddOrUpdateRoad(ALGraph *graph, const char *fromName, const char *toName, int length);
int graphDeleteRoad(ALGraph *graph, const char *fromName, const char *toName);
void manageGraphCrud(ALGraph *graph);
void saveGraph(const ALGraph * graph);
void loadGraph(ALGraph * graph); 
int graphSaveToFiles(const ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath);
int graphLoadFromFiles(ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath);
void freeMatrix(ALGraph *graph);

void transToMatrix(ALGraph * graph); 
int getEdgeNum(const ALGraph * graph,const char vertexName[10]);
void createGuideGraphEX(const ALGraph * graph,ALGraph * guidgraph,int n);

#endif
