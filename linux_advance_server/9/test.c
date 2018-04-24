#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#define NP          3
 
void *Producer(void *arg)
{
    pthread_t   tid;
    tid = pthread_self();
    printf("%u\n", (unsigned int)tid);
    int index = (int)arg;
    printf("%d\n", index);
    return NULL;
}
 
int main()
{
    pthread_t idP, idC;
    int index;
    for (index = 0; index < NP; index++)
    {
        pthread_create(&idP, NULL, Producer, (void*)index);
    }
    pthread_exit(NULL);
}
