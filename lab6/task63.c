#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 20 //缓冲区大小
#define K 5 //生产者线程数量
#define M 3 //消费者线程数量
#define RANDOM 10 //随机数范围

int buffer[N];
sem_t mutex, full, empty, producerAllDone;
int sum_producer = 0, sum_consumer = 0;
int Head,Tail; //buffer 头和尾
int consumerDone;


void* producer(void* vargp)
{
    int id = *(int *)vargp;
    srand(time(NULL));

    sem_wait(&empty);
    sem_wait(&mutex);

    for(int i = 0; i < N / K; i++)
    {
        int item = (rand() % (RANDOM + 1)) + 1; //产生随机数
        buffer[Tail] = item;
        Tail = (Tail + 1) % N;
        sum_producer += item;
        printf("producer %d produce %d\n", id, item);
    }

    sem_post(&mutex);
    sem_post(&full);
}

void* consumer(void* vargp)
{
    int id = *(int *)vargp;
    srand(time(NULL));

    sem_wait(&full);
    sem_wait(&mutex);

    //表示不空 Taile == Head表示空
    while(Tail != Head)
    {
        int item = buffer[Head];
        sum_consumer += item;
        Head = (Head + 1) % N;
        printf("Consumer %d consumed %d\n",id, item);
        if(consumerDone == M - 1)
            sem_wait(&producerAllDone);
    }
    consumerDone++;

    sem_post(&mutex);
    sem_post(&empty);
}


int main()
{
    pthread_t producer_threads[K], consumer_threads[M];

    int producer_id[K], consumer_id[M];

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);
    sem_init(&producerAllDone, 0, N);

    for(int i = 0; i < K; i++)
    {
        producer_id[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, (void*)&producer_id[i]);
    }

    for(int i = 0; i < M; i++)
    {
        consumer_id[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, (void*)&consumer_id[i]);
    }

    for(int i = 0; i < K; i++)
        pthread_join(producer_threads[i], NULL);

    sem_post(&producerAllDone);

    for(int i = 0; i < M; i++)
        pthread_join(consumer_threads[i], NULL);

    printf("Total sum produced: %d\n", sum_producer);
    printf("Total sum consumed: %d\n", sum_consumer);
    if(sum_producer == sum_consumer)
        printf("Sums match!\n");
    else
        printf("Sums do not match!\n");

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&producerAllDone);

    return 0;
}
