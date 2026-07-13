
#include "main.h"
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif

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
                printf("Please select an option: ");
                if(!readInt(&opt) || opt < 0 || opt > 8)
		{
                        printf("ERROR: invalid option, please try again.\n");
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
				printf("\n");
				break; 
			case  5:
                                statsRecordPathQuery();
				minDistance(graph,path,shortpath); 
				printf("\n");
				break;	
                        case  6:
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
                        case  7:
                                statsPrintReport(&graph);
                                printf("\n");
                                break;
                        case  8:
                                showHelpMenu();
                                printf("Recent migration audit records:\n");
                                userManagerPrintMigrationAudit(NULL, 10);
                                printf("\nRecent administrator action records:\n");
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
