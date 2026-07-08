#include "graph.h"

//创建图的邻接表
void createGraph(ALGraph * graph)
{
	int i=0;
	int k=0;
	printf("请输入旅游图的景点数和边数：");
	scanf("%d %d",&graph->nodenum,&graph->edgnum);
	
	for(i=0;i<graph->nodenum;i++) //录入景点 
	{
		printf("请输入第%d个景点名称",i);
		scanf("%s",graph->roadlist[i].data);
	} 
	
	for(k=0;k<graph->edgnum;k++) //录入边 
	{
		printf("请输入第%d条边的两个顶点和长度：",k);
		int length = 0;
		char c1[10],c2[10];
		scanf("%s",c1);
		scanf("%s",c2);
		scanf("%d",&length);
		int i = locate(*graph,c1);
		int j = locate(*graph,c2);
		CNode *node1 = (CNode *)malloc(sizeof(CNode));
		CNode *node2 = (CNode *)malloc(sizeof(CNode));
		node1->length = length;
		node2->length = length;
		node1->index = i;
		node2->index = j;
		node1->next = graph->roadlist[j].first;
		graph->roadlist[j].first = node1;
		node2->next = graph->roadlist[i].first;
		graph->roadlist[i].first = node2;
	} 
}
 //查看整个图信息
void printGraph(ALGraph * graph)
{
	
}
//创建导游图 
void createGuideGraph(ALGraph * graph,ALGraph * guidgraph)
{
	
}

