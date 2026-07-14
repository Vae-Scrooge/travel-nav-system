#include "global.h"

unsigned int hashString(const char *str)
{
        unsigned int hash = 5381;
        int c;
        if(str == NULL) return 0;
        while((c = *str++) != '\0')
        {
                hash = ((hash << 5) + hash) + c;
        }
        return hash % HASH_SIZE;
}

void initHashTable(ALGraph *graph)
{
        int i;
        if(graph == NULL) return;
        for(i = 0; i < HASH_SIZE; i++)
        {
                graph->hashTable[i] = NULL;
        }
}

void freeHashTable(ALGraph *graph)
{
        int i;
        HashEntry *current, *next;
        if(graph == NULL) return;
        for(i = 0; i < HASH_SIZE; i++)
        {
                current = graph->hashTable[i];
                while(current != NULL)
                {
                        next = current->next;
                        free(current);
                        current = next;
                }
                graph->hashTable[i] = NULL;
        }
}

void hashInsert(ALGraph *graph, const char *key, int value)
{
        unsigned int index;
        HashEntry *entry;
        if(graph == NULL || key == NULL) return;
        index = hashString(key);
        entry = (HashEntry *)malloc(sizeof(HashEntry));
        if(entry == NULL) return;
        strncpy(entry->key, key, sizeof(entry->key) - 1);
        entry->key[sizeof(entry->key) - 1] = '\0';
        entry->value = value;
        entry->next = graph->hashTable[index];
        graph->hashTable[index] = entry;
}

void hashRemove(ALGraph *graph, const char *key)
{
        unsigned int index;
        HashEntry *current, *prev;
        if(graph == NULL || key == NULL) return;
        index = hashString(key);
        current = graph->hashTable[index];
        prev = NULL;
        while(current != NULL)
        {
                if(strcmp(current->key, key) == 0)
                {
                        if(prev == NULL)
                        {
                                graph->hashTable[index] = current->next;
                        }
                        else
                        {
                                prev->next = current->next;
                        }
                        free(current);
                        return;
                }
                prev = current;
                current = current->next;
        }
}

void hashUpdateAll(ALGraph *graph)
{
        int i;
        if(graph == NULL) return;
        freeHashTable(graph);
        for(i = 0; i < graph->nodenum; i++)
        {
                if(graph->roadlist[i].data[0] != '\0')
                {
                        hashInsert(graph, graph->roadlist[i].data, i);
                }
        }
}

int hashFind(ALGraph *graph, const char *key)
{
        unsigned int index;
        HashEntry *current;
        if(graph == NULL || key == NULL) return -1;
        index = hashString(key);
        current = graph->hashTable[index];
        while(current != NULL)
        {
                if(strcmp(current->key, key) == 0)
                {
                        return current->value;
                }
                current = current->next;
        }
        return -1;
}

int locate(ALGraph graph, const char *e)
{
        int result;
        int i;
        if(e == NULL)
        {
                return -1;
        }
        result = hashFind(&graph, e);
        if(result >= 0)
        {
                return result;
        }
        for(i=0;i<graph.nodenum;i++)
        {
                if(strcmp(e,graph.roadlist[i].data)==0)
                {
                        return i;
                }
        }
        return -1;
}

