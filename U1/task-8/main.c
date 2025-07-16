#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <linux/futex.h>
#include <sys/syscall.h>
 #include <unistd.h>

uint64_t valor = 0;
_Atomic uint32_t trava = 0;

void enter_region(void)       
{
    uint32_t v = 0;

    if (atomic_compare_exchange_strong(&trava, &v, 1)) {
        return;
    }

    do {
        if (v == 2 || atomic_compare_exchange_strong(&trava, &v, 2)) {
            syscall(SYS_futex, &trava, FUTEX_WAIT, 2);
        }
        v = 0;

    } while (!atomic_compare_exchange_strong(&trava, &v, 2));

}

void leave_region(void){       
    uint32_t v = atomic_fetch_sub(&trava, 1);
    
    if (v != 1) {
        atomic_store(&trava, 0);   
        syscall(SYS_futex, &trava, FUTEX_WAKE, 1);
    }
}

void *thread(void *arg)
{
    size_t i = 1000000;

    while (i--) {
        enter_region();
        valor++;
        leave_region();
    }

    return NULL;
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

