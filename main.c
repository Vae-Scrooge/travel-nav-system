
#include "main.h"
#include <ctype.h>

int main()
{
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
				printf("创建景区景点图");
				break; 
			case  2:
				//对应的工程
				printf("景区景点分布图");
				break; 
			case  3:
				//对应的工程
				printf("导游路线");
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
