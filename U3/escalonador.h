#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <ucontext.h>

#define TAMANHO_FILA 10

typedef void (*RotinaTarefa)(void);
typedef enum { PRONTA, EXECUTANDO, FINALIZADA } EstadoTarefa;

typedef struct {
    int id;
    EstadoTarefa estado;
    ucontext_t contexto;
    RotinaTarefa rotina;
} Tarefa;

typedef struct {
    int contador;
    Tarefa fila[TAMANHO_FILA];
    Tarefa* tarefa_atual;
    ucontext_t contexto;
} Escalonador;

void inicializar_escalonador(Escalonador* escalonador);
void criar_tarefa(RotinaTarefa rotina);
void escalonador();
void yield();

#endif