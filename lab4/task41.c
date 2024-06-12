#include<stdio.h>
#include<stdbool.h>

#define MAXSIZE 100

int main()
{
    FILE* fp1;FILE* fp2;
    fp1 = fopen("student.txt", "r");
    if (fp1 == NULL)  // 文件指针赋值失败
    {
        printf("文件读取失败");
	return 1;
    }
    fp2 = fopen("csStudent.txt", "w");
    if (fp2 == NULL)  // 文件指针赋值失败
    {
        printf("文件读取失败");
	return 1;
    }
    char buf[MAXSIZE];
    char* ans = "计算机与网络安全学院\0";
    while(fgets(buf,MAXSIZE,fp1))
    {
	int cnt = 0;
	int st = -1,ed = -1;
	for(int i = 0; buf[i] != '\n'; i++)
	{
	    if(buf[i] == ':')
		cnt++;
	    if(cnt == 2 && st == -1)
            {
		st = i + 1;
	    }else if(cnt == 3 && ed == -1) 
	    {
		ed = i - 1;
		break;
	    }
	}
	int k = 0;
	bool flag = true;
	if(st == -1 || ed == -1)
	    continue;
	for(int i = st;i <= ed;i += 2)
        {
	    if(buf[i] + buf[i+1] != ans[k] + ans[k+1])
            {
		flag = false;
                break;
            }
	    k += 2;
        }

	int len = 0;
	for(int i = 0 ; ans[i] != '\0'; i++)
	    len++;

	if(flag == true && k == len)
	{
	    fputs(buf,fp2);
	}
    }
    printf("操作成功\n");

    fclose(fp1);
    fclose(fp2);

    return 0;
}
