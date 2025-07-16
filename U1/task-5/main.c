#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>

uint64_t valor = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *arg)
{
    size_t i =1000000;

    while (i--) {
        pthread_mutex_lock(&mutex);
        valor++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(void)
{
    pthread_t th1, th2;

    pthread_create(&th1, NULL, thread, NULL);
    pthread_create(&th2, NULL, thread, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("Valor: %lu\n", valor);

    return 0;
}