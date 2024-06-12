#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#define N 1024       // 矩阵大小
//#define NUM_THREADS 2  // 线程数量

double a[N][N], b[N][N], result[N][N],suquenResult[N][N];

// 线程参数结构体
typedef struct {
    int start_row;
    int end_row;
} ThreadParam;

// 初始化矩阵
void initializeMatrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 3;  // 随机填充0到99之间的数字
        }
    }
}

// 矩阵乘法函数，由每个线程调用
void *threadMatrixMultiply(void *param) {
    ThreadParam *p = (ThreadParam *) param;
    printf("p->start_row = %d, p->end_row = %d\n",p->start_row,p->end_row);
    for (int i = p->start_row; i < p->end_row; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
            for (int k = 0; k < N; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void sequenceMartrixMultiply()
{
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++) {
            suquenResult[i][j] = 0;
            for (int k = 0; k < N; k++) {
                suquenResult[i][j] += a[i][k] * b[k][j];
            }
        }
}

void showMartix(double matrix[N][N])
{
    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%.0lf ", matrix[i][j]);
        printf("\n");
    }
}

bool Check()
{
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(result[i][j] != suquenResult[i][j])
                return false;
    return true;
}

int main(int argc,char **argv) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    // 将 argv[1] 转换为整数类型
    int NUM_THREADS = atoi(argv[1]);
    pthread_t threads[NUM_THREADS];
    ThreadParam params[NUM_THREADS];
    int rows_per_thread = N / NUM_THREADS;

    srand(time(NULL));
    initializeMatrix(a);
    initializeMatrix(b);

    // 记录开始时间
    clock_t start = clock();

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        params[i].start_row = i * rows_per_thread;
        params[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, threadMatrixMultiply, (void *)&params[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 记录结束时间并计算运行时间
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Matrix multiplication completed in %.2f seconds with %d threads.\n", time_spent, NUM_THREADS);

    sequenceMartrixMultiply();
    if(Check())
        printf("sequenceMartrixMultiply and threadMatrixMultiply got the same result\n");
    else 
	printf("Wrong Ans");
    //    showMartix(result);

    return 0;
}
