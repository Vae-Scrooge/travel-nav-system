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
//	parray = (double *)malloc(sizeof(double)*graph->nodenum); 
//	CNode *p = (CNode *)malloc(sizeof(CNode)); 
//	for(i=0;i<graph->nodenum;i++)
//	{
//		parray[i] = (double *)malloc(sizeof(double)*graph->nodenum);
//	}
//	//初始化二维数组
//	 for(i=0;i<graph->nodenum;i++)
//	{
//		for(j=0;j<graph->nodenum;j++)
//		{
//			if(i == j)
//			{ 
//				parray[i][j] = 0;
//			}else
//			{
//				parray[i][j] = INF;
//			}
//		} 
//	}
//	//把景区路由图中景点间的边存到二维数组
//	 for(i=0;i<graph->nodenum;i++)
//	{		
//		for(p=graph->roadlist[i].first;p!=NULL;p=p->next)
//		{
//			j = p->index;
//			parray[i][j] = p->length;
//		} 
//	}
	
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
//	free(p);
//	free(parray);	
}

void transToMatrix(ALGraph * graph)
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
	free(p);
}

int visited[MAXNUM];//标记已经访问的经典  0 未访问  1 已经访问
extern char path[MAXNUM][10];  //存储遍历过程中顺序 访问的景点名称  
char * guidePath[2 * MAXNUM];
int edgnum = 0;
//创建导游图 
void createGuideGraph(ALGraph * graph,ALGraph * guidgraph)
{
	DFSTraverse(*graph); //深度优先遍历决定优先游览哪些景点 
	int k = 0;
	int n = 0;
	int flag = 0;
	int i = 0;
	int j = 0;
	CNode *node1 = NULL;
	CNode *node2 = NULL; 
	//游览路径尽量不走已经走过的路，最后要回到出发点乘车 
//	for(i = 0;i<graph->nodenum;i++)
//	{
//	
//	} 
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
	//导游图邻接表 
	createGuideGraph(guidgraph,n); 
//	for(i=0;i<=n;i++)
//	{
//		visited[i]=0;
//	}
//	for(i=0;i<graph->nodenum;i++)
//	{
//		strcpy(guidgraph->roadlist[i].data , graph->roadlist[i].data);
//		guidgraph->roadlist[i].first = NULL;
//	}
//	for(k=0;k<n;k++)
//	{
//		i = locate(*graph,guidePath[k]);
//		j = locate(*graph,guidePath[k+1]);
//		if(visited[j])
//		{
//			int m = k+2;
//			if(m<=n)
//			{
//				while(visited[locate(*graph,guidePath[m])])
//				{
//					if(isedg(*graph,guidePath[k],guidePath[m]))
//					{
//						j = locate(*graph,guidePath[m]);
//						visited[j] = 1;
//						visited[i] = 1;
//						node1 = (CNode *) malloc(sizeof(CNode));
////					 	node2= (CNode *) malloc(sizeof(CNode));
//					 	node1->index = j;
//					 	node1->length = getlength(*graph,i,j);
//					 	node1->next = guidgraph->roadlist[i].first;
//					 	guidgraph->roadlist[i].first = node1;
////					 	node2->next = guidgraph->roadlist[i].first;
////					 	guidgraph->roadlist[i].first = node1;
////						node1->next = node2;
//						edgnum ++ ;
//					}
//					if(++m>=n)
//					{
//						break;
//					}					
//				}
//			}
//		}else
//		{
//			visited[j] = 1;
//			visited[i] = 1;
//			node1 = (CNode *) malloc(sizeof(CNode));
////		 	node2= (CNode *) malloc(sizeof(CNode));
//		 	node1->index = j;
//		 	node1->length = getlength(*graph,i,j);
//		 	node1->next = guidgraph->roadlist[i].first;
//		 	guidgraph->roadlist[i].first = node1;
////		 	node2->next = guidgraph->roadlist[i].first;
////		 	guidgraph->roadlist[i].first = node1;
////			node1->next = node2;
//			edgnum ++ ;
//		}		
//	}
//	guidgraph->nodenum = graph->nodenum;
//	guidgraph->edgnum = edgnum;
}

void createGuideGraph(ALGraph * graph,ALGraph * guidgraph,int n)
{
	int i=0,j=0,k=0;
	CNode *node = NULL; 
	CNode *p = NULL;
	for(i=0;i<graph->nodenum;i++)
	{
		strcpy(guidgraph->roadlist[i].data , graph->roadlist[i].data);
		guidgraph->roadlist[i].first = NULL;
	}
	for(k=0;k<n-1;k++)
	{
		i = locate(*graph,guidePath[k]);
		j = locate(*graph,guidePath[k+1]);
		node = (CNode *) malloc(sizeof(CNode));
		for(p = graph->roadlist[i].first,p!=NULL;p=p->next)
		{
			if(p->index == j)
			{
				node->length = p->length;
			}
		}
		node->index = j;
		node->next = graph->roadlist[i].first;
		graph->roadlist[i].first = node; 
		edgnum ++ ;		
	}
	guidgraph->nodenum = graph->nodenum;
	guidgraph->edgnum = edgnum;
} 

void saveGraph(ALGraph * graph)
{
	FILE *fp;
	//保存顶点和边参数 
	fp = fopen("graphParams.txt","w");
	if(fp==NULL)
	{
		printf("打开图参数文件《%s》失败！\n","graphParams.txt");
		return ;
	}
	fprintf(fp,"%d %d\n",graph->nodenum,graph->edgnum);
	fclose(fp);
	//保存顶点
	fp = fopen("graphVertex.txt","w");
	if(fp==NULL)
	{
		printf("打开图顶点文件《%s》失败！\n","graphVertex.txt");
		return ;
	}
	int i = 0;
	for(i=0;i<graph->nodenum;i++)
	{
		fprintf(fp,"%s\n",graph->roadlist[i].data);
	}
	fclose(fp);
	//保存弧
	 fp = fopen("graphEdge.txt","w");
	if(fp==NULL)
	{
		printf("打开图的弧文件《%s》失败！\n","graphEdge.txt");
		return ;
	}
	i = 0;
	CNode *p=NULL; 
	for(i=0;i<graph->nodenum;i++)
	{
		for(p=graph->roadlist[i].first;p!=NULL;p=p->next)
		{
			fprintf(fp,"%s %s %d\n",graph->roadlist[i].data,graph->roadlist[p->index].data,p->length);
		}
	}
	fclose(fp);
	printf("图保存成功！\n");	
} 
void loadGraph(ALGraph * graph)
{
	FILE *fp;
	//加载图参数 
	fp = fopen("graphParams.txt","r");
	if(fp==NULL)
	{
		printf("打开图参数文件《%s》失败！\n","graphParams.txt");
		return ;
	}
	fscanf(fp,"%d %d\n",&graph->nodenum,&graph->edgnum);
	fclose(fp);
	//加载顶点
	fp = fopen("graphVertex.txt","r");
	if(fp==NULL)
	{
		printf("打开图顶点文件《%s》失败！\n","graphVertex.txt");
		return ;
	}
	int i = 0;
	for(i=0;i<graph->nodenum;i++)
	{
		fscanf(fp,"%s\n",graph->roadlist[i].data);
		graph->roadlist[i].first = NULL;
	}
	fclose(fp);
	//加载弧
	 fp = fopen("graphEdge.txt","r");
	if(fp==NULL)
	{
		printf("打开图的弧文件《%s》失败！\n","graphEdge.txt");
		return ;
	}
	int length = 0;
	CNode *p=NULL;
	char v1[10]="";
	char v2[10]=""; 
	while(fscanf(fp,"%s %s %d",v1,v2,&length)!=EOF)
	{
		i = locate(*graph,v1);
		p= (CNode *)malloc(sizeof(CNode));
		p->length = length;
		p->index = locate(*graph,v2);
		p->next =  graph->roadlist[i].first;
		graph->roadlist[i].first = p; 
	}
	fclose(fp);
	printf("图加载成功！\n");
}



