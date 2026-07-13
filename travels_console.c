#include "travels.h"
#include "stats.h"

static void printDijkstraPath(ALGraph graph, int prev[], int current)
{
        if(current < 0)
        {
                return;
        }
        if(prev[current] != -1)
        {
                printDijkstraPath(graph, prev, prev[current]);
                printf("->");
        }
        printf("%s", graph.roadlist[current].data);
}

void minDistance(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM])
{
        char start[10];
        char end[10];
        int i=0;
        int j=0;
        int prev[MAXNUM];
        double distance = 0;
        (void)path;
        (void)shortpath;

        if(graph.nodenum <= 0)
        {
                printf("Graph is empty.\n");
                return;
        }
        printf("Please input start and end scenic spot names:\n");
        readString(start, sizeof(start));
        readString(end, sizeof(end));
        i = locate(graph,start);
        j = locate(graph,end);
        if(i < 0 || j < 0)
        {
                printf("Unknown scenic spot name.\n");
                return;
        }
        if(!dijkstraShortestPath(graph, i, j, &distance, prev))
        {
                printf("No available path from <%s> to <%s>.\n", start, end);
                return;
        }
        statsRecordSpotAccess(&graph, start);
        statsRecordSpotAccess(&graph, end);
        printf("Distance from <%s> to <%s>: %.0f meters.\n",start,end,distance);
        printf("Shortest path: ");
        printDijkstraPath(graph, prev, j);
        printf("\n");
}
