#include "travels.h"

int visited[MAXNUM];//标记已经访问的经典  0 未访问  1 已经访问
char path[MAXNUM][10];  //存储遍历过程中顺序 访问的景点名称   
int index = 0;
CNode *p;
//深度优先遍历
void DFSTraverse(ALGraph graph)
{
	int i = 0;
	//初始化标记所有顶点都是没有访问 
	for(i=0;i<graph.nodenum;i++)
	{
		visited[i] = 0;
	}
	//按照深度优先访问景区景点 
	 for(i=0;i<graph.nodenum;i++)
	{
		if(!visited[i])
		{
			DFS(graph,i);
		}
	}
} 
//深度遍历递归
void DFS(ALGraph graph,int i)
{
    visited[i] = 1; //标记第i节点已经访问
	strcmp(path[index++],graph.roadlist[i].data);//复制访问节点的名称到访问数组中 
	//printf("-->%s",graph.roadlist[i].data); 
	//查找访问节点的邻接顶点是否已经访问，没有访问的话继续采用深度优先遍历形式进行访问
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(!visited[p->index])
		{
			DFS(graph,p->index);
		//	p = graph.roadlist[i].first;  //应该是 graph.roadlist[i].first 还是 graph.roadlist[p->index].first 还是可以不需要这条语句 
		}
	} 
} 
//是否是边
int isedg(ALGraph graph,char * e1,char * e2)
{
	int i = locate(graph,e1);
	int j = locate(graph,e2);
	CNode *p;
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(p->index == j)
		{
			return 1;
		}
	}
	return 0;
} 
//计算最短路径 
void shortPath(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM])
{

} 
//显示最短路径
void printPath(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM],int i,int j)
{
} 
//是否有回路
int islooptest()
{
	return 0;
} 
//查询最短距离
void minDistance(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM])
{
	
}
