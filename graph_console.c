#include "graph.h"
#include "travels.h"
#include "userManager.h"

extern double **parray;
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
                printf("4 Add or update road\n");
                printf("5 Delete road\n");
                printf("6 List graph\n");
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
                        case 5:
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
                        case 6:
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

        if(graph == NULL || graph->nodenum <= 0)
        {
                printf("Graph is empty.\n");
                return;
        }
        transToMatrix(graph);
        if(parray == NULL)
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
                        printf("%4.f\t", parray[i][j]);
                }
                printf("\n");
        }
}

void createGuideGraph(ALGraph *graph, ALGraph *guidgraph)
{
        int n = 0;
        int flag = 0;
        int i = 0;
        char targetVertex[10] = "";

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
                        printf("%s ", path[i]);
                }
                else if(i == graph->nodenum - 1)
                {
                        guidePath[n++] = path[i];
                        printf("%s ", path[i]);
                        if(!isedg(*graph, path[i], path[0]))
                        {
                                flag = 1;
                                strcpy(targetVertex, path[0]);
                        }
                }
                else
                {
                        guidePath[n++] = path[i];
                        printf("%s ", path[i]);
                        if(!isedg(*graph, path[i], path[i + 1]))
                        {
                                flag = 1;
                                strcpy(targetVertex, path[i + 1]);
                        }
                }

                if(flag)
                {
                        int tempIndex = n - 1;
                        int temp = 0;
                        int mm = n - 1;
                        for(mm = n - 1; mm >= 0; mm--)
                        {
                                if(strcmp(targetVertex, guidePath[mm]) == 0)
                                {
                                        temp = mm;
                                }
                        }
                        while(!isedg(*graph, guidePath[tempIndex], targetVertex))
                        {
                                if(getEdgeNum(graph, guidePath[tempIndex]) > 1)
                                {
                                        CNode *p = NULL;
                                        int x = locate(*graph, guidePath[tempIndex]);
                                        for(p = graph->roadlist[x].first; p != NULL; p = p->next)
                                        {
                                                if(strcmp(graph->roadlist[p->index].data, guidePath[tempIndex - 1]) != 0)
                                                {
                                                        for(; temp < tempIndex - 1; temp++)
                                                        {
                                                                if(isedg(*graph, graph->roadlist[p->index].data, guidePath[temp]))
                                                                {
                                                                        break;
                                                                }
                                                        }
                                                }
                                                if(temp != tempIndex - 1)
                                                {
                                                        break;
                                                }
                                        }
                                        if(p != NULL)
                                        {
                                                tempIndex = temp;
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
                                        guidePath[n++] = path[tempIndex];
                                        printf("%s ", path[tempIndex]);
                                }
                                else
                                {
                                        break;
                                }
                        }
                }
                flag = 0;
        }

        printf("Guide route graph:\n");
        for(i = 0; i <= n; i++)
        {
                printf("->%s", guidePath[i]);
        }
        printf("\n");
        createGuideGraphEX(graph, guidgraph, n);
}
