#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
     	return 1;
    }
    int N = atoi(argv[1]);
    struct timeval ts,te;
    gettimeofday(&ts, NULL);

    for(int i = 1; i <= N; i++)
    {
	    pid_t pid = fork();
	    if(pid == 0)
		    exit(0);
    }

    gettimeofday(&te, NULL);

    printf("%d times fork call costs %lld us\n",N,te.tv_usec - ts.tv_usec);

    gettimeofday(&ts, NULL);

    for(int i = 1; i <= N; i++)
    {
	pthread_t thread;
	pthread_create(&thread, NULL, NULL, NULL);
    }

    gettimeofday(&te, NULL);

    printf("%d times pthread call costs %lld us\n",N,te.tv_usec - ts.tv_usec);
}
