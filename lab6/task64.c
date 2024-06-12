#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAXTHREADS 32

void *sum(void *vargp);

unsigned long long psum64[MAXTHREADS]; /*每个线程汇总部分和*/
unsigned long long nelems_per_thread; /*每个线程汇总元素个数*/

void *sum(void* vargp)
{
    int myid = *((int*) vargp); /* 获得线程ID号 */
    unsigned long long begin = myid * nelems_per_thread; /* 首元素序号 */
    unsigned long long end = begin + nelems_per_thread; /* 末元素序号 */
    unsigned long long i, lsum = 0;

    for(i = begin; i < end; i++)
        lsum += i * i;
    psum64[myid] = lsum;
    return NULL;
}

int main(int argc, char **argv)
{
    unsigned long long i, nelems, log_nelems, nthreads, result = 0;
    pthread_t tid[MAXTHREADS];
    int myid[MAXTHREADS];

    /*获取输入参数*/
    if(argc != 3)
    {
        printf("Usage: %s <nthreads><log_nelems>\n", argv[0]);
        exit(0);
    }

    nthreads = atoi(argv[1]);
    log_nelems = atoi(argv[2]);
    nelems = (1LL << log_nelems); /* ILL是64位的long long整数常量 */
    nelems_per_thread = nelems / nthreads;

    /*创建对等线程并等待它们结束*/
    for(int i = 0; i < nthreads; i++)
    {
        myid[i] = i;
        pthread_create(&tid[i], NULL, sum, &myid[i]);
    }
    for(int i = 0; i < nthreads; i++)
        pthread_join(tid[i], NULL);

    /*将每个线程汇总的部分和加起来*/
    for(int i = 0; i < nthreads; i++)
        result += psum64[i];

    /*检查最终结果*/
    if(result == ((2 * (nelems - 1) + 1) * (nelems) * (nelems - 1) / 6))
        printf("Correct: result = %ld\n", result);
    else
        printf("Error: result = %ld\n", result);

    exit(0);

}
