#include<stdio.h>
#include<string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct _subject {
        char sno[20];	   //学号
        char name[20];   //姓名
        float chinese;	   //语文成绩
        float math;		//数学成绩
        float english;	   //英语成绩
}  subject;

const int N = 5;

int main()
{
	subject mark[N];
	int fd;
	fd = open("data.txt",O_WRONLY|O_CREAT|O_TRUNC,0);
	char enter = '\n',space = ' ';
	for(int i = 0; i < N; i++)
	{
		scanf("%s",mark[i].sno);
		scanf("%s",mark[i].name);
		scanf("%f ",&mark[i].chinese);
		scanf("%f ",&mark[i].math);
		scanf("%f",&mark[i].english);

	    write(fd,&mark[i],sizeof(mark[i]));

	    char chinese_str[10];
	    char math_str[10];
	    char english_str[10];

	    snprintf(chinese_str, sizeof(chinese_str), "%.1f", mark[i].chinese);
	    snprintf(math_str, sizeof(math_str), "%.1f", mark[i].math);
	    snprintf(english_str, sizeof(english_str), "%.1f", mark[i].english);
	    //write(fd,mark[i].sno,strlen(mark[i].sno));
            //write(fd,&space,1);
            //write(fd,mark[i].name,strlen(mark[i].name));
 	    //write(fd,&space,1);
            //write(fd,chinese_str,4);
            //write(fd,&space,1);
            //write(fd,math_str,4);
            //write(fd,&space,1);
            //write(fd,english_str,4);
            //write(fd,&enter,1);
	}
	close(fd);
	fd = open("data.txt",O_RDONLY,0);
	subject sub[N];
	printf("1、3、5条学生数据如下：\n");
	for(int i = 0 ; i < N; i++)
	{
		read(fd,&sub[i],sizeof(sub[i]));
		if(i % 2 == 0)
		{
			printf("%s %s% .1f% .1f% .1f\n",sub[i].sno,sub[i].name,sub[i].
					chinese,sub[i].math,sub[i].english);
		}
	}
	close(fd);
	return 0;
}
