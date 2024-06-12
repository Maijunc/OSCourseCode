#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>

#define MAXLINE 50
#define MAXARGS 10
#define MAXPIPE 20

int parseline(char *buf,char **argv,int *argc)
{
    char *delim; //指向第1个分隔符
    *argc = 0; //字符串数组argv中参数的个数
    int bg;

    buf[strlen(buf)-1] = ' '; //用空格替换行末换行符
    while(*buf && (*buf == ' ')) //删除行首空格
        buf++;

    //创建argv数组
    while((delim = strchr(buf,' ')))
    {
        //将字符串放入argv中
        argv[(*argc)++] = buf;
        *delim = '\0';

        buf = delim + 1;
        while(*buf && (*buf == ' ')) //忽略空格，找到下一个字符串的开头
            buf++;
    }
    //delim 最后一次匹配到行末空格
    argv[*argc] = NULL;

    if(argc == 0) //忽略空行
        return 1;

    //如果最后一个参数是&，表示应该在后台运行这个程序
    if(bg = (*argv[(*argc) - 1] == '&') != 0)
        argv[--(*argc)] = NULL;

    return bg;
}

int builtin_command(char **argv)
{
    if(!strcmp(argv[0],"exit") || !strcmp(argv[0],"logout"))
        exit(0);
    if(!strcmp(argv[0],"&")) //忽略以&开始的命令串`
        return 1;

    return 0;
}

void RunCommand(char **argv,int argc,int infd,int outfd)
{
    pid_t pid;
    if((pid = fork()) == 0)
    {
        //让 0 指向 infd 所指的
        if (infd != STDIN_FILENO) {
            dup2(infd, STDIN_FILENO);
            close(infd);
        }
        //让 1 指向 outfd 所指的
        if (outfd != STDOUT_FILENO) {
            dup2(outfd, STDOUT_FILENO);
            close(outfd);
        }

        //执行命令
        if(execvp(argv[0],argv) < 0)
        {
            printf("%s:Command not found.\n",argv[0]);
            exit(0);
        }
    }
}

void execute(char *cmdline)
{
    char *argv[MAXARGS];  //参数表 内含多个参数
    char buf[MAXLINE];   //保存修改后的命令行
    int argc = 0;
    int bg; //是否在后台执行
    pid_t pid; //子进程pid

    strcpy(buf,cmdline);
    bg = parseline(buf,argv,&argc); //解析命令行，argc得到argv的长度
    if(argv[0] == NULL)
        return;   //如果第一个参数为空，忽略命令

    //先处理参数表
    int infd = STDIN_FILENO, outfd = STDOUT_FILENO;
    if (!builtin_command(argv))
    {
        if (bg)
        {
            if ((pid = fork()) == 0)
            {
                setpgid(0, 0);
                RunCommand(argv, argc, STDIN_FILENO, STDOUT_FILENO);
                exit(0);
            }
            else if (pid < 0)
            {
                perror("fork");
                return;
            }
            printf("%d\n", pid);
            return;
        }

        for (int i = 0; i < argc; i++)
        {
            if (!strcmp(argv[i], ">"))
            {
                argv[i] = NULL;
                outfd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (outfd == -1)
                {
                    perror("open");
                    return;
                }
                break;
            }
        }
    }

    int pipePos[MAXPIPE];
    int pipeCnt = 0;
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
            pipePos[pipeCnt++] = i;
    }

    //如果没有管道
    if(!pipeCnt)
    {
        RunCommand(argv, argc, infd, outfd);
        wait(NULL);
    }
    else
    {
        int fds[2];
        char* sArgv[MAXARGS];
        pipe(fds);
        for(int i = 0, j = 0; i <= pipeCnt; i++)
        {
            // j 到 pipePos[i] - 1为一个指令
            int len = 0;

            if(i != pipeCnt)
            {
                for(int k = j; k < pipePos[i]; k++)
                {
                    sArgv[len++] = argv[k];
                }
                len++; //留作NULL
            }
            else
            {
                for(int k = j; k < argc; k++)
                {
                    sArgv[len++] = argv[k];
                }
                len++; //留作NULL
            }


            sArgv[len - 1] = NULL;
            //A的输入是stdin 输出是 fds[0]
            //B的输入是fds[1] 输出是fds[0]
            //C的输入是fds[1] 输出是outfd
            if(i == 0)
                RunCommand(sArgv,len,infd,fds[1]);
            else if(i == pipeCnt)
                RunCommand(sArgv,len,fds[0],outfd);
            else
                RunCommand(sArgv,len,fds[0],fds[1]);

            j = pipePos[i] + 1;
        }
    }

    return;
}


int main()
{
    char cmdline[MAXLINE];
    while(1)
    {
        printf("%% ");
        fgets(cmdline,MAXLINE,stdin);

        if(feof(stdin))
            exit(0);

        execute(cmdline); //执行命令
    }
}
