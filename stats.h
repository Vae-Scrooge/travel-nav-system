#ifndef STATS_H
#define STATS_H

#include "global.h"

typedef struct SystemStats {
        int totalStarts;
        int totalLogins;
        int totalRegistrations;
        int totalPathQueries;
        int spotAccess[MAXNUM];
} SystemStats;

void statsInit(void);
void statsLoad(const ALGraph *graph);
void statsSave(const ALGraph *graph);
void statsRecordStart(void);
void statsRecordLogin(void);
void statsRecordRegistration(void);
void statsRecordPathQuery(void);
void statsRecordSpotAccess(const ALGraph *graph, const char *spotName);
void statsPrintReport(const ALGraph *graph);
SystemStats statsGetSnapshot(void);

#endif
