#include "userManager.h"
#include "../core/password.h"
#include "stats.h"
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#define makeDirectory(path) _mkdir(path)
#else
#include <sys/stat.h>
#define makeDirectory(path) mkdir(path, 0700)
#endif

#define USER_ROLE_NAME "user"
#define ADMIN_ROLE_NAME "admin"
#define AUDIT_DIR "audit"

static char currentUsername[COLUMNLENGTH] = "";
static UserRole currentRole = USER_ROLE_GUEST;

static int storedPasswordIsHashed(const char *storedPassword)
{
        return storedPassword != NULL && strncmp(storedPassword, "pbkdf2_sha256$", 14) == 0;
}

static const char *roleToString(UserRole role)
{
        if(role == USER_ROLE_ADMIN)
        {
                return ADMIN_ROLE_NAME;
        }
        if(role == USER_ROLE_USER)
        {
                return USER_ROLE_NAME;
        }
        return "guest";
}

static UserRole parseRole(const char *roleName)
{
        if(roleName != NULL && strcmp(roleName, ADMIN_ROLE_NAME) == 0)
        {
                return USER_ROLE_ADMIN;
        }
        if(roleName != NULL && strcmp(roleName, USER_ROLE_NAME) == 0)
        {
                return USER_ROLE_USER;
        }
        return USER_ROLE_USER;
}

UserRole userManagerRoleForUsername(const char *username)
{
        (void)username;
        return USER_ROLE_USER;
}

UserRole userManagerRoleForUserInFile(const char *userPath, const char *username)
{
        FILE *fp;
        char line[256];
        char fileUser[COLUMNLENGTH];
        char filePassword[PASSWORD_FIELD_LENGTH];
        char fileRole[16];

        if(userPath == NULL || username == NULL || username[0] == '\0')
        {
                return USER_ROLE_GUEST;
        }
        fp = fopen(userPath, "r");
        if(fp == NULL)
        {
                return USER_ROLE_GUEST;
        }
        while(fgets(line, sizeof(line), fp) != NULL)
        {
                int fields = sscanf(line, "%9s %127s %15s", fileUser, filePassword, fileRole);
                if(fields >= 2 && strcmp(fileUser, username) == 0)
                {
                        fclose(fp);
                        if(fields >= 3)
                        {
                                return parseRole(fileRole);
                        }
                        return USER_ROLE_USER;
                }
        }
        fclose(fp);
        return USER_ROLE_GUEST;
}

void userManagerSetCurrentUser(const char *username)
{
        userManagerSetCurrentUserFromFile("data/user.txt", username);
}

void userManagerSetCurrentUserFromFile(const char *userPath, const char *username)
{
        if(username == NULL || username[0] == '\0')
        {
                currentUsername[0] = '\0';
                currentRole = USER_ROLE_GUEST;
                return;
        }
        strncpy(currentUsername, username, sizeof(currentUsername) - 1);
        currentUsername[sizeof(currentUsername) - 1] = '\0';
        currentRole = userManagerRoleForUserInFile(userPath, currentUsername);
        if(currentRole == USER_ROLE_GUEST)
        {
                currentRole = USER_ROLE_USER;
        }
}

const char *userManagerGetCurrentUser(void)
{
        return currentUsername;
}

UserRole userManagerGetCurrentRole(void)
{
        return currentRole;
}

int userCanManageGraph(void)
{
        return currentRole == USER_ROLE_ADMIN;
}

static void ensureMigrationDirectories(void)
{
        makeDirectory("backup");
        makeDirectory("backup/user");
        makeDirectory(AUDIT_DIR);
}

static void ensureAuditDirectory(void)
{
        makeDirectory(AUDIT_DIR);
}

static int copyFile(const char *sourcePath, const char *targetPath)
{
        FILE *source;
        FILE *target;
        char buffer[512];
        size_t bytesRead;

        if(sourcePath == NULL || targetPath == NULL)
        {
                return 0;
        }
        source = fopen(sourcePath, "rb");
        if(source == NULL)
        {
                return 0;
        }
        target = fopen(targetPath, "wb");
        if(target == NULL)
        {
                fclose(source);
                return 0;
        }
        while((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0)
        {
                if(fwrite(buffer, 1, bytesRead, target) != bytesRead)
                {
                        fclose(source);
                        fclose(target);
                        return 0;
                }
        }
        fclose(source);
        fclose(target);
        return 1;
}

static void sanitizePathName(const char *source, char *target, size_t targetSize)
{
        size_t i;
        if(target == NULL || targetSize == 0)
        {
                return;
        }
        if(source == NULL)
        {
                target[0] = '\0';
                return;
        }
        for(i = 0; source[i] != '\0' && i + 1 < targetSize; i++)
        {
                char ch = source[i];
                if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.')
                {
                        target[i] = ch;
                }
                else
                {
                        target[i] = '_';
                }
        }
        target[i] = '\0';
}

static int buildMigrationBackupPath(const char *userPath, char *backupPath, size_t backupPathSize)
{
        char safeName[96];
        time_t now;
        const struct tm *timeInfo;
        int written;

        if(userPath == NULL || backupPath == NULL || backupPathSize == 0)
        {
                return 0;
        }
        sanitizePathName(userPath, safeName, sizeof(safeName));
        now = time(NULL);
        timeInfo = localtime(&now);
        if(timeInfo == NULL)
        {
                return 0;
        }
        written = snprintf(backupPath, backupPathSize, "backup/user/%s_%04d%02d%02d%02d%02d%02d.bak",
                           safeName,
                           timeInfo->tm_year + 1900,
                           timeInfo->tm_mon + 1,
                           timeInfo->tm_mday,
                           timeInfo->tm_hour,
                           timeInfo->tm_min,
                           timeInfo->tm_sec);
        if(written < 0 || written >= (int)backupPathSize)
        {
                return 0;
        }
        return 1;
}

static void logMigrationEvent(const char *userPath, const char *username, const char *status, int affectedRows, double elapsedSeconds)
{
        FILE *logFile;
        time_t now;

        ensureMigrationDirectories();
        logFile = fopen("backup/user/migration.log", "a");
        if(logFile == NULL)
        {
                return;
        }
        now = time(NULL);
        fprintf(logFile, "%ld userPath=%s username=%s status=%s affectedRows=%d elapsedSeconds=%.3f\n",
                (long)now,
                userPath != NULL ? userPath : "(null)",
                username != NULL ? username : "(null)",
                status != NULL ? status : "(null)",
                affectedRows,
                elapsedSeconds);
        fclose(logFile);
}

void userManagerLogAdminAction(const char *action, const char *target, const char *result)
{
        FILE *logFile;
        time_t now;

        ensureAuditDirectory();
        logFile = fopen("audit/admin_actions.log", "a");
        if(logFile == NULL)
        {
                return;
        }
        now = time(NULL);
        fprintf(logFile, "%ld user=%s role=%s action=%s target=%s result=%s\n",
                (long)now,
                currentUsername[0] != '\0' ? currentUsername : "(anonymous)",
                roleToString(currentRole),
                action != NULL ? action : "(null)",
                target != NULL ? target : "(null)",
                result != NULL ? result : "(null)");
        fclose(logFile);
}

static void printAuditFile(const char *logPath, int maxLines)
{
        FILE *fp;
        char line[256];
        int count = 0;

        if(logPath == NULL || maxLines <= 0)
        {
                printf("No audit records requested.\n");
                return;
        }
        fp = fopen(logPath, "r");
        if(fp == NULL)
        {
                printf("Audit log is empty or unavailable: %s\n", logPath);
                return;
        }
        while(count < maxLines && fgets(line, sizeof(line), fp) != NULL)
        {
                printf("%s", line);
                count++;
        }
        fclose(fp);
        if(count == 0)
        {
                printf("Audit log is empty: %s\n", logPath);
        }
}

void userManagerPrintMigrationAudit(const char *logPath, int maxLines)
{
        printAuditFile(logPath != NULL ? logPath : "backup/user/migration.log", maxLines);
}

void userManagerPrintAdminAudit(const char *logPath, int maxLines)
{
        printAuditFile(logPath != NULL ? logPath : "audit/admin_actions.log", maxLines);
}

static int migrateLegacyUserPassword(const char *userPath, const char *username, const char *newPasswordHash)
{
        FILE *input;
        FILE *output;
        char tempPath[128];
        char backupPath[160];
        char fileUser[COLUMNLENGTH];
        char filePassword[PASSWORD_FIELD_LENGTH];
        char fileRole[16];
        char line[256];
        int migrated = 0;
        time_t startTime = time(NULL);

        if(userPath == NULL || username == NULL || newPasswordHash == NULL)
        {
                logMigrationEvent(userPath, username, "invalid-arguments", 0, 0);
                return 0;
        }
        if(snprintf(tempPath, sizeof(tempPath), "%s.tmp", userPath) < 0 || strlen(tempPath) >= sizeof(tempPath))
        {
                logMigrationEvent(userPath, username, "temp-path-too-long", 0, 0);
                return 0;
        }

        ensureMigrationDirectories();
        input = fopen(userPath, "r");
        if(input == NULL)
        {
                logMigrationEvent(userPath, username, "source-open-failed", 0, 0);
                return 0;
        }
        if(!buildMigrationBackupPath(userPath, backupPath, sizeof(backupPath)) || !copyFile(userPath, backupPath))
        {
                fclose(input);
                logMigrationEvent(userPath, username, "backup-failed", 0, difftime(time(NULL), startTime));
                return 0;
        }
        output = fopen(tempPath, "w");
        if(output == NULL)
        {
                fclose(input);
                logMigrationEvent(userPath, username, "temp-open-failed", 0, difftime(time(NULL), startTime));
                return 0;
        }

        while(fgets(line, sizeof(line), input) != NULL)
        {
                int fields = sscanf(line, "%9s %127s %15s", fileUser, filePassword, fileRole);
                if(fields < 2)
                {
                        fputs(line, output);
                        continue;
                }
                if(strcmp(fileUser, username) == 0)
                {
                        if(fields >= 3)
                        {
                                fprintf(output, "%s %s %s\n", fileUser, newPasswordHash, fileRole);
                        }
                        else
                        {
                                fprintf(output, "%s %s %s\n", fileUser, newPasswordHash, roleToString(USER_ROLE_USER));
                        }
                        migrated = 1;
                }
                else if(fields >= 3)
                {
                        fprintf(output, "%s %s %s\n", fileUser, filePassword, fileRole);
                }
                else
                {
                        fprintf(output, "%s %s\n", fileUser, filePassword);
                }
        }

        fclose(input);
        fclose(output);

        if(!migrated)
        {
                remove(tempPath);
                logMigrationEvent(userPath, username, "target-not-found", 0, difftime(time(NULL), startTime));
                return 0;
        }
        if(remove(userPath) != 0 || rename(tempPath, userPath) != 0)
        {
                remove(tempPath);
                copyFile(backupPath, userPath);
                logMigrationEvent(userPath, username, "rollback-completed", 0, difftime(time(NULL), startTime));
                return 0;
        }
        logMigrationEvent(userPath, username, "success", 1, difftime(time(NULL), startTime));
        return 1;
}

#ifdef UNIT_TEST
int userManagerTestMigrateLegacyUserPassword(const char *userPath, const char *username, const char *newPasswordHash)
{
        return migrateLegacyUserPassword(userPath, username, newPasswordHash);
}

const char *userManagerTestRoleToString(UserRole role)
{
        return roleToString(role);
}

int userManagerTestCopyFile(const char *sourcePath, const char *targetPath)
{
        return copyFile(sourcePath, targetPath);
}

void userManagerTestSanitizePathName(const char *source, char *target, size_t targetSize)
{
        sanitizePathName(source, target, targetSize);
}

int userManagerTestBuildMigrationBackupPath(const char *userPath, char *backupPath, size_t backupPathSize)
{
        return buildMigrationBackupPath(userPath, backupPath, backupPathSize);
}
#endif

int saveUser(const char username[COLUMNLENGTH],const char password[COLUMNLENGTH])
{
        return saveUserToFile("data/user.txt", username, password);
}

int saveUserToFile(const char *userPath, const char *username, const char *password)
{
        return saveUserToFileWithRole(userPath, username, password, USER_ROLE_USER);
}

int saveUserToFileWithRole(const char *userPath, const char *username, const char *password, UserRole role)
{
	FILE *fp;
        char salt[PASSWORD_SALT_SIZE];
        char passwordHash[PASSWORD_HASH_SIZE];
        if(userPath == NULL || username == NULL || password == NULL || username[0] == '\0' || password[0] == '\0')
        {
                printf("Username and password cannot be empty.\n");
                return 0;
        }
        if(validateUserInFile(userPath, username, password, 0) != 2)
        {
                printf("Username already exists.\n");
                return 0;
        }
        if(!generatePasswordSalt(salt, sizeof(salt)) || !hashPassword(password, salt, passwordHash, sizeof(passwordHash)))
        {
                printf("Failed to hash password.\n");
                return 0;
        }
        fp = fopen(userPath,"a");
	if(fp==NULL)
	{
                printf("Failed to open user file.\n");
		return 0;
	}
        fprintf(fp,"%s %s %s\n",username,passwordHash,roleToString(role));
	fclose(fp);
        statsRecordRegistration();
        printf("User saved.\n");
	return 1;
}

int validateUser(const char username[COLUMNLENGTH],const char password[COLUMNLENGTH])
{
        return validateUserInFile("data/user.txt", username, password, 1);
}

int userChangePasswordInFile(const char *userPath, const char *username, const char *oldPassword, const char *newPassword)
{
        FILE *input;
        FILE *output;
        char tempPath[128];
        char backupPath[160];
        char fileUser[COLUMNLENGTH];
        char filePassword[PASSWORD_FIELD_LENGTH];
        char fileRole[16];
        char line[256];
        int changed = 0;
        time_t startTime = time(NULL);

        if(userPath == NULL || username == NULL || oldPassword == NULL || newPassword == NULL || username[0] == '\0' || newPassword[0] == '\0')
        {
                printf("Invalid input.\n");
                return 0;
        }

        if(strcmp(oldPassword, newPassword) == 0)
        {
                printf("New password cannot be the same as old password.\n");
                return 0;
        }

        if(validateUserInFile(userPath, username, oldPassword, 0) != 1)
        {
                printf("Old password is incorrect.\n");
                return 0;
        }

        if(snprintf(tempPath, sizeof(tempPath), "%s.tmp", userPath) < 0 || strlen(tempPath) >= sizeof(tempPath))
        {
                printf("Failed to create temp file.\n");
                return 0;
        }

        ensureMigrationDirectories();
        input = fopen(userPath, "r");
        if(input == NULL)
        {
                printf("Failed to open user file.\n");
                return 0;
        }
        if(!buildMigrationBackupPath(userPath, backupPath, sizeof(backupPath)) || !copyFile(userPath, backupPath))
        {
                fclose(input);
                printf("Failed to backup user file.\n");
                return 0;
        }
        output = fopen(tempPath, "w");
        if(output == NULL)
        {
                fclose(input);
                printf("Failed to create temp file.\n");
                return 0;
        }

        while(fgets(line, sizeof(line), input) != NULL)
        {
                int fields = sscanf(line, "%9s %127s %15s", fileUser, filePassword, fileRole);
                if(fields < 2)
                {
                        fputs(line, output);
                        continue;
                }
                if(strcmp(fileUser, username) == 0)
                {
                        char salt[PASSWORD_SALT_SIZE];
                        char passwordHash[PASSWORD_HASH_SIZE];
                        if(!generatePasswordSalt(salt, sizeof(salt)) || !hashPassword(newPassword, salt, passwordHash, sizeof(passwordHash)))
                        {
                                fclose(input);
                                fclose(output);
                                remove(tempPath);
                                printf("Failed to hash new password.\n");
                                return 0;
                        }
                        if(fields >= 3)
                        {
                                fprintf(output, "%s %s %s\n", fileUser, passwordHash, fileRole);
                        }
                        else
                        {
                                fprintf(output, "%s %s %s\n", fileUser, passwordHash, roleToString(USER_ROLE_USER));
                        }
                        changed = 1;
                }
                else if(fields >= 3)
                {
                        fprintf(output, "%s %s %s\n", fileUser, filePassword, fileRole);
                }
                else
                {
                        fprintf(output, "%s %s\n", fileUser, filePassword);
                }
        }

        fclose(input);
        fclose(output);

        if(!changed)
        {
                remove(tempPath);
                printf("User not found.\n");
                return 0;
        }
        if(remove(userPath) != 0 || rename(tempPath, userPath) != 0)
        {
                remove(tempPath);
                copyFile(backupPath, userPath);
                printf("Failed to update password, rolled back.\n");
                return 0;
        }

        logMigrationEvent(userPath, username, "password-change", 1, difftime(time(NULL), startTime));
        printf("Password changed successfully.\n");
        return 1;
}

int userChangePassword(const char *username, const char *oldPassword, const char *newPassword)
{
        return userChangePasswordInFile("data/user.txt", username, oldPassword, newPassword);
}

int validateUserInFile(const char *userPath, const char *username, const char *password, int migrateLegacyPassword)
{
	char fusername[COLUMNLENGTH] = "";
        char fpassword[PASSWORD_FIELD_LENGTH] = "";
        char line[256];
	FILE *fp;
        int result = 2;

        if(userPath == NULL || username == NULL || password == NULL)
        {
                return 2;
        }
        fp = fopen(userPath,"r");
	if(fp==NULL)
	{
                return 2;
	}
        while(fgets(line, sizeof(line), fp) != NULL){
                if(sscanf(line, "%9s %127s", fusername, fpassword) != 2)
                {
                        continue;
                }
		if(strcmp(username,fusername) == 0)
		{
			break;
		}
	}
	fclose(fp);
	if(strcmp(username,fusername) !=0)
	{
                result = 2;
        }else if(!verifyPassword(password,fpassword))
	{
                result = 3;
        }
        else
        {
                result = 1;
                if(migrateLegacyPassword && !storedPasswordIsHashed(fpassword))
                {
                        char salt[PASSWORD_SALT_SIZE];
                        char passwordHash[PASSWORD_HASH_SIZE];
                        if(generatePasswordSalt(salt, sizeof(salt)) && hashPassword(password, salt, passwordHash, sizeof(passwordHash)))
                        {
                                migrateLegacyUserPassword(userPath, username, passwordHash);
                        }
                }
	}
        return result;
}
	
