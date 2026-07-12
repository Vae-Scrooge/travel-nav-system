#include "userManager.h"

int loginsys() //1 登录成功  0 退出系统
{
	char username[COLUMNLENGTH];
	char password[COLUMNLENGTH];
	int flag = 0;
	printf("如果忘了用户名和密码，则用户名输入0退出系统，重新注册后再登录！\n");
	while(1)
	{		
		printf("请输入用户名：");
		scanf("%s",username); 
		if(strcmp(username,"0")==0)
		{
			break; 
		}
		printf("\n");
		printf("请输入密码：");
		scanf("%s",password); 
		printf("\n");
		int rs = validateUser(username,password);
		if(rs == 1)
		{
			printf("验证通过!\n");
			flag = 1;
			break;
		}else if(rs == 2)
		{
			printf("用户不存在!\n");
		} else if(rs == 3)
		{
			printf("密码不正确!\n");
		}
	} 
	return flag;	
}

int userRegster() //1 注册成功  0 退出系统 
{
	char username[COLUMNLENGTH];
	char password[COLUMNLENGTH];
	printf("请输入用户名和密码：");
	scanf("%s",username);
	scanf("%s",password);
	printf("\n");
	int x = saveUser(username,password); 
	if(x == 0)
	{ 
		printf("注册失败！\n");
		return 0;
	}
	else
	{
		printf("注册成功！\n");
		return 1;
	 } 
} 
int saveUser(char username[COLUMNLENGTH],char password[COLUMNLENGTH])
{
	FILE *fp;
	fp = fopen("user.txt","a");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0;
	}
	fprintf(fp,"%s %s\n",username,password);
	fclose(fp);
	printf("保存成功！\n");
	return 1;
}

int validateUser(char username[COLUMNLENGTH],char password[COLUMNLENGTH]) //返回 1 验证通过  2 用户不存在 3 密码不正确  
{
	char fusername[COLUMNLENGTH] = "";
	char fpassword[COLUMNLENGTH] = ""; 
	FILE *fp;
	fp = fopen("user.txt","r");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return;
	}
	while(fscanf(fp,"%s %s\n",fusername,fpassword)!=EOF){
		if(strcmp(username,fusername) == 0)
		{
			break;
		}
	}
	fclose(fp);
	int flag = 1; 
	if(strcmp(username,fusername) !=0)
	{
		flag = 2; 
	}else if(strcmp(password,fpassword) !=0)
	{
		flag = 3; 
	}
	return flag; 
}



int usermenu()
{
	int opt = 0;  
	char buf[100];
	int flag = 0;
	
	while(1)
	{
		printf("请输入您的选择：\n");
		printf("1 登录\n\n");
		printf("2 注册\n\n");
		printf("0 退出系统\n\n");
		scanf("%s",buf);
		if(!isdigit(buf[0])){
			printf("你输入的不是数字，请重新输入你的选择：");
			scanf("%s",buf);
		}else if(atoi(&buf[0])<0||atoi(&buf[0])>2)
		{
			printf("你输入的数字有错误，请重新输入你的选择：");
			scanf("%s",buf);
		}else{				
			opt = atoi(buf);
		}
		switch(opt)
		{
			case 1:
				if(loginsys()==1) //1 登录成功  0 退出系统 
					flag = 1;
				break;
			case 2:
				if(userRegster()==1)  //1 注册成功  0 退出系统 
					flag = 2;
		}
		
		if(flag == 0 || flag == 1)
		{
			break;
		} 			
	}
	return flag;
}
	
