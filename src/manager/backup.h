#ifndef BACKUP_H
#define BACKUP_H

#include "global.h"

#define BACKUP_DIR "backup"
#define MAX_BACKUP_FILES 10

int backupCreate(const ALGraph *graph, const char *userPath);
int backupRestore(const char *backupPath, ALGraph *graph, const char *userPath);
void backupList(void);
int backupDelete(const char *backupName);

#endif