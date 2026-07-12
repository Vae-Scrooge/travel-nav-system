#ifndef GRAPH_H
#define GRAPH_H

#include "global.h" 

void createGraph(ALGraph * graph);//创建图的邻接表
void printGraph(ALGraph * graph); //查看整个图信息
void createGuideGraph(ALGraph * graph,ALGraph * guidgraph);//创建导游图 

void saveGraph(ALGraph * graph);
void loadGraph(ALGraph * graph); 

void transToMatrix(ALGraph * graph); 

#endif
