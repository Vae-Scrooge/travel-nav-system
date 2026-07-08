#ifndef TRAVELS_H
#define TRAVELS_H

#include "global.h"


void DFSTravels(ALGraph graph);//深度优先遍历
void DFS(ALGraph graph,int i); //深度遍历递归
int isedg(ALGraph graph,char * e1,char * e2);//是否是边
void shortPath(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM]); //计算最短路径 
void printPath(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM],int i,int j);//显示最短路径
int islooptest(); //是否有回路
//查询最短距离
void minDistance(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM]); 


#endif
