#include <stdio.h>
#include "escalonador.h"

void tarefa_0() {
    for (int i = 0; i < 5; ++i) {
        printf("tarefa 0!!!: %d\n", i);
        yield();
    }
}

void tarefa_1() {
    for (int i = 0; i < 5; ++i) {
        printf("tarefa 1!!!: %d\n", i);
        yield();
    }
}

void tarefa_2() {
    for (int i = 0; i < 5; ++i) {
        printf("tarefa 2!!!: %d\n", i);
        yield();
    }
}

int main(void) {
    Escalonador MeuEscalonador;
    inicializar_escalonador(&MeuEscalonador);

    criar_tarefa(tarefa_0);
    criar_tarefa(tarefa_1);
    criar_tarefa(tarefa_2);

    printf("Rodando o escalonador\n");
    escalonador();

    return 0;
}