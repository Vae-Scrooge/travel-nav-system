#include "../manager/userManager.h"
#include "../manager/stats.h"

int loginsys()
{
        char username[COLUMNLENGTH];
        char password[COLUMNLENGTH];
        int flag = 0;
        printf("Please input username and password. Input username 0 to exit.\n");
        while(1)
        {
                int rs;
                printf("Username: ");
                readString(username, sizeof(username));
                if(strcmp(username, "0") == 0)
                {
                        break;
                }
                printf("\n");
                printf("Password: ");
                readString(password, sizeof(password));
                printf("\n");
                rs = validateUser(username, password);
                if(rs == 1)
                {
                        printf("Login succeeded.\n");
                        userManagerSetCurrentUser(username);
                        statsRecordLogin();
                        flag = 1;
                        break;
                }
                else if(rs == 2)
                {
                        printf("Username does not exist.\n");
                }
                else if(rs == 3)
                {
                        printf("Password is incorrect.\n");
                }
        }
        return flag;
}

int userRegster()
{
        char username[COLUMNLENGTH];
        char password[COLUMNLENGTH];
        int result;

        printf("Please input username and password: ");
        readString(username, sizeof(username));
        readString(password, sizeof(password));
        printf("\n");
        result = saveUser(username, password);
        if(result == 0)
        {
                printf("Registration failed.\n");
                return 0;
        }
        printf("Registration succeeded.\n");
        return 1;
}

int usermenu()
{
        int opt = 0;
        int flag = 0;

        while(1)
        {
                printf("Please select an option:\n");
                printf("1 Login\n\n");
                printf("2 Register\n\n");
                printf("0 Exit\n\n");
                if(!readInt(&opt) || opt < 0 || opt > 2)
                {
                        printf("Invalid option, please try again.\n");
                        continue;
                }
                switch(opt)
                {
                        case 0:
                                return 0;
                        case 1:
                                if(loginsys() == 1)
                                {
                                        flag = 1;
                                }
                                break;
                        case 2:
                                if(userRegster() == 1)
                                {
                                        flag = 2;
                                }
                                break;
                        default:
                                break;
                }

                if(flag == 1)
                {
                        break;
                }
        }
        return flag;
}
