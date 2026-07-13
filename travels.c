#include "travels.h"
#include "stats.h"

int visited[MAXNUM];
char path[MAXNUM][10];
extern double **parray; 
typedef char str[10];
static int dfsIndex = 0;

void DFSTraverse(ALGraph graph)
{
	int i = 0;
        dfsIndex = 0;
	for(i=0;i<graph.nodenum;i++)
	{
		visited[i] = 0;
	}
	 for(i=0;i<graph.nodenum;i++)
	{
		if(!visited[i])
		{
			DFS(graph,i);
		}
	}
} 

void DFS(ALGraph graph,int i)
{
        const CNode *p = NULL;
        visited[i] = 1;
        if(dfsIndex < MAXNUM)
        {
                strcpy(path[dfsIndex++],graph.roadlist[i].data);
        }
	//printf("-->%s",graph.roadlist[i].data); 
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(!visited[p->index])
		{
			DFS(graph,p->index);
                        p = graph.roadlist[i].first;
		}
	} 
} 

int isedg(ALGraph graph,const char * e1,const char * e2)
{
	int i = locate(graph,e1);
	int j = locate(graph,e2);
        const CNode *p = NULL;
        if(i < 0 || j < 0)
        {
                return 0;
        }
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(p->index == j)
		{
			return 1;
		}
	}
	return 0;
} 

int getlength(ALGraph graph,int i,int j)
{
        const CNode *p = NULL;
        if(i < 0 || j < 0 || i >= graph.nodenum || j >= graph.nodenum)
        {
                return 0;
        }
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(p->index == j)
		{
			return p->length;
		}
	}
	return 0;
} 


void shortPath(ALGraph graph,int pathMatrix[][MAXNUM],double shortpath[][MAXNUM])
{
	int i=0;
	int k=0; 
	for(i=0;i<graph.nodenum;i++)
	{
                int j=0;
		for(j=0;j<graph.nodenum;j++)
		{
                        shortpath[i][j] = parray[i][j];
                        pathMatrix[i][j] = -1;
                        if(parray[i][j]<INF)
			{
                                pathMatrix[i][j] = i;
			} 
		}
	}
        for(k=0;k<graph.nodenum;k++)
	{
                for(i=0;i<graph.nodenum;i++)
		{
                        int j=0;
                        for(j=0;j<graph.nodenum;j++)
			{
                                if(shortpath[i][k] >= INF || shortpath[k][j] >= INF)
                                {
                                        continue;
                                }
                                if((shortpath[i][k]+shortpath[k][j])<shortpath[i][j])
				{
					shortpath[i][j] = shortpath[i][k]+shortpath[k][j]; 
                                        pathMatrix[i][j] = k;
				} 
			} 
		}
	}
} 

void printPath(ALGraph graph,int pathMatrix[][MAXNUM],double shortpath[][MAXNUM],int i,int j)
{	
        int x = -1;
        (void)shortpath;
        if(i < 0 || j < 0 || i >= graph.nodenum || j >= graph.nodenum)
        {
                printf(" no path");
                return;
        }
        x = pathMatrix[i][j];
        if(x < 0)
        {
                printf(" no path");
                return;
        }
	if(x == i)
	{
		printf("->%s",graph.roadlist[j].data);
	}else
	{
                printPath(graph,pathMatrix,shortpath,i,x);
                printPath(graph,pathMatrix,shortpath,x,j);
	}
} 

int islooptest(ALGraph graph)
{
        str *pnode = (str *)malloc(sizeof(str)*graph.nodenum);
        int *indegree = (int *)malloc(sizeof(int)*graph.nodenum);
        int *degarr = (int *)malloc(sizeof(int)*graph.nodenum);
        const CNode *p = NULL;
	int i = 0;
	int top = 0;
	int count = 0;
        int result = 1;
        if(pnode == NULL || indegree == NULL || degarr == NULL)
        {
                free(pnode);
                free(indegree);
                free(degarr);
                printf("Not enough memory to test loop.\n");
                return 0;
        }
	for(i=0;i<graph.nodenum;i++)
	{
		indegree[i] = 0;	 
	} 
	 for(i=0;i<graph.nodenum;i++)
	{
		for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
		{
			indegree[p->index]++;
		}	 
	}
	for(i=0;i<graph.nodenum;i++)
	{
		if(indegree[i]==0)
		{
			degarr[top++] = i;
		}	 
		
	}
	 while(top!=0)
	 {
                int nodeIndex = degarr[--top];
                strcpy(pnode[count++],graph.roadlist[nodeIndex].data);
                for(p=graph.roadlist[nodeIndex].first;p!=NULL;p=p->next)
		{
			indegree[p->index]--;
			if(indegree[p->index] == 0)  
				degarr[top++] = p->index;
		}
	  } 
	  if(count<graph.nodenum)
	  {
                printf("Graph has a loop. Loop-related nodes:\n");
	  	for(i = 0;i < graph.nodenum;i++)
	  	{
                        int j = 0;
                        for(j=0;j<count;j++)
	  		{
	  			if(strcmp(graph.roadlist[i].data,pnode[j])==0) 
	  				break;
			}
			if(j>=count) 
				printf("%s-",graph.roadlist[i].data);
		}
                result = 0;
	  }else
	  {
                printf("Graph has no loop.\n");
                result = 1;
	  } 	 
          free(pnode);
          free(indegree);
          free(degarr);
          return result;
} 

int dijkstraShortestPath(ALGraph graph, int start, int end, double *distance, int prev[])
{
        double dist[MAXNUM];
        int used[MAXNUM];
        int i = 0;

        if(start < 0 || end < 0 || start >= graph.nodenum || end >= graph.nodenum || distance == NULL || prev == NULL)
        {
                return 0;
        }
        for(i = 0; i < graph.nodenum; i++)
        {
                dist[i] = INF;
                used[i] = 0;
                prev[i] = -1;
        }

        dist[start] = 0;

        for(i = 0; i < graph.nodenum; i++)
        {
                int u = -1;
                int v = 0;
                const CNode *edge = NULL;
                for(v = 0; v < graph.nodenum; v++)
                {
                        if(!used[v] && (u == -1 || dist[v] < dist[u]))
                        {
                                u = v;
                        }
                }
                if(u == -1 || dist[u] >= INF)
                {
                        break;
                }
                if(u == end)
                {
                        break;
                }

                used[u] = 1;
                for(edge = graph.roadlist[u].first; edge != NULL; edge = edge->next)
                {
                        if(edge->length > 0 && dist[u] + edge->length < dist[edge->index])
                        {
                                dist[edge->index] = dist[u] + edge->length;
                                prev[edge->index] = u;
                        }
                }
        }

        *distance = dist[end];
        return dist[end] < INF;
}

