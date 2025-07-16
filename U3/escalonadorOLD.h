#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <setjmp.h>


typedef void (*RotinaTarefa)(void);
typedef enum { PRONTA, EXECUTANDO, FINALIZADA } EstadoTarefa;

typedef struct {
    int id;
    EstadoTarefa estado;
    jmp_buf contexto;
    char *pilha;
    RotinaTarefa rotina;
} Tarefa;


void criar_tarefa(RotinaTarefa rotina);
void wrapper_tarefa();
void inicializar_escalonador();
void escalonador();
void yield();

#endif