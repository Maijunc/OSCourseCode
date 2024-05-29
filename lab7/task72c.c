#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<string.h>

#define N 1024

int main()
{
    int msqid, pid;
    msqid = msgget(123, 0644|IPC_CREAT);
    char buf[N];
    *((long*)buf) = 1;

    pid = getpid();
    char pidString[10];
    sprintf(pidString, "%d", pid);

    char msg[N - sizeof(long)] = "hello from Process "; //总长度为 前sizeof(long)的标记 + 内容
    strcpy(msg,strcat(msg ,pidString));
    memcpy(buf + sizeof(long), msg, strlen(msg) + 1);
    msgsnd(msqid, buf, strlen(buf + sizeof(long)), 0);
    printf("you send %s to msq %d\n", buf + sizeof(long), msqid);

    return 0;
}
