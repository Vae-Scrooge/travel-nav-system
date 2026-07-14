
#include "main.h"
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "src/core/global.h"
#include "src/core/graph.h"
#include "src/core/travels.h"
#include "src/console/menu.h"
#include "src/manager/userManager.h"
#include "src/manager/stats.h"
#include "src/manager/backup.h"

static void configureConsoleEncoding(void)
{
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
}

int main()
{
	ALGraph graph;
	ALGraph guidegraph; 
	int opt = 0;  
        int path[MAXNUM][MAXNUM];
        double shortpath[MAXNUM][MAXNUM];
	
        configureConsoleEncoding();
        initGraph(&graph);
        initGraph(&guidegraph);
	loadGraph(&graph); 
        statsLoad(&graph);
        statsRecordStart();
	transToMatrix(&graph);
	
        if(usermenu()==0)
        {
                freeMatrix(&graph);
                freeGraph(&guidegraph);
                freeGraph(&graph);
		return 0;
        }
		
	showMenu(); 
	while(2)
	{
                printf("请选择你的操作：");
                if(!readInt(&opt) || opt < 0 || opt > 11)
		{
                        printf("ERROR: 无效的选择，请重新输入。\n");
                        continue;
		}
		
		switch(opt)
		{
			case 0:
                                statsSave(&graph);
                                freeMatrix(&graph);
                                freeGraph(&guidegraph);
                                freeGraph(&graph);
				return 0;
			case  1:
                                if(!userCanManageGraph())
                                {
                                        printf("Permission denied: only administrators can create or replace graph data.\n");
                                        break;
                                }
                                printf("This operation replaces current graph data. Type yes to continue: ");
                                {
                                        char answer[8];
                                        if(!readString(answer, sizeof(answer)) || (strcmp(answer, "yes") != 0 && strcmp(answer, "YES") != 0))
                                        {
                                                printf("Operation cancelled.\n");
                                                userManagerLogAdminAction("graph.replace", "graph", "cancelled");
                                                break;
                                        }
                                }
				createGraph(&graph);
				saveGraph(&graph);
				transToMatrix(&graph); 
                                userManagerLogAdminAction("graph.replace", "graph", "success");
				printf("\n");
				break; 
			case  2:
				printGraph(&graph);
				printf("\n");
				break; 
			case  3:
				createGuideGraph(&graph,&guidegraph);
				printf("\n");
				break; 
			case  4:
                                if(guidegraph.nodenum <= 0)
                                {
                                        printf("Please create guide route first.\n");
                                        break;
                                }
				printGraph(&guidegraph);
				islooptest(guidegraph); 
                                transToMatrix(&graph);
				printf("\n");
				break; 
                        case  5:
                        {
                                char filename[50];
                                if(guidegraph.nodenum <= 0)
                                {
                                        printf("Please create guide route first.\n");
                                        break;
                                }
                                printf("Enter export filename: ");
                                readString(filename, sizeof(filename));
                                exportGuideRoute(guidegraph, filename);
                                printf("\n");
                                break;
                        }
			case  6:
                                statsRecordPathQuery();
                                transToMatrix(&graph);
				minDistance(graph,path,shortpath); 
				printf("\n");
				break;	
                        case  7:
                                if(!userCanManageGraph())
                                {
                                        printf("Permission denied: only administrators can manage scenic spots and roads.\n");
                                        break;
                                }
                                manageGraphCrud(&graph);
                                saveGraph(&graph);
                                transToMatrix(&graph);
                                printf("\n");
                                break;
                        case  8:
                                statsPrintReport(&graph);
                                printf("\n");
                                break;
                        case  9:
                        {
                                int backupOpt = -1;
                                char backupName[50];
                                while(backupOpt != 0)
                                {
                                        printf("\nBackup and Restore\n");
                                        printf("1 Create backup\n");
                                        printf("2 List backups\n");
                                        printf("3 Restore backup\n");
                                        printf("4 Delete backup\n");
                                        printf("0 Back\n");
                                        printf("Please select an option: ");
                                        if(!readInt(&backupOpt))
                                        {
                                                printf("Invalid option.\n");
                                                continue;
                                        }
                                        switch(backupOpt)
                                        {
                                                case 1:
                                                        backupCreate(&graph, "data/user.txt");
                                                        break;
                                                case 2:
                                                        backupList();
                                                        break;
                                                case 3:
                                                        printf("Enter backup name: ");
                                                        readString(backupName, sizeof(backupName));
                                                        if(backupRestore(backupName, &graph, "data/user.txt"))
                                                        {
                                                                transToMatrix(&graph);
                                                        }
                                                        break;
                                                case 4:
                                                        printf("Enter backup name: ");
                                                        readString(backupName, sizeof(backupName));
                                                        backupDelete(backupName);
                                                        break;
                                                case 0:
                                                        break;
                                                default:
                                                        printf("Invalid option.\n");
                                        }
                                }
                                printf("\n");
                                break;
                        }
                        case  10:
                        {
                                char oldPassword[COLUMNLENGTH];
                                char newPassword[COLUMNLENGTH];
                                const char *currentUser = userManagerGetCurrentUser();
                                printf("修改密码\n");
                                printf("旧密码：");
                                readString(oldPassword, sizeof(oldPassword));
                                printf("新密码：");
                                readString(newPassword, sizeof(newPassword));
                                userChangePassword(currentUser, oldPassword, newPassword);
                                printf("\n");
                                break;
                        }
                        case  11:
                                showHelpMenu();
                                printf("最近的迁移审计记录：\n");
                                userManagerPrintMigrationAudit(NULL, 10);
                                printf("\n最近的管理员操作记录：\n");
                                userManagerPrintAdminAudit(NULL, 10);
                                printf("\n");
                                break;
		} 
	}
        statsSave(&graph);
        freeMatrix(&graph);
        freeGraph(&guidegraph);
        freeGraph(&graph);
	return 0;
}
