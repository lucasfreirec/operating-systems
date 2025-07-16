#include "escalonador.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_PILHA 65546 // 64 KB
#define TAMANHO_FILA 10

jmp_buf contexto_escalonador;
Tarefa* tarefa_atual;
static Tarefa fila[TAMANHO_FILA];
static int contador;


void criar_tarefa(RotinaTarefa rotina)
{
    if (contador >= TAMANHO_FILA) {
        printf("Error: Maximum number of tasks reached.\n");
        return;
    }

    Tarefa* nova_tarefa = &fila[contador];

    nova_tarefa->id = contador;
    nova_tarefa->estado = PRONTA;
    nova_tarefa->rotina = rotina;

    // mmap(endereço, tamanho, permissao, tipo de mapeamento, fd, offset)
    // oq o man diz:
    // MAP_ANONYMOUS: The mapping is not backed by any file; its contents are initialized to zero.  The fd argument is ignored
    // The offset argument should be zero
    nova_tarefa->pilha = mmap(NULL, TAMANHO_PILHA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    //salva o contexto atual da CPU (serve para inicializar tudo, pois ainda so contem lixo)
    setjmp(nova_tarefa->contexto);
    
    // aponta o ponteiro de pilha para o topo da pilha de tarefa
    ((unsigned long*)nova_tarefa->contexto)[6] = (unsigned long)(nova_tarefa->pilha + TAMANHO_PILHA);
    
    // aponta o ponteiro de instrucao para a funcao que a tarefa deve executar
    ((unsigned long*)nova_tarefa->contexto)[7] = (unsigned long)wrapper_tarefa;

    contador++;

    printf("Tarefa %d criada.\n", nova_tarefa->id);
}

void wrapper_tarefa() {
    // chama a funcao q sera executada
    tarefa_atual->rotina();

    // After the routine finishes, perform cleanup
    printf("A tarefa %d foi finalizada.\n", tarefa_atual->id);
    tarefa_atual->estado = FINALIZADA;

    // Yield para o escalonador
    longjmp(contexto_escalonador, 1);
}

// round-robin cooperativo
static Tarefa* encontrar_tarefa(void) {
    // determina onde comecar a busca
    int start_index = (tarefa_atual == NULL) ? 0 : (tarefa_atual->id + 1);

    for (int i = 0; i < contador; ++i) {
        // busca circular
        int index = (start_index + i) % contador;

        if (fila[index].estado == PRONTA) {
            // retorna a primeira tarefa pronta para executar
            return &fila[index];
        }
    }

    // n tem tarefa pronta, retorna VAZIO
    return NULL;
}

void inicializar_escalonador() {
    contador = 0;

    tarefa_atual = NULL;

    for (int i = 0; i < TAMANHO_FILA; ++i) {
        fila[i].estado = FINALIZADA;
    }

    printf("Escalonador iniciado.\n");
}

void escalonador() {
    // ponto de retorno das tarefas
    if (setjmp(contexto_escalonador) == 0) { }

    
    while (1) {
        Tarefa* prox_tarefa = encontrar_tarefa();
        
        if (prox_tarefa) {
            tarefa_atual = prox_tarefa;
            tarefa_atual->estado = EXECUTANDO;

            longjmp(tarefa_atual->contexto, 1);
        } else {
            printf("Todas as tarefas foram finalizadas.\n");
            break;
        }
    }
}

void yield() {
    // salva contexto atual
    if (setjmp(tarefa_atual->contexto) == 0) {

        tarefa_atual->estado = PRONTA;
        // volta pro escalonador
        longjmp(contexto_escalonador, 1);
    }
    // quando o escalonador dar o longjmp, o codigo segue a execucao dps do if
}
