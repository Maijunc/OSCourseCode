#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void* Print(void *vargp)
{
    srand(time(0));
    if(!strcmp((char*)vargp,"GetTime"))
    {
        time_t now_time;
        char curTime[20] = "Current time ";
        for(int i = 1; i <= 5; i++)
        {
            time(&now_time);
            printf("%s%s",curTime,ctime(&now_time));
            sleep(rand() % 5 + 1);
        }
    }
    else
    {
        for(int i = 1; i <= 5; i++)
        {
            printf("%s\n",vargp);
            sleep(rand() % 5 + 1);
        }
    }
}

int main()
{
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1, NULL, Print, (void*)"My name is maitianjun");
    pthread_create(&tid2, NULL, Print, (void*)"My student number is 2022414120219");
    pthread_create(&tid3, NULL, Print, (void*)"GetTime");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    return 0;
}
