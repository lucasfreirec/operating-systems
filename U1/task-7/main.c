#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>

#define FALSE 0
#define TRUE  1
#define N 2     /* number of processes */

volatile int turn;                            /* whose turn is it? */
volatile int interested[N];                   /* all values initially 0 (FALSE) */

uint64_t valor = 0;

void enter_region(int process)       /* process is 0 or 1 */
{
    int other;                  /* number of the other process */

    other = 1 - process;        /* the opposite of process */
    interested[process] = TRUE; /* show that you are interested */
    turn = process;             /* set flag */
    atomic_thread_fence(memory_order_seq_cst);
    while (turn == process && interested[other] == TRUE) /* null statement */ ;
}


void leave_region(int process)       /* process: who is leaving */
{
    interested[process] = FALSE;    /* indicate departure from critical region */
}

void *thread(void *arg)
{
    int proc = (size_t)arg;
    size_t i = 1000000;

    while (i--) {
        enter_region(proc);
        valor++;
        leave_region(proc);
    }
    return NULL;
}

int main(void)
{
    pthread_t th1, th2;

    pthread_create(&th1, NULL, thread, (void *) 0);
    pthread_create(&th2, NULL, thread, (void *) 1);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("Valor: %lu\n", valor);

    return 0;
}

