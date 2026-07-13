#ifndef PASSWORD_H
#define PASSWORD_H

#include <stddef.h>

#define PASSWORD_HASH_SIZE 128
#define PASSWORD_SALT_SIZE 17

int generatePasswordSalt(char *salt, size_t saltSize);
int hashPassword(const char *password, const char *salt, char *output, size_t outputSize);
int verifyPassword(const char *password, const char *storedHash);

#endif
