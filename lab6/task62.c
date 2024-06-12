#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *increase(void *arg);
void *decrease(void *arg);
int cnt = 0;

int main(int argc, char **argv)
{
    unsigned int niters;
    pthread_t tid1, tid2;

    if (argc != 2) {
        printf("usage:%s <niters>\n", argv[0]);
        exit(2);
    }

    niters = atoi(argv[1]);

    pthread_create(&tid1, NULL, increase, (void *)niters);
    pthread_create(&tid2, NULL, decrease, (void *)niters);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if (cnt != 0)
        printf("Error! cnt = %d\n", cnt);
    else
        printf("Correct cnt = %d\n", cnt);

    exit(0);
}

void *increase(void *vargp)
{
    unsigned int i, niters = (unsigned int)vargp;
    for (i = 0; i < niters; i++) {
        cnt++;
    }
    return NULL;
}

void *decrease(void *vargp)
{
    unsigned int i, niters = (unsigned int)vargp;
    for (i = 0; i < niters; i++) {
        cnt--;
    }
    return NULL;
}
