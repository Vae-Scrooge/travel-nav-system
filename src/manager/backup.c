#include "backup.h"
#include "../core/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#define stat _stat
#endif

static int copyFile(const char *source, const char *dest)
{
        FILE *src = fopen(source, "rb");
        FILE *dst = fopen(dest, "wb");
        char buffer[1024];
        size_t n;

        if(src == NULL || dst == NULL)
        {
                if(src) fclose(src);
                if(dst) fclose(dst);
                return 0;
        }

        while((n = fread(buffer, 1, sizeof(buffer), src)) > 0)
        {
                fwrite(buffer, 1, n, dst);
        }

        fclose(src);
        fclose(dst);
        return 1;
}

static int ensureDirExists(const char *dir)
{
        struct stat st;
        if(stat(dir, &st) != 0)
        {
#ifdef _WIN32
                return mkdir(dir) == 0;
#else
                return mkdir(dir, 0755) == 0;
#endif
        }
        return S_ISDIR(st.st_mode);
}

static void cleanupOldBackups(int maxFiles)
{
        DIR *dir;
        struct dirent *entry;
        char paths[MAX_BACKUP_FILES * 2][256];
        int count = 0;
        int i, j;

        dir = opendir(BACKUP_DIR);
        if(dir == NULL) return;

        while((entry = readdir(dir)) != NULL)
        {
                if(entry->d_name[0] != '.')
                {
                        strcpy(paths[count], entry->d_name);
                        count++;
                }
        }
        closedir(dir);

        for(i = 0; i < count - 1; i++)
        {
                for(j = i + 1; j < count; j++)
                {
                        if(strcmp(paths[i], paths[j]) < 0)
                        {
                                char temp[256];
                                strcpy(temp, paths[i]);
                                strcpy(paths[i], paths[j]);
                                strcpy(paths[j], temp);
                        }
                }
        }

        for(i = maxFiles; i < count; i++)
        {
                char fullPath[256];
                sprintf(fullPath, "%s/%s", BACKUP_DIR, paths[i]);
                remove(fullPath);
        }
}

int backupCreate(const ALGraph *graph, const char *userPath)
{
        char timestamp[20];
        char backupDir[256];
        char paramsPath[256];
        char vertexPath[256];
        char edgePath[256];
        char userBackupPath[256];
        time_t now;

        if(!ensureDirExists(BACKUP_DIR))
        {
                printf("Failed to create backup directory.\n");
                return 0;
        }

        time(&now);
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
        sprintf(backupDir, "%s/%s", BACKUP_DIR, timestamp);

        if(!ensureDirExists(backupDir))
        {
                printf("Failed to create backup subdirectory.\n");
                return 0;
        }

        sprintf(paramsPath, "%s/graphParams.txt", backupDir);
        sprintf(vertexPath, "%s/graphVertex.txt", backupDir);
        sprintf(edgePath, "%s/graphEdge.txt", backupDir);

        if(!graphSaveToFiles(graph, paramsPath, vertexPath, edgePath))
        {
                printf("Failed to backup graph data.\n");
                remove(backupDir);
                return 0;
        }

        if(userPath != NULL)
        {
                sprintf(userBackupPath, "%s/user.txt", backupDir);
                if(!copyFile(userPath, userBackupPath))
                {
                        printf("Warning: Failed to backup user data.\n");
                }
        }

        cleanupOldBackups(MAX_BACKUP_FILES);
        printf("Backup created: %s\n", backupDir);
        return 1;
}

int backupRestore(const char *backupName, ALGraph *graph, const char *userPath)
{
        char backupDir[256];
        char paramsPath[256];
        char vertexPath[256];
        char edgePath[256];
        char userBackupPath[256];
        struct stat st;

        sprintf(backupDir, "%s/%s", BACKUP_DIR, backupName);

        if(stat(backupDir, &st) != 0 || !S_ISDIR(st.st_mode))
        {
                printf("Backup '%s' not found.\n", backupName);
                return 0;
        }

        sprintf(paramsPath, "%s/graphParams.txt", backupDir);
        sprintf(vertexPath, "%s/graphVertex.txt", backupDir);
        sprintf(edgePath, "%s/graphEdge.txt", backupDir);

        if(!graphLoadFromFiles(graph, paramsPath, vertexPath, edgePath))
        {
                printf("Failed to restore graph data.\n");
                return 0;
        }

        if(userPath != NULL)
        {
                sprintf(userBackupPath, "%s/user.txt", backupDir);
                if(stat(userBackupPath, &st) == 0)
                {
                        if(!copyFile(userBackupPath, userPath))
                        {
                                printf("Warning: Failed to restore user data.\n");
                        }
                }
        }

        printf("Backup '%s' restored successfully.\n", backupName);
        return 1;
}

void backupList(void)
{
        DIR *dir;
        struct dirent *entry;
        int found = 0;

        dir = opendir(BACKUP_DIR);
        if(dir == NULL)
        {
                printf("No backups found.\n");
                return;
        }

        printf("Available backups:\n");
        while((entry = readdir(dir)) != NULL)
        {
                if(entry->d_name[0] != '.')
                {
                        printf("  %s\n", entry->d_name);
                        found = 1;
                }
        }
        closedir(dir);

        if(!found)
        {
                printf("No backups found.\n");
        }
}

int backupDelete(const char *backupName)
{
        char backupDir[256];
        char paramsPath[256];
        char vertexPath[256];
        char edgePath[256];
        char userBackupPath[256];
        struct stat st;

        sprintf(backupDir, "%s/%s", BACKUP_DIR, backupName);

        if(stat(backupDir, &st) != 0 || !S_ISDIR(st.st_mode))
        {
                printf("Backup '%s' not found.\n", backupName);
                return 1;
        }

        sprintf(paramsPath, "%s/graphParams.txt", backupDir);
        sprintf(vertexPath, "%s/graphVertex.txt", backupDir);
        sprintf(edgePath, "%s/graphEdge.txt", backupDir);
        sprintf(userBackupPath, "%s/user.txt", backupDir);

        remove(paramsPath);
        remove(vertexPath);
        remove(edgePath);
        remove(userBackupPath);
        rmdir(backupDir);

        printf("Backup '%s' deleted.\n", backupName);
        return 1;
}