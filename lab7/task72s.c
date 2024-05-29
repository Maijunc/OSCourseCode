#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<assert.h>

#define N 1024

int msqid;

void close_sig_handler(int sig){
    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}

int main()
{
    msqid = msgget(123, 0644|IPC_CREAT);
    char buf[N];

    struct sigaction sa;
    sa.sa_handler = close_sig_handler;
    assert(sigaction(SIGINT,&sa,NULL)!=-1);//捕获到信号sig，使用sa中规定的方法处理

    while(1)
    {
        msgrcv(msqid, buf, sizeof(buf), 1, 0);
        printf("receipt of hello from maitianjun's server to Process %s\n",buf + sizeof(long) + sizeof("hello from Process"));
    }
    
}
