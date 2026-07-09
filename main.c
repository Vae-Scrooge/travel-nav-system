
#include "main.h"
#include <ctype.h>

int main()
{
	ALGraph graph;
	ALGraph guidegraph; 
	int opt = 0;  
	char buf[100]; 
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
				break; 
			case  2:
				//景区景点分布图
				printGraph(&graph);
				break; 
			case  3:
				//导游路线
				createGuideGraph(&graph,&guidegraph);
				break; 
			case  4:
				//导游路线中的回路
				printf("游路线中的回路");
				break; 
			case  5:
				//两个景点间的最短路径和距离
				printf("两个景点间的最短路径和距离");
				break;	
		} 
	}
	return 0;
}
