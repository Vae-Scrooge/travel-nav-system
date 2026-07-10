#include "travels.h"

int visited[MAXNUM];//标记已经访问的经典  0 未访问  1 已经访问
char path[MAXNUM][10];  //存储遍历过程中顺序 访问的景点名称   
CNode *p;
typedef char str[10];
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
int index = 0;
//深度遍历递归
void DFS(ALGraph graph,int i)
{
    visited[i] = 1; //标记第i节点已经访问
	strcpy(path[index++],graph.roadlist[i].data);//复制访问节点的名称到访问数组中 
	//printf("-->%s",graph.roadlist[i].data); 
	//查找访问节点的邻接顶点是否已经访问，没有访问的话继续采用深度优先遍历形式进行访问
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(!visited[p->index])
		{
			DFS(graph,p->index);
			p = graph.roadlist[i].first;   //防递归回去时候出现空指针 
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

//获取两个顶点间的边长 
int getlength(ALGraph graph,int i,int j)
{
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(p->index == j)
		{
			return p->length;
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
int islooptest(ALGraph graph)
{
	str *pnode = (str *)malloc(sizeof(str)*graph.nodenum); //保存入度为0的顶点名称
	int *indegree = (int *)malloc(sizeof(int)*graph.nodenum); //保存顶点的入读数量
	int *degarr = (int *)malloc(sizeof(int)*graph.nodenum); //保存入度为0的顶点
	CNode *p = (CNode *) malloc(sizeof(CNode));
	int i = 0;
	int top = 0;
	int j = 0;
	int count = 0;
	//初始化所有顶点的入度为0 
	for(i=0;i<graph.nodenum;i++)
	{
		indegree[i] = 0;	 
	} 
	//统计所有顶点的入度数
	 for(i=0;i<graph.nodenum;i++)
	{
		for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
		{
			indegree[p->index]++;
		}	 
	}
	//找出入度为0的顶点，保存到 degarr 中
	for(i=0;i<graph.nodenum;i++)
	{
		if(indegree[i]==1)
		{
			degarr[top++] = i;
		}	 
		
	}
	 //去除入度为0的顶点j，并找出顶点j的邻接顶点是否有入度为0的顶点
	 while(top!=0)
	 {
	 	j = degarr[--top];
	 	strcpy(pnode[count++],graph.roadlist[j].data);//将入度为0的顶点名称拷贝到 pnode
	 	//检查邻接顶点去掉和j顶点的边，入度是否变为0
		for(p=graph.roadlist[j].first;p!=NULL;p=p->next)
		{
			indegree[p->index]--;
			if(indegree[p->index] == 1)  
				degarr[top++] = p->index;
		}
	  } 
	  //判断是否有回路，然后输出
	  if(count<graph.nodenum)
	  {
	  	printf("图中有回路,回路是：\n");
	  	for(i = 0;i < graph.nodenum;i++)
	  	{
	  		for(j=0;j<count;j++) //判断i节点是否是入度为0的节点 
	  		{
	  			if(strcmp(graph.roadlist[i].data,pnode[j])==0) 
	  				break;
			}
			if(j>=count) 
				printf("%s-",graph.roadlist[i].data);
		}
	  	return 0;
	  }else
	  {
	  	printf("不存在回路！\n");
	  	return 1;
	  } 	 
} 
//查询最短距离
void minDistance(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM])
{
	
}
