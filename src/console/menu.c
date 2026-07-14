#include "menu.h"

void showMenu(void)
{
	printf("\n***********************************\n");
        printf("      欢迎使用导游路线规划系统\n\n");
        printf("请选择你的操作\n");
	printf("**********************************\n\n");
        printf("1 重新建立景点图\n\n");
        printf("2 查看景点图\n\n");
        printf("3 生成导游路线\n\n");
        printf("4 查看导游路线中的路径\n\n");
        printf("5 导出导游路线\n\n");
        printf("6 查询两点间最短路径\n\n");
        printf("7 管理景点和道路\n\n");
        printf("8 查看访问统计\n\n");
        printf("9 备份和恢复数据\n\n");
        printf("10 修改密码\n\n");
        printf("11 查看帮助和审计日志\n\n");
        printf("0 退出系统\n\n");
}

void showHelpMenu(void)
{
        printf("\n帮助\n");
        printf("1. 重新建立景点图：管理员权限。覆盖当前图数据。\n");
        printf("2. 查看景点图：打印当前邻接矩阵。\n");
        printf("3. 生成导游路线：基于深度优先遍历生成导游路线。\n");
        printf("4. 查看导游路线中的路径：验证导游路线是否包含回路。\n");
        printf("5. 导出导游路线：将导游路线导出到文件。\n");
        printf("6. 查询两点间最短路径：使用Floyd-Warshall算法查找最短路径。\n");
        printf("7. 管理景点和道路：管理员权限。删除操作需要确认。\n");
        printf("8. 查看访问统计：打印系统使用情况和景点访问次数。\n");
        printf("9. 备份和恢复数据：创建、列出、恢复和删除备份。\n");
        printf("10. 修改密码：修改当前用户密码。\n");
        printf("11. 查看帮助和审计日志：显示此帮助和最近的迁移/管理员审计记录。\n");
        printf("0. 退出：保存统计数据并释放资源。\n\n");

        printf("错误提示：\n");
        printf("- ERROR 表示请求的操作验证或持久化失败。\n");
        printf("- Permission denied 表示当前角色无法执行所选操作。\n");
        printf("- 只有确认删除或替换操作时才输入 yes。\n\n");
}
