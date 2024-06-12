#include<stdio.h>
#include<sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define N 1000000

void TestWriteFunc()
{
	char c = 'g';
	int fd = open("testtime.txt",O_WRONLY|O_CREAT|O_TRUNC,0);
	for(int i = 1; i <= N; i++)
		write(fd,&c,1);

	close(fd);
}

void TestReadFunc()
{
        char c;
        int fd = open("testtime.txt",O_RDONLY,0);
        for(int i = 1; i <= N; i++)
                read(fd,&c,1);

        close(fd);
}

int main(void)
{
	float time_use=0;
	struct timeval start;
	struct timeval end;

	gettimeofday(&start,NULL); //gettimeofday(&start,&tz);结果一样

	TestWriteFunc();

	gettimeofday(&end,NULL);

	time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
	printf("write() * 100000 is %.10f us\n",time_use);

	gettimeofday(&start,NULL); //gettimeofday(&start,&tz);结果一样

        TestReadFunc();

        gettimeofday(&end,NULL);

        time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);///微秒
	printf("read() * 100000 is %.10f us\n",time_use);

	return 0;

}
