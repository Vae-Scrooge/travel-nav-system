#include "menu.h"

void showMenu(void)
{
	printf("\n***********************************\n");
        printf("      Welcome to Toursys\n\n");
        printf("Please select an option\n");
	printf("**********************************\n\n");
        printf("1 Create or replace scenic graph (admin only)\n\n");
        printf("2 View scenic graph\n\n");
        printf("3 Create guide route\n\n");
        printf("4 Check guide route loop\n\n");
        printf("5 Query shortest path\n\n");
        printf("6 Manage scenic spots and roads\n\n");
        printf("7 View access statistics\n\n");
        printf("8 View help and audit logs\n\n");
        printf("0 Exit\n\n");
}

void showHelpMenu(void)
{
        printf("\nHelp\n");
        printf("1. Create or replace scenic graph: administrator only. This overwrites current graph data.\n");
        printf("2. View scenic graph: print current adjacency matrix.\n");
        printf("3. Create guide route: generate a route based on graph traversal.\n");
        printf("4. Check guide route loop: verify whether the guide route contains loops.\n");
        printf("5. Query shortest path: use Dijkstra to find the shortest route between two spots.\n");
        printf("6. Manage scenic spots and roads: administrator only. Delete operations require confirmation.\n");
        printf("7. View access statistics: print system usage and scenic spot access counts.\n");
        printf("8. View help and audit logs: show this guide and recent migration/admin audit records.\n");
        printf("0. Exit: save statistics and release resources.\n\n");
        printf("Error tips:\n");
        printf("- ERROR means the requested operation failed validation or persistence.\n");
        printf("- Permission denied means the current role cannot perform the selected action.\n");
        printf("- Type yes only when you are sure about delete or replace operations.\n\n");
}
