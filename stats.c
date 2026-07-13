#include "stats.h"

#include <time.h>

static SystemStats g_stats;

void statsInit(void)
{
        int i;
        g_stats.totalStarts = 0;
        g_stats.totalLogins = 0;
        g_stats.totalRegistrations = 0;
        g_stats.totalPathQueries = 0;
        for(i = 0; i < MAXNUM; i++)
        {
                g_stats.spotAccess[i] = 0;
        }
}

void statsLoad(const ALGraph *graph)
{
        FILE *fp = fopen("stats.txt", "r");
        char name[32];
        int value = 0;
        (void)graph;

        statsInit();
        if(fp == NULL)
        {
                return;
        }

        while(fscanf(fp, "%31s %d", name, &value) == 2)
        {
                if(strcmp(name, "starts") == 0)
                {
                        g_stats.totalStarts = value;
                }
                else if(strcmp(name, "logins") == 0)
                {
                        g_stats.totalLogins = value;
                }
                else if(strcmp(name, "registrations") == 0)
                {
                        g_stats.totalRegistrations = value;
                }
                else if(strcmp(name, "pathQueries") == 0)
                {
                        g_stats.totalPathQueries = value;
                }
                else if(strncmp(name, "spot", 4) == 0)
                {
                        int index = atoi(name + 4);
                        if(index >= 0 && index < MAXNUM)
                        {
                                g_stats.spotAccess[index] = value;
                        }
                }
        }
        fclose(fp);
}

void statsSave(const ALGraph *graph)
{
        FILE *fp = fopen("stats.txt", "w");

        if(fp == NULL)
        {
                printf("Failed to save stats.txt.\n");
                return;
        }

        fprintf(fp, "starts %d\n", g_stats.totalStarts);
        fprintf(fp, "logins %d\n", g_stats.totalLogins);
        fprintf(fp, "registrations %d\n", g_stats.totalRegistrations);
        fprintf(fp, "pathQueries %d\n", g_stats.totalPathQueries);
        if(graph != NULL)
        {
                int i;
                for(i = 0; i < graph->nodenum; i++)
                {
                        fprintf(fp, "spot%d %d\n", i, g_stats.spotAccess[i]);
                }
        }
        fclose(fp);
}

void statsRecordStart(void)
{
        g_stats.totalStarts++;
}

void statsRecordLogin(void)
{
        g_stats.totalLogins++;
}

void statsRecordRegistration(void)
{
        g_stats.totalRegistrations++;
}

void statsRecordPathQuery(void)
{
        g_stats.totalPathQueries++;
}

void statsRecordSpotAccess(const ALGraph *graph, const char *spotName)
{
        int index;
        if(graph == NULL || spotName == NULL)
        {
                return;
        }
        index = locate(*graph, spotName);
        if(index >= 0 && index < MAXNUM)
        {
                g_stats.spotAccess[index]++;
        }
}

void statsPrintReport(const ALGraph *graph)
{
        int i;
        time_t now = time(NULL);

        printf("\nAccess Statistics Report\n");
        printf("Generated at: %s", ctime(&now));
        printf("System starts: %d\n", g_stats.totalStarts);
        printf("Logins: %d\n", g_stats.totalLogins);
        printf("Registrations: %d\n", g_stats.totalRegistrations);
        printf("Path queries: %d\n", g_stats.totalPathQueries);
        printf("Spot access count:\n");

        if(graph == NULL || graph->nodenum <= 0)
        {
                printf("  Graph is empty.\n");
                return;
        }

        for(i = 0; i < graph->nodenum; i++)
        {
                printf("  %s: %d\n", graph->roadlist[i].data, g_stats.spotAccess[i]);
        }
}

SystemStats statsGetSnapshot(void)
{
        return g_stats;
}
