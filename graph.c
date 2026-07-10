#include "graph.h"
#include "travels.h"

double **parray; 

//创建图的邻接表
void createGraph(ALGraph * graph)
{
	int i=0;
	int k=0;
	printf("请输入旅游图的景点数和边数：");
	scanf("%d %d",&graph->nodenum,&graph->edgnum);
	
	for(i=0;i<graph->nodenum;i++) //录入景点 
	{
		printf("请输入第%d个景点名称:",i);
		scanf("%s",graph->roadlist[i].data);
		graph->roadlist[i].first = NULL;
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
	int i=0,j=0;
	//就是创建矩阵（也就二维数组） 
	//double **parray = (double *)malloc(sizeof(double)*graph->nodenum); 
	parray = (double *)malloc(sizeof(double)*graph->nodenum); 
	CNode *p = (CNode *)malloc(sizeof(CNode)); 
	for(i=0;i<graph->nodenum;i++)
	{
		parray[i] = (double *)malloc(sizeof(double)*graph->nodenum);
	}
	//初始化二维数组
	 for(i=0;i<graph->nodenum;i++)
	{
		for(j=0;j<graph->nodenum;j++)
		{
			if(i == j)
			{ 
				parray[i][j] = 0;
			}else
			{
				parray[i][j] = INF;
			}
		} 
	}
	//把景区路由图中景点间的边存到二维数组
	 for(i=0;i<graph->nodenum;i++)
	{		
		for(p=graph->roadlist[i].first;p!=NULL;p=p->next)
		{
			j = p->index;
			parray[i][j] = p->length;
		} 
	}
	//输出列标题  
	 for(i=0;i<graph->nodenum;i++)
	{
		printf("\t%s",graph->roadlist[i].data);
	}
	printf("\n");
	//输出行标题 和单元格数据 
	for(i=0;i<graph->nodenum;i++)
	{
		printf("%s\t",graph->roadlist[i].data);
		for(j=0;j<graph->nodenum;j++)
		{
			printf("%4.f\t",parray[i][j]);
		} 
		printf("\n");
	}
	free(p);
//	free(parray);	
}

int visited[MAXNUM];//标记已经访问的经典  0 未访问  1 已经访问
extern char path[MAXNUM][10];  //存储遍历过程中顺序 访问的景点名称  
char * guidePath[2 * MAXNUM];
int edgnum = 0;
//创建导游图 
void createGuideGraph(ALGraph * graph,ALGraph * guidgraph)
{
	DFSTraverse(*graph);
	int k = 0;
	int n = 0;
	int flag = 0;
	int i = 0;
	int j = 0;
	CNode *node1 = NULL;
	CNode *node2 = NULL;
	for(i = 0;i<graph->nodenum-1;i++)
	{
		k=0;
		flag = 1;
		while(flag)
		{
			guidePath[n++] = path[i+k];  //path保存的是深度优先访问的经典序列 
			if(isedg(*graph,path[i+k],path[i+1]))
			{
				flag = 0;
			}else
			{
				k--;
			}			
		}		
	}
	guidePath[n] = path[i];
	printf("导游线路图：\n");
	for(i=0;i<=n;i++)
	{
		printf("->%s",guidePath[i]);
	}
	printf("\n");
	for(i=0;i<=n;i++)
	{
		visited[i]=0;
	}
	for(i=0;i<graph->nodenum;i++)
	{
		strcpy(guidgraph->roadlist[i].data , graph->roadlist[i].data);
		guidgraph->roadlist[i].first = NULL;
	}
	for(k=0;k<n;k++)
	{
		i = locate(*graph,guidePath[k]);
		j = locate(*graph,guidePath[k+1]);
		if(visited[j])
		{
			int m = k+2;
			if(m<=n)
			{
				while(visited[locate(*graph,guidePath[m])])
				{
					if(isedg(*graph,guidePath[k],guidePath[m]))
					{
						j = locate(*graph,guidePath[m]);
						visited[j] = 1;
						visited[i] = 1;
						node1 = (CNode *) malloc(sizeof(CNode));
//					 	node2= (CNode *) malloc(sizeof(CNode));
					 	node1->index = j;
					 	node1->length = getlength(*graph,i,j);
					 	node1->next = guidgraph->roadlist[i].first;
					 	guidgraph->roadlist[i].first = node1;
//					 	node2->next = guidgraph->roadlist[i].first;
//					 	guidgraph->roadlist[i].first = node1;
//						node1->next = node2;
						edgnum ++ ;
					}
					if(++m>=n)
					{
						break;
					}					
				}
			}
		}else
		{
			visited[j] = 1;
			visited[i] = 1;
			node1 = (CNode *) malloc(sizeof(CNode));
//		 	node2= (CNode *) malloc(sizeof(CNode));
		 	node1->index = j;
		 	node1->length = getlength(*graph,i,j);
		 	node1->next = guidgraph->roadlist[i].first;
		 	guidgraph->roadlist[i].first = node1;
//		 	node2->next = guidgraph->roadlist[i].first;
//		 	guidgraph->roadlist[i].first = node1;
//			node1->next = node2;
			edgnum ++ ;
		}		
	}
	guidgraph->nodenum = graph->nodenum;
	guidgraph->edgnum = edgnum;
}

