#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <stdatomic.h>

#define TAMANHO 10

volatile int dados[TAMANHO];
volatile size_t inserir = 0;
volatile size_t remover = 0;
_Atomic uint32_t trava = 0;

struct semaforo;
void sem_inicializar(struct semaforo *s);
void sem_incrementar(struct semaforo *s);
void sem_decrementar(struct semaforo *s);
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

void *produtor(void *arg) {
    int v;

    for (v = 1;; v++) {
        enter_region();
        while (((inserir +1) % TAMANHO) == remover){
            leave_region();
            enter_region();
        };

        printf("Produzindo %d\n", v);

        dados[inserir] = v;
        inserir = (inserir +1) % TAMANHO;
        leave_region();
        
        usleep(500000);
    }

    return NULL;
}

void *consumidor(void *arg){
    for (;;) {
        enter_region();
        while (inserir == remover){
            leave_region();
            enter_region();
        };

        printf("%zu: Consumindo %d\n", (size_t)arg, dados[remover]);

        remover = (remover +1) % TAMANHO;
        leave_region();

       }

    return NULL;
}

int main(void)
{
    pthread_t th1, th2, th3;

    pthread_create(&th1, NULL, produtor, NULL);
    pthread_create(&th2, NULL, consumidor, NULL);
    pthread_create(&th3, NULL, consumidor, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    return 0;
}


