
#include "main.h"
#include <ctype.h>

int main()
{
	char e1[10];
	char e2[10];
	
	ALGraph graph; 
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
				//对应的工程
				createGraph(&graph);
				break; 
			case  2:
				//对应的工程
				printGraph(&graph);
				break; 
			case  3:
				//对应的工程
				//printf("导游路线");
				
				printf("输入两个顶点名称：");
				scanf("%s %s",e1,e2); 
				if(isedg(graph,e1,e2))
					printf("右边");
				else
					printf("五边");
				break; 
			case  4:
				//对应的工程
				printf("游路线中的回路");
				break; 
			case  5:
				//对应的工程
				printf("两个景点间的最短路径和距离");
				break;	
		} 
	}
	return 0;
}
