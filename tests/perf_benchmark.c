#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../graph.h"
#include "../travels.h"

static void set_vertex_name(ALGraph *graph, int index)
{
        snprintf(graph->roadlist[index].data, sizeof(graph->roadlist[index].data), "N%d", index);
        graph->roadlist[index].first = NULL;
}

static size_t estimate_graph_memory(const ALGraph *graph)
{
        if(graph == NULL)
        {
                return 0;
        }
        return sizeof(ALGraph) + (size_t)graph->edgnum * sizeof(CNode);
}

static void build_chain_graph(ALGraph *graph, int nodeCount, int extraStep)
{
        int i;

        initGraph(graph);
        graph->nodenum = nodeCount;
        for(i = 0; i < nodeCount; i++)
        {
                set_vertex_name(graph, i);
        }
        for(i = 0; i < nodeCount - 1; i++)
        {
                addEdge(graph, i, i + 1, 10);
                addEdge(graph, i + 1, i, 10);
        }
        for(i = 0; i + extraStep < nodeCount; i++)
        {
                addEdge(graph, i, i + extraStep, 12 + (i % 7));
                addEdge(graph, i + extraStep, i, 12 + (i % 7));
        }
}

static void build_dense_graph(ALGraph *graph, int nodeCount)
{
        int i;
        int j;

        initGraph(graph);
        graph->nodenum = nodeCount;
        for(i = 0; i < nodeCount; i++)
        {
                set_vertex_name(graph, i);
        }
        for(i = 0; i < nodeCount; i++)
        {
                for(j = 0; j < nodeCount; j++)
                {
                        if(i != j)
                        {
                                addEdge(graph, i, j, 5 + ((i + j) % 13));
                        }
                }
        }
}

static void build_random_graph(ALGraph *graph, int nodeCount, int edgePairs)
{
        int i;

        initGraph(graph);
        graph->nodenum = nodeCount;
        for(i = 0; i < nodeCount; i++)
        {
                set_vertex_name(graph, i);
        }
        srand(20260713u);
        for(i = 0; i < edgePairs; i++)
        {
                int from = rand() % nodeCount;
                int to = rand() % nodeCount;
                int length = 1 + (rand() % 50);
                if(from == to)
                {
                        to = (to + 1) % nodeCount;
                }
                addEdge(graph, from, to, length);
                addEdge(graph, to, from, length);
        }
}

static void build_unreachable_graph(ALGraph *graph, int nodeCount)
{
        int i;
        int half = nodeCount / 2;

        initGraph(graph);
        graph->nodenum = nodeCount;
        for(i = 0; i < nodeCount; i++)
        {
                set_vertex_name(graph, i);
        }
        for(i = 0; i < half - 1; i++)
        {
                addEdge(graph, i, i + 1, 10);
                addEdge(graph, i + 1, i, 10);
        }
        for(i = half; i < nodeCount - 1; i++)
        {
                addEdge(graph, i, i + 1, 10);
                addEdge(graph, i + 1, i, 10);
        }
}

static void run_case(const char *scenario, ALGraph *graph, int startNode, int endNode, int iterations)
{
        int prev[MAXNUM];
        double distance = 0;
        int i;
        clock_t start;
        clock_t end;
        double elapsedSeconds;
        double averageMilliseconds;
        int reachable = 0;

        start = clock();
        for(i = 0; i < iterations; i++)
        {
                reachable = dijkstraShortestPath(*graph, startNode, endNode, &distance, prev);
        }
        end = clock();
        elapsedSeconds = (double)(end - start) / CLOCKS_PER_SEC;
        averageMilliseconds = elapsedSeconds * 1000.0 / iterations;
        printf("%s,%d,%d,%d,%d,%lu,%.6f,%.6f\n",
               scenario,
               graph->nodenum,
               graph->edgnum,
               iterations,
               reachable,
               (unsigned long)estimate_graph_memory(graph),
               elapsedSeconds,
               averageMilliseconds);
}

int main(void)
{
        ALGraph graph;

        printf("scenario,nodes,edges,iterations,reachable,estimatedMemoryBytes,totalSeconds,averageMilliseconds\n");
        build_chain_graph(&graph, 10, 3);
        run_case("chain-small", &graph, 0, 9, 1000);
        freeGraph(&graph);

        build_chain_graph(&graph, 50, 7);
        run_case("chain-medium", &graph, 0, 49, 1000);
        freeGraph(&graph);

        build_chain_graph(&graph, 100, 11);
        run_case("chain-large", &graph, 0, 99, 1000);
        freeGraph(&graph);

        build_random_graph(&graph, 80, 160);
        run_case("random-sparse", &graph, 0, 79, 1000);
        freeGraph(&graph);

        build_dense_graph(&graph, 60);
        run_case("dense", &graph, 0, 59, 500);
        freeGraph(&graph);

        build_unreachable_graph(&graph, 80);
        run_case("unreachable", &graph, 0, 79, 1000);
        freeGraph(&graph);
        return 0;
}
