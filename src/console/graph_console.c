#include "../core/global.h"
#include "../core/graph.h"
#include "../core/travels.h"
#include "../manager/userManager.h"

extern char path[MAXNUM][10];
extern char *guidePath[2 * MAXNUM];

static int confirmAction(const char *message)
{
        char answer[8];

        printf("%s Type yes to continue: ", message);
        if(!readString(answer, sizeof(answer)))
        {
                return 0;
        }
        return strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0;
}

void manageGraphCrud(ALGraph *graph)
{
        int opt = -1;
        char first[10];
        char second[10];
        int length = 0;

        if(graph == NULL)
        {
                return;
        }

        while(opt != 0)
        {
                printf("\nScenic Spot and Road Management\n");
                printf("1 Add scenic spot\n");
                printf("2 Rename scenic spot\n");
                printf("3 Delete scenic spot\n");
                printf("4 Update spot details\n");
                printf("5 View spot details\n");
                printf("6 Add or update road\n");
                printf("7 Delete road\n");
                printf("8 List graph\n");
                printf("0 Back\n");
                printf("Please select an option: ");
                if(!readInt(&opt))
                {
                        printf("Invalid option.\n");
                        continue;
                }

                switch(opt)
                {
                        case 1:
                        {
                                int ok = 0;
                                printf("Input new scenic spot name: ");
                                readString(first, sizeof(first));
                                ok = graphAddSpot(graph, first);
                                printf(ok ? "Spot added.\n" : "ERROR: failed to add spot.\n");
                                userManagerLogAdminAction("spot.add", first, ok ? "success" : "failed");
                                break;
                        }
                        case 2:
                        {
                                int ok = 0;
                                printf("Input old and new scenic spot names: ");
                                readString(first, sizeof(first));
                                readString(second, sizeof(second));
                                ok = graphRenameSpot(graph, first, second);
                                printf(ok ? "Spot renamed.\n" : "ERROR: failed to rename spot.\n");
                                userManagerLogAdminAction("spot.rename", first, ok ? "success" : "failed");
                                break;
                        }
                        case 3:
                        {
                                int ok = 0;
                                printf("Input scenic spot name to delete: ");
                                readString(first, sizeof(first));
                                if(!confirmAction("Deleting a scenic spot also removes related roads."))
                                {
                                        printf("Operation cancelled.\n");
                                        userManagerLogAdminAction("spot.delete", first, "cancelled");
                                        break;
                                }
                                ok = graphDeleteSpot(graph, first);
                                printf(ok ? "Spot deleted.\n" : "ERROR: failed to delete spot.\n");
                                userManagerLogAdminAction("spot.delete", first, ok ? "success" : "failed");
                                break;
                        }
                        case 4:
                        {
                                int ok = 0;
                                char description[100];
                                double price = 0.0;
                                char openTime[20];
                                printf("Input spot name: ");
                                readString(first, sizeof(first));
                                printf("Input description: ");
                                readString(description, sizeof(description));
                                printf("Input ticket price: ");
                                readInt(&length);
                                price = (double)length;
                                printf("Input open time: ");
                                readString(openTime, sizeof(openTime));
                                ok = graphUpdateSpotInfo(graph, first, description, price, openTime);
                                printf(ok ? "Spot details updated.\n" : "ERROR: failed to update spot details.\n");
                                userManagerLogAdminAction("spot.update", first, ok ? "success" : "failed");
                                break;
                        }
                        case 5:
                        {
                                printf("Input spot name: ");
                                readString(first, sizeof(first));
                                graphPrintSpotInfo(graph, first);
                                break;
                        }
                        case 6:
                        {
                                int ok = 0;
                                printf("Input two spot names and road length: ");
                                readString(first, sizeof(first));
                                readString(second, sizeof(second));
                                ok = readInt(&length) && graphAddOrUpdateRoad(graph, first, second, length);
                                if(ok)
                                {
                                        printf("Road saved.\n");
                                }
                                else
                                {
                                        printf("ERROR: failed to save road.\n");
                                }
                                userManagerLogAdminAction("road.save", first, ok ? "success" : "failed");
                                break;
                        }
                        case 7:
                        {
                                int ok = 0;
                                printf("Input two spot names: ");
                                readString(first, sizeof(first));
                                readString(second, sizeof(second));
                                if(!confirmAction("Deleting a road may affect shortest path results."))
                                {
                                        printf("Operation cancelled.\n");
                                        userManagerLogAdminAction("road.delete", first, "cancelled");
                                        break;
                                }
                                ok = graphDeleteRoad(graph, first, second);
                                printf(ok ? "Road deleted.\n" : "ERROR: failed to delete road.\n");
                                userManagerLogAdminAction("road.delete", first, ok ? "success" : "failed");
                                break;
                        }
                        case 8:
                                printGraph(graph);
                                break;
                        case 0:
                                break;
                        default:
                                printf("Invalid option.\n");
                }
        }
}

void createGraph(ALGraph *graph)
{
        int i = 0;
        int k = 0;
        int nodenum = 0;
        int edgePairs = 0;

        if(graph == NULL)
        {
                return;
        }
        freeGraph(graph);

        printf("Please input scenic spot count and undirected road count: ");
        if(!readInt(&nodenum) || !readInt(&edgePairs) || nodenum <= 0 || nodenum > MAXNUM || edgePairs < 0)
        {
                printf("Invalid graph parameters.\n");
                initGraph(graph);
                return;
        }
        graph->nodenum = nodenum;
        graph->edgnum = 0;

        for(i = 0; i < graph->nodenum; i++)
        {
                printf("Please input scenic spot %d name: ", i);
                readString(graph->roadlist[i].data, sizeof(graph->roadlist[i].data));
                graph->roadlist[i].first = NULL;
        }

        for(k = 0; k < edgePairs; k++)
        {
                int length = 0;
                char c1[10];
                char c2[10];
                int from;
                int to;

                printf("Please input road %d endpoints and length: ", k);
                readString(c1, sizeof(c1));
                readString(c2, sizeof(c2));
                if(!readInt(&length) || length <= 0)
                {
                        printf("Invalid road length, skipped.\n");
                        continue;
                }
                from = locate(*graph, c1);
                to = locate(*graph, c2);
                if(from < 0 || to < 0)
                {
                        printf("Unknown scenic spot, skipped.\n");
                        continue;
                }
                addEdge(graph, from, to, length);
                addEdge(graph, to, from, length);
        }
}

void printGraph(ALGraph *graph)
{
        int i = 0;
        int j = 0;
        double **matrix;

        if(graph == NULL || graph->nodenum <= 0)
        {
                printf("Graph is empty.\n");
                return;
        }
        transToMatrix(graph);
        matrix = getMatrix();
        if(matrix == NULL)
        {
                printf("Failed to build adjacency matrix.\n");
                return;
        }

        for(i = 0; i < graph->nodenum; i++)
        {
                printf("\t%s", graph->roadlist[i].data);
        }
        printf("\n");
        for(i = 0; i < graph->nodenum; i++)
        {
                printf("%s\t", graph->roadlist[i].data);
                for(j = 0; j < graph->nodenum; j++)
                {
                        printf("%4.f\t", matrix[i][j]);
                }
                printf("\n");
        }
}

static int findVertexIndex(const char **pathArray, int length, const char *target)
{
        int i;
        for(i = 0; i < length; i++)
        {
                if(strcmp(pathArray[i], target) == 0)
                {
                        return i;
                }
        }
        return -1;
}

static int findAlternativePath(ALGraph *graph, const char *currentVertex, const char *prevVertex, const char **guidePathArray, int startIndex, int endIndex)
{
        CNode *p;
        int x = locate(*graph, currentVertex);
        if(x < 0)
        {
                return -1;
        }

        for(p = graph->roadlist[x].first; p != NULL; p = p->next)
        {
                const char *neighbor = graph->roadlist[p->index].data;
                if(strcmp(neighbor, prevVertex) != 0)
                {
                        int foundIndex = findVertexIndex(guidePathArray, endIndex, neighbor);
                        if(foundIndex >= startIndex && foundIndex < endIndex)
                        {
                                return foundIndex;
                        }
                }
        }
        return -1;
}

static int completeGuidePath(ALGraph *graph, const char **guidePathArray, int *n, const char *targetVertex, int isEnd)
{
        int tempIndex = *n - 1;
        int targetPos;

        targetPos = findVertexIndex(guidePathArray, *n, targetVertex);
        if(targetPos < 0)
        {
                return 0;
        }

        while(!isedg(*graph, guidePathArray[tempIndex], targetVertex))
        {
                int alternative;
                const char *prevVertex = (tempIndex > 0) ? guidePathArray[tempIndex - 1] : NULL;

                if(getEdgeNum(graph, guidePathArray[tempIndex]) > 1 && prevVertex != NULL)
                {
                        alternative = findAlternativePath(graph, guidePathArray[tempIndex], prevVertex, guidePathArray, targetPos, tempIndex);
                        if(alternative >= 0)
                        {
                                tempIndex = alternative;
                        }
                        else
                        {
                                tempIndex--;
                        }
                }
                else
                {
                        tempIndex--;
                }

                if(tempIndex >= 0)
                {
                        guidePathArray[(*n)++] = guidePathArray[tempIndex];
                }
                else
                {
                        break;
                }
        }

        if(isEnd && *n > 0)
        {
                guidePathArray[*n] = guidePathArray[0];
        }

        return 1;
}

void createGuideGraph(ALGraph *graph, ALGraph *guidgraph)
{
        int n = 0;
        int i = 0;
        char targetVertex[10] = "";
        int isEnd = 0;

        if(graph == NULL || guidgraph == NULL)
        {
                return;
        }
        DFSTraverse(*graph);

        for(i = 0; i < graph->nodenum; i++)
        {
                if(i == 0)
                {
                        guidePath[n++] = path[i];
                }
                else if(i == graph->nodenum - 1)
                {
                        guidePath[n++] = path[i];
                        if(!isedg(*graph, path[i], path[0]))
                        {
                                strcpy(targetVertex, path[0]);
                                isEnd = 1;
                                completeGuidePath(graph, (const char **)guidePath, &n, targetVertex, isEnd);
                        }
                }
                else
                {
                        guidePath[n++] = path[i];
                        if(!isedg(*graph, path[i], path[i + 1]))
                        {
                                strcpy(targetVertex, path[i + 1]);
                                isEnd = 0;
                                completeGuidePath(graph, (const char **)guidePath, &n, targetVertex, isEnd);
                        }
                }
        }

        printf("导游路线图:\n");
        for(i = 0; i <= n; i++)
        {
                printf("->%s", guidePath[i]);
        }
        printf("\n");
        createGuideGraphEX(graph, guidgraph, n);
}
