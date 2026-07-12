#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "global.h"

#define COLUMNLENGTH 10

int loginsys();
int userRegster();
int saveUser(char username[COLUMNLENGTH],char password[COLUMNLENGTH]);
int validateUser(char username[COLUMNLENGTH],char password[COLUMNLENGTH]); //返回 1 验证通过  0用户不存在 2 密码不正确 
int usermenu(); 
#endif
