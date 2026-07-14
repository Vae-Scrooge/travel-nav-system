#include "../core/travels.h"
#include "../manager/stats.h"

void minDistance(ALGraph graph,int path[][MAXNUM],double shortpath[][MAXNUM])
{
        char start[10];
        char end[10];
        int i=0;
        int j=0;

        if(graph.nodenum <= 0)
        {
                printf("Graph is empty.\n");
                return;
        }
        shortPath(graph, path, shortpath);
        printf("请输入要查询的两个景点名称：\n");
        readString(start, sizeof(start));
        readString(end, sizeof(end));
        i = locate(graph,start);
        j = locate(graph,end);
        if(i < 0 || j < 0)
        {
                printf("Unknown scenic spot name.\n");
                return;
        }
        if(shortpath[i][j] >= INF)
        {
                printf("No available path from <%s> to <%s>.\n", start, end);
                return;
        }
        statsRecordSpotAccess(&graph, start);
        statsRecordSpotAccess(&graph, end);
        printf("<%s>到<%s>的距离是：%f米。\n",start,end,shortpath[i][j]);
        printf("<%s>到<%s>的路径是：\n",start,end);
        printf("%s", graph.roadlist[i].data);
        printPath(graph, path, shortpath, i, j);
        printf("\n");
}
