#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "global.h"

#define COLUMNLENGTH 10
#define PASSWORD_FIELD_LENGTH 128

typedef enum UserRole {
        USER_ROLE_GUEST = 0,
        USER_ROLE_USER = 1,
        USER_ROLE_ADMIN = 2
} UserRole;

int loginsys();
int userRegster();
int saveUser(const char username[COLUMNLENGTH],const char password[COLUMNLENGTH]);
int validateUser(const char username[COLUMNLENGTH],const char password[COLUMNLENGTH]); // 1 success, 2 user missing, 3 wrong password
int saveUserToFile(const char *userPath, const char *username, const char *password);
int saveUserToFileWithRole(const char *userPath, const char *username, const char *password, UserRole role);
int validateUserInFile(const char *userPath, const char *username, const char *password, int migrateLegacyPassword);
void userManagerSetCurrentUser(const char *username);
void userManagerSetCurrentUserFromFile(const char *userPath, const char *username);
const char *userManagerGetCurrentUser(void);
UserRole userManagerGetCurrentRole(void);
UserRole userManagerRoleForUsername(const char *username);
UserRole userManagerRoleForUserInFile(const char *userPath, const char *username);
int userCanManageGraph(void);
void userManagerLogAdminAction(const char *action, const char *target, const char *result);
void userManagerPrintMigrationAudit(const char *logPath, int maxLines);
void userManagerPrintAdminAudit(const char *logPath, int maxLines);
#ifdef UNIT_TEST
int userManagerTestMigrateLegacyUserPassword(const char *userPath, const char *username, const char *newPasswordHash);
const char *userManagerTestRoleToString(UserRole role);
int userManagerTestCopyFile(const char *sourcePath, const char *targetPath);
void userManagerTestSanitizePathName(const char *source, char *target, size_t targetSize);
int userManagerTestBuildMigrationBackupPath(const char *userPath, char *backupPath, size_t backupPathSize);
#endif
int usermenu(); 
#endif
