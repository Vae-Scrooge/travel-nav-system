#include "global.h"
#include "graph.h"
#include "travels.h"

double **parray = NULL;
static int matrixNodeCount = 0;
extern char path[MAXNUM][10];
char * guidePath[2 * MAXNUM];

void initGraph(ALGraph *graph)
{
        int i = 0;
        if(graph == NULL)
        {
                return;
        }
        graph->nodenum = 0;
        graph->edgnum = 0;
        initHashTable(graph);
        for(i = 0; i < MAXNUM; i++)
        {
                graph->roadlist[i].data[0] = '\0';
                graph->roadlist[i].description[0] = '\0';
                graph->roadlist[i].ticketPrice = 0.0;
                graph->roadlist[i].openTime[0] = '\0';
                graph->roadlist[i].first = NULL;
        }
}

void freeGraph(ALGraph *graph)
{
        int i = 0;
        if(graph == NULL)
        {
                return;
        }
        for(i = 0; i < graph->nodenum; i++)
        {
                CNode *p = graph->roadlist[i].first;
                while(p != NULL)
                {
                        CNode *next = p->next;
                        free(p);
                        p = next;
                }
                graph->roadlist[i].first = NULL;
        }
        freeHashTable(graph);
        graph->nodenum = 0;
        graph->edgnum = 0;
}

int addEdge(ALGraph *graph, int from, int to, int length)
{
        CNode *node = NULL;
        if(graph == NULL || from < 0 || to < 0 || from >= graph->nodenum || to >= graph->nodenum || length <= 0)
        {
                return 0;
        }

        node = (CNode *)malloc(sizeof(CNode));
        if(node == NULL)
        {
                return 0;
        }
        node->index = to;
        node->length = length;
        node->next = graph->roadlist[from].first;
        graph->roadlist[from].first = node;
        graph->edgnum++;
        return 1;
}

static int updateDirectedEdge(ALGraph *graph, int from, int to, int length)
{
        CNode *p = NULL;
        if(graph == NULL || from < 0 || to < 0 || from >= graph->nodenum || to >= graph->nodenum || length <= 0)
        {
                return 0;
        }
        for(p = graph->roadlist[from].first; p != NULL; p = p->next)
        {
                if(p->index == to)
                {
                        p->length = length;
                        return 1;
                }
        }
        return addEdge(graph, from, to, length);
}

static int deleteDirectedEdge(ALGraph *graph, int from, int to)
{
        CNode *current = NULL;
        CNode *previous = NULL;
        if(graph == NULL || from < 0 || to < 0 || from >= graph->nodenum || to >= graph->nodenum)
        {
                return 0;
        }
        current = graph->roadlist[from].first;
        while(current != NULL)
        {
                if(current->index == to)
                {
                        if(previous == NULL)
                        {
                                graph->roadlist[from].first = current->next;
                        }
                        else
                        {
                                previous->next = current->next;
                        }
                        free(current);
                        graph->edgnum--;
                        return 1;
                }
                previous = current;
                current = current->next;
        }
        return 0;
}

int graphAddSpot(ALGraph *graph, const char *name)
{
        if(graph == NULL || name == NULL || name[0] == '\0')
        {
                return 0;
        }
        if(graph->nodenum >= MAXNUM || locate(*graph, name) >= 0)
        {
                return 0;
        }
        strncpy(graph->roadlist[graph->nodenum].data, name, sizeof(graph->roadlist[graph->nodenum].data) - 1);
        graph->roadlist[graph->nodenum].data[sizeof(graph->roadlist[graph->nodenum].data) - 1] = '\0';
        graph->roadlist[graph->nodenum].description[0] = '\0';
        graph->roadlist[graph->nodenum].ticketPrice = 0.0;
        graph->roadlist[graph->nodenum].openTime[0] = '\0';
        graph->roadlist[graph->nodenum].first = NULL;
        hashInsert(graph, name, graph->nodenum);
        graph->nodenum++;
        return 1;
}

int graphRenameSpot(ALGraph *graph, const char *oldName, const char *newName)
{
        int index;
        if(graph == NULL || oldName == NULL || newName == NULL || newName[0] == '\0')
        {
                return 0;
        }
        index = locate(*graph, oldName);
        if(index < 0 || locate(*graph, newName) >= 0)
        {
                return 0;
        }
        hashRemove(graph, oldName);
        strncpy(graph->roadlist[index].data, newName, sizeof(graph->roadlist[index].data) - 1);
        graph->roadlist[index].data[sizeof(graph->roadlist[index].data) - 1] = '\0';
        hashInsert(graph, newName, index);
        return 1;
}

int graphDeleteSpot(ALGraph *graph, const char *name)
{
        int index;
        int i;
        if(graph == NULL || name == NULL)
        {
                return 0;
        }
        index = locate(*graph, name);
        if(index < 0)
        {
                return 0;
        }

        while(graph->roadlist[index].first != NULL)
        {
                CNode *next = graph->roadlist[index].first->next;
                free(graph->roadlist[index].first);
                graph->roadlist[index].first = next;
                graph->edgnum--;
        }

        for(i = 0; i < graph->nodenum; i++)
        {
                if(i != index)
                {
                        while(deleteDirectedEdge(graph, i, index))
                        {
                        }
                }
        }

        hashRemove(graph, name);

        for(i = index; i < graph->nodenum - 1; i++)
        {
                graph->roadlist[i] = graph->roadlist[i + 1];
        }
        graph->nodenum--;
        graph->roadlist[graph->nodenum].data[0] = '\0';
        graph->roadlist[graph->nodenum].first = NULL;

        for(i = 0; i < graph->nodenum; i++)
        {
                CNode *p = graph->roadlist[i].first;
                while(p != NULL)
                {
                        if(p->index > index)
                        {
                                p->index--;
                        }
                        p = p->next;
                }
        }

        hashUpdateAll(graph);
        return 1;
}

int graphAddOrUpdateRoad(ALGraph *graph, const char *fromName, const char *toName, int length)
{
        int from;
        int to;
        if(graph == NULL || fromName == NULL || toName == NULL || length <= 0)
        {
                return 0;
        }
        from = locate(*graph, fromName);
        to = locate(*graph, toName);
        if(from < 0 || to < 0 || from == to)
        {
                return 0;
        }
        return updateDirectedEdge(graph, from, to, length) && updateDirectedEdge(graph, to, from, length);
}

int graphDeleteRoad(ALGraph *graph, const char *fromName, const char *toName)
{
        int from;
        int to;
        int deletedForward;
        int deletedBackward;
        if(graph == NULL || fromName == NULL || toName == NULL)
        {
                return 0;
        }
        from = locate(*graph, fromName);
        to = locate(*graph, toName);
        if(from < 0 || to < 0 || from == to)
        {
                return 0;
        }
        deletedForward = deleteDirectedEdge(graph, from, to);
        deletedBackward = deleteDirectedEdge(graph, to, from);
        return deletedForward || deletedBackward;
}

void freeMatrix(ALGraph *graph)
{
        int i = 0;
        (void)graph;
        if(parray == NULL)
        {
                parray = NULL;
                matrixNodeCount = 0;
                return;
        }
        for(i = 0; i < matrixNodeCount; i++)
        {
                free(parray[i]);
        }
        free(parray);
        parray = NULL;
        matrixNodeCount = 0;
}


void transToMatrix(ALGraph * graph)
{
        int i=0;
        const CNode *p = NULL;
        if(graph == NULL || graph->nodenum <= 0)
        {
                return;
        }
        freeMatrix(graph);
        parray = (double **)calloc(graph->nodenum, sizeof(double *));
        if(parray == NULL)
        {
                return;
        }
        matrixNodeCount = graph->nodenum;
	for(i=0;i<graph->nodenum;i++)
	{
		parray[i] = (double *)malloc(sizeof(double)*graph->nodenum);
                if(parray[i] == NULL)
                {
                        freeMatrix(graph);
                        return;
                }
	}
	 for(i=0;i<graph->nodenum;i++)
	{
                int j=0;
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
	 for(i=0;i<graph->nodenum;i++)
	{		
		for(p=graph->roadlist[i].first;p!=NULL;p=p->next)
		{
                        int edgeIndex = p->index;
                        parray[i][edgeIndex] = p->length;
		} 
	}
}


int getEdgeNum(const ALGraph * graph,const char vertexName[10])
{
	int num = 0;
        const CNode *p=NULL;
        int i = -1;
        if(graph == NULL || vertexName == NULL)
        {
                return 0;
        }
        i = locate(*graph,vertexName);
        if(i < 0)
        {
                return 0;
        }
	for(p=graph->roadlist[i].first;p!=NULL;p=p->next)
	{
		num++;
	}
	return num;	
}

void createGuideGraphEX(const ALGraph * graph,ALGraph * guidgraph,int n)
{
        int i=0,k=0;
        const CNode *p = NULL;
        if(graph == NULL || guidgraph == NULL)
        {
                return;
        }
        freeGraph(guidgraph);
	for(i=0;i<graph->nodenum;i++)
	{
		strcpy(guidgraph->roadlist[i].data , graph->roadlist[i].data);
		guidgraph->roadlist[i].first = NULL;
	}
        guidgraph->nodenum = graph->nodenum;
        guidgraph->edgnum = 0;
	for(k=0;k<n-1;k++)
	{
                int j = 0;
		i = locate(*graph,guidePath[k]);
		j = locate(*graph,guidePath[k+1]);
                if(i < 0 || j < 0)
                {
                        continue;
                }
		for(p = graph->roadlist[i].first;p!=NULL;p=p->next)
		{
			if(p->index == j)
			{
                                addEdge(guidgraph, i, j, p->length);
                                break;
			}
		}
	}
} 

void saveGraph(const ALGraph * graph)
{
        if(graphSaveToFiles(graph, "data/graphParams.txt", "data/graphVertex.txt", "data/graphEdge.txt"))
        {
                printf("Graph saved successfully.\n");
        }
        else
	{
                printf("Failed to save graph.\n");
	}
}

int graphSaveToFiles(const ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath)
{
        FILE *fp;
        int i;
        const CNode *p = NULL;

        if(graph == NULL || paramsPath == NULL || vertexPath == NULL || edgePath == NULL)
	{
                return 0;
        }

        fp = fopen(paramsPath, "w");
        if(fp == NULL)
        {
                return 0;
	}
        fprintf(fp, "%d %d\n", graph->nodenum, graph->edgnum);
        fclose(fp);

        fp = fopen(vertexPath, "w");
        if(fp == NULL)
	{
                return 0;
	}
        for(i = 0; i < graph->nodenum; i++)
        {
                fprintf(fp, "%s|%s|%.2f|%s\n", 
                        graph->roadlist[i].data,
                        graph->roadlist[i].description,
                        graph->roadlist[i].ticketPrice,
                        graph->roadlist[i].openTime);
        }
	fclose(fp);

        fp = fopen(edgePath, "w");
        if(fp == NULL)
	{
                return 0;
	}
        for(i = 0; i < graph->nodenum; i++)
	{
                for(p = graph->roadlist[i].first; p != NULL; p = p->next)
		{
                        if(p->index >= 0 && p->index < graph->nodenum)
                        {
                                fprintf(fp, "%s %s %d\n", graph->roadlist[i].data, graph->roadlist[p->index].data, p->length);
                        }
		}
	}
	fclose(fp);
        return 1;
} 

int graphUpdateSpotInfo(ALGraph *graph, const char *name, const char *description, double ticketPrice, const char *openTime)
{
        int index;
        if(graph == NULL || name == NULL)
        {
                return 0;
        }
        index = locate(*graph, name);
        if(index < 0)
        {
                return 0;
        }
        if(description != NULL)
        {
                strncpy(graph->roadlist[index].description, description, sizeof(graph->roadlist[index].description) - 1);
                graph->roadlist[index].description[sizeof(graph->roadlist[index].description) - 1] = '\0';
        }
        if(ticketPrice >= 0)
        {
                graph->roadlist[index].ticketPrice = ticketPrice;
        }
        if(openTime != NULL)
        {
                strncpy(graph->roadlist[index].openTime, openTime, sizeof(graph->roadlist[index].openTime) - 1);
                graph->roadlist[index].openTime[sizeof(graph->roadlist[index].openTime) - 1] = '\0';
        }
        return 1;
}

void graphPrintSpotInfo(ALGraph *graph, const char *name)
{
        int index;
        if(graph == NULL || name == NULL)
        {
                return;
        }
        index = locate(*graph, name);
        if(index < 0)
        {
                printf("Spot '%s' not found.\n", name);
                return;
        }
        printf("Spot: %s\n", graph->roadlist[index].data);
        printf("Description: %s\n", graph->roadlist[index].description[0] != '\0' ? graph->roadlist[index].description : "None");
        printf("Ticket Price: %.2f\n", graph->roadlist[index].ticketPrice);
        printf("Open Time: %s\n", graph->roadlist[index].openTime[0] != '\0' ? graph->roadlist[index].openTime : "None");
}

void loadGraph(ALGraph * graph)
{
        if(graphLoadFromFiles(graph, "data/graphParams.txt", "data/graphVertex.txt", "data/graphEdge.txt"))
        {
                printf("Graph loaded successfully: %d spots, %d roads.\n", graph->nodenum, graph->edgnum);
        }
        else
        {
                printf("Failed to load graph.\n");
        }
}

int graphLoadFromFiles(ALGraph *graph, const char *paramsPath, const char *vertexPath, const char *edgePath)
{
	FILE *fp;
        int fileEdgeNum = 0;
        int nodenum = 0;
        int i;
        char line[128];

        if(graph == NULL || paramsPath == NULL || vertexPath == NULL || edgePath == NULL)
        {
                return 0;
        }
        initGraph(graph);

        fp = fopen(paramsPath, "r");
        if(fp == NULL)
	{
                return 0;
	}
        if(fscanf(fp, "%d %d", &nodenum, &fileEdgeNum) != 2 || nodenum <= 0 || nodenum > MAXNUM || fileEdgeNum < 0)
        {
                fclose(fp);
                return 0;
        }
        graph->nodenum = 0;
        graph->edgnum = 0;
	fclose(fp);

        fp = fopen(vertexPath, "r");
        if(fp == NULL)
	{
                initGraph(graph);
                return 0;
	}
        for(i = 0; i < nodenum; i++)
	{
                char line[200];
                char *token;
                char *saveptr;

                if(fgets(line, sizeof(line), fp) == NULL)
                {
                        fclose(fp);
                        initGraph(graph);
                        return 0;
                }
                line[strcspn(line, "\n")] = '\0';

                token = strtok_r(line, "|", &saveptr);
                if(token == NULL || strlen(token) > 9)
                {
                        fclose(fp);
                        initGraph(graph);
                        return 0;
                }
                strcpy(graph->roadlist[i].data, token);
                graph->nodenum++;

                if(locate(*graph, graph->roadlist[i].data) != i)
                {
                        fclose(fp);
                        initGraph(graph);
                        return 0;
                }

                graph->roadlist[i].description[0] = '\0';
                graph->roadlist[i].ticketPrice = 0.0;
                graph->roadlist[i].openTime[0] = '\0';

                token = strtok_r(NULL, "|", &saveptr);
                if(token != NULL && strlen(token) > 0)
                {
                        strncpy(graph->roadlist[i].description, token, sizeof(graph->roadlist[i].description) - 1);
                        graph->roadlist[i].description[sizeof(graph->roadlist[i].description) - 1] = '\0';
                }

                token = strtok_r(NULL, "|", &saveptr);
                if(token != NULL && strlen(token) > 0)
                {
                        sscanf(token, "%lf", &graph->roadlist[i].ticketPrice);
                }

                token = strtok_r(NULL, "|", &saveptr);
                if(token != NULL && strlen(token) > 0)
                {
                        strncpy(graph->roadlist[i].openTime, token, sizeof(graph->roadlist[i].openTime) - 1);
                        graph->roadlist[i].openTime[sizeof(graph->roadlist[i].openTime) - 1] = '\0';
                }

		graph->roadlist[i].first = NULL;
	}
	fclose(fp);

        fp = fopen(edgePath, "r");
        if(fp == NULL)
	{
                return 0;
	}
        while(fgets(line, sizeof(line), fp) != NULL)
	{
                int length = 0;
                char extra[8] = "";
                char v1[10] = "";
                char v2[10] = "";
                int from = -1;
                int to = -1;

                if(line[0] == '\n' || line[0] == '\0')
                {
                        continue;
                }
                if(sscanf(line, "%9s %9s %d %7s", v1, v2, &length, extra) != 3)
                {
                        fclose(fp);
                        initGraph(graph);
                        return 0;
                }
                from = locate(*graph, v1);
                to = locate(*graph, v2);
                if(from < 0 || to < 0 || length <= 0)
                {
                        fclose(fp);
                        initGraph(graph);
                        return 0;
                }
                addEdge(graph, from, to, length);
	}
	fclose(fp);
        hashUpdateAll(graph);
        return 1;
}



