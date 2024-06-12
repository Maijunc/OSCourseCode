#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h> 

int main()
{
	int p1 = 0,p11 = 0,p12 = 0,p121 = 0,p122 = 0;
	p11 = fork();
	if(p11 == 0)
	{
		time_t curTime;
		time(&curTime);
		printf("p11:当前时间是%s",ctime(&curTime));
		exit(0);
	}
	p12 = fork();
	if(p12 == 0)
	{
		p121 = fork();
		if(p121 == 0)
		{
			printf("p121:我的学号是xxx\n");
			exit(0);
		}
		p122 = fork();
		if(p122 == 0)
		{
			printf("p122:我的姓名是xxx\n");
			exit(0);
		}
		printf("p12: I am young brother process\n");
		exit(0);
	}
	printf("p1: I am father process\n");
	int status = 0;
	pid_t wpid;
	while ((wpid = wait(&status)) > 0);
	return 0;
}
