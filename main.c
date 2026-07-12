
#include "main.h"
#include <ctype.h>

int main()
{
	ALGraph graph;
	ALGraph guidegraph; 
	int opt = 0;  
	char buf[100]; 
	int path[MAXNUM][MAXNUM];  //存的是i到j两个顶点的最短路径（顶点序列）
	double shortpath[MAXNUM][MAXNUM]; //保存的是i到j两个顶点最短路径的权重（比如公里数）
	
	//加载图
	loadGraph(&graph); 
	//把景区路由图中景点间的边存到二维数组 
	transToMatrix(&graph);
	
	if(usermenu()==0)//退出系统 
		return 0;
		
	showMenu(); 
	while(2)
	{
		printf("请输入你的选择：");
		scanf("%s",buf);
		while(1)
		{
			if(!isdigit(buf[0])){
				printf("你输入的不是数字，请重新输入你的选择：");
				scanf("%s",buf);
			}else if(atoi(&buf[0])<0||atoi(&buf[0])>5)
			{
				printf("你输入的数字有错误，请重新输入你的选择：");
				scanf("%s",buf);
			}else{				
				opt = atoi(&buf[0]);
				break;
			}
		}
		
		switch(opt)
		{
			case 0:
				return 0;
			case  1:
				//创建景区景点图
				createGraph(&graph);
				//保存图 
				saveGraph(&graph);
				transToMatrix(&graph); 
				printf("\n");
				break; 
			case  2:
				//景区景点分布图
				printGraph(&graph);
				printf("\n");
				break; 
			case  3:
				//导游路线
				createGuideGraph(&graph,&guidegraph);
				printf("\n");
				break; 
			case  4:
				//导游路线中的回路
				printGraph(&guidegraph);
				islooptest(guidegraph); 
				printf("\n");
				break; 
			case  5:
				//两个景点间的最短路径和距离
				minDistance(graph,path,shortpath); 
				printf("\n");
				break;	
		} 
	}
	return 0;
}
