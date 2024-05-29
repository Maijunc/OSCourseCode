#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 1024

int main()
{

    pid_t p1, p2;
    int fds[2];
    pipe(fds);
    char buffer_w[N] = {0}, buffer_r[N] = {0};

    p1 = fork();
    if (p1 == 0) // 子进程p1
    {
        close(fds[0]); // 关闭读端口
        strcpy(buffer_w, "Child P1 is sending My name maitianjun!\n");
        int count = write(fds[1], buffer_w, strlen(buffer_w));
        printf("Child p1 write %d bytes\n", count);
        exit(EXIT_SUCCESS);
    }

    p2 = fork();
    if (p2 == 0) // 子进程p2
    {
        close(fds[0]); // 关闭读端口
        waitpid(p1, NULL, 0);

        time_t curTime;
        time(&curTime);
        char* timeStr = ctime(&curTime);

        strcpy(buffer_w, "Child P2 is sending the current time ");
        strcat(buffer_w, timeStr);
        buffer_w[strlen(buffer_w) - 1] = '!';
        strcat(buffer_w, "\n");

        int count = write(fds[1], buffer_w, strlen(buffer_w));
        printf("Child p2 write %d bytes\n", count);
        exit(EXIT_SUCCESS);
    }

    if (p1 > 0 && p2 > 0) // 父进程
    {
        close(fds[1]); // 关闭写端口

        waitpid(p1, NULL, 0);
        waitpid(p2, NULL, 0);
        int count = read(fds[0], buffer_r, N);
        printf("Parent read %d bytes from p1 and p2:\n", count);
        printf("%s", buffer_r);

        close(fds[0]); // 关闭读端口
    }

    return 0;
}
