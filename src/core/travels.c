#include "global.h"
#include "travels.h"
#include "../manager/stats.h"

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
	for(p=graph.roadlist[i].first;p!=NULL;p=p->next)
	{
		if(!visited[p->index])
		{
			DFS(graph,p->index);
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
	int j=0;
	int k=0; 
	for(i=0;i<graph.nodenum;i++)
	{
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
        (void)shortpath;
        int x = pathMatrix[i][j];
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
        char pnode[MAXNUM][10];
        int indegree[MAXNUM];
        int degarr[MAXNUM];
        const CNode *p = NULL;
	int i = 0;
	int top = 0;
	int count = 0;
        int result = 1;

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
          return result;
} 

typedef struct HeapNode {
        int node;
        double distance;
} HeapNode;

static void heapSwap(HeapNode *heap, int i, int j)
{
        HeapNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
}

static void heapifyUp(HeapNode *heap, int *heapSize)
{
        int i = *heapSize - 1;
        while(i > 0)
        {
                int parent = (i - 1) / 2;
                if(heap[i].distance >= heap[parent].distance)
                {
                        break;
                }
                heapSwap(heap, i, parent);
                i = parent;
        }
}

static void heapifyDown(HeapNode *heap, int *heapSize, int i)
{
        int smallest;
        int left, right;
        while(1)
        {
                left = 2 * i + 1;
                right = 2 * i + 2;
                smallest = i;
                if(left < *heapSize && heap[left].distance < heap[smallest].distance)
                {
                        smallest = left;
                }
                if(right < *heapSize && heap[right].distance < heap[smallest].distance)
                {
                        smallest = right;
                }
                if(smallest == i)
                {
                        break;
                }
                heapSwap(heap, i, smallest);
                i = smallest;
        }
}

static void heapInsert(HeapNode *heap, int *heapSize, int node, double distance)
{
        heap[*heapSize].node = node;
        heap[*heapSize].distance = distance;
        (*heapSize)++;
        heapifyUp(heap, heapSize);
}

static HeapNode heapExtractMin(HeapNode *heap, int *heapSize)
{
        HeapNode min = heap[0];
        (*heapSize)--;
        heap[0] = heap[*heapSize];
        heapifyDown(heap, heapSize, 0);
        return min;
}

int dijkstraShortestPath(ALGraph graph, int start, int end, double *distance, int prev[])
{
        double dist[MAXNUM];
        int visited[MAXNUM];
        HeapNode heap[MAXNUM * 2];
        int heapSize = 0;
        int i = 0;

        if(start < 0 || end < 0 || start >= graph.nodenum || end >= graph.nodenum || distance == NULL || prev == NULL)
        {
                return 0;
        }
        for(i = 0; i < graph.nodenum; i++)
        {
                dist[i] = INF;
                visited[i] = 0;
                prev[i] = -1;
        }

        dist[start] = 0;
        heapInsert(heap, &heapSize, start, 0);

        while(heapSize > 0)
        {
                HeapNode minNode = heapExtractMin(heap, &heapSize);
                int u = minNode.node;

                if(visited[u])
                {
                        continue;
                }
                visited[u] = 1;

                if(u == end)
                {
                        break;
                }

                for(const CNode *edge = graph.roadlist[u].first; edge != NULL; edge = edge->next)
                {
                        if(edge->length > 0 && !visited[edge->index] && dist[u] + edge->length < dist[edge->index])
                        {
                                dist[edge->index] = dist[u] + edge->length;
                                prev[edge->index] = u;
                                heapInsert(heap, &heapSize, edge->index, dist[edge->index]);
                        }
                }
        }

        *distance = dist[end];
        return dist[end] < INF;
}

int exportGuideRoute(ALGraph guidegraph, const char *filename)
{
        FILE *fp;
        int i;
        const CNode *p;
        int visited[MAXNUM];

        if(filename == NULL || guidegraph.nodenum <= 0)
        {
                return 0;
        }

        fp = fopen(filename, "w");
        if(fp == NULL)
        {
                return 0;
        }

        fprintf(fp, "Guide Route Report\n");
        fprintf(fp, "==================\n\n");
        fprintf(fp, "Total spots: %d\n", guidegraph.nodenum);
        fprintf(fp, "Total roads: %d\n\n", guidegraph.edgnum);

        fprintf(fp, "Spot Details:\n");
        fprintf(fp, "-------------\n");
        for(i = 0; i < guidegraph.nodenum; i++)
        {
                fprintf(fp, "%d. %s\n", i + 1, guidegraph.roadlist[i].data);
                if(guidegraph.roadlist[i].description[0] != '\0')
                {
                        fprintf(fp, "   Description: %s\n", guidegraph.roadlist[i].description);
                }
                fprintf(fp, "   Ticket Price: %.2f\n", guidegraph.roadlist[i].ticketPrice);
                if(guidegraph.roadlist[i].openTime[0] != '\0')
                {
                        fprintf(fp, "   Open Time: %s\n", guidegraph.roadlist[i].openTime);
                }
        }
        fprintf(fp, "\n");

        fprintf(fp, "Route Path:\n");
        fprintf(fp, "-----------\n");
        for(i = 0; i < guidegraph.nodenum; i++)
        {
                visited[i] = 0;
        }

        i = 0;
        while(i >= 0 && i < guidegraph.nodenum && visited[i] == 0)
        {
                visited[i] = 1;
                fprintf(fp, "%s", guidegraph.roadlist[i].data);

                for(p = guidegraph.roadlist[i].first; p != NULL; p = p->next)
                {
                        if(!visited[p->index])
                        {
                                fprintf(fp, " ->[%d]-> %s\n", p->length, guidegraph.roadlist[p->index].data);
                                i = p->index;
                                break;
                        }
                }
                if(p == NULL)
                {
                        break;
                }
        }
        fprintf(fp, "\n");

        fprintf(fp, "Adjacency Matrix:\n");
        fprintf(fp, "-----------------\n");
        for(i = 0; i < guidegraph.nodenum; i++)
        {
                fprintf(fp, "\t%s", guidegraph.roadlist[i].data);
        }
        fprintf(fp, "\n");
        for(i = 0; i < guidegraph.nodenum; i++)
        {
                fprintf(fp, "%s\t", guidegraph.roadlist[i].data);
                for(int j = 0; j < guidegraph.nodenum; j++)
                {
                        int length = getlength(guidegraph, i, j);
                        if(length > 0)
                        {
                                fprintf(fp, "%4d\t", length);
                        }
                        else if(i == j)
                        {
                                fprintf(fp, "%4d\t", 0);
                        }
                        else
                        {
                                fprintf(fp, " INF\t");
                        }
                }
                fprintf(fp, "\n");
        }

        fclose(fp);
        printf("Guide route exported to '%s'\n", filename);
        return 1;
}

