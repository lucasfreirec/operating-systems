#include <stdio.h>
#include <unistd.h>
#include "stdint.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

struct super {
    uint32_t assinatura;
    uint32_t tamanho_bloco;
    uint64_t nr_arquivos;
} __attribute__ ((packed));

struct arquivo {
    char nome[20];
    uint32_t bloco_inicial;
    uint64_t tamanho;
} __attribute__((packed));

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    struct stat st;
    
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(1);
    }

    size_t image_size = st.st_size;
    void *map=mmap(NULL, image_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if(map == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    struct super *sb = (struct super *)map;

    if (sb->assinatura != 0x5346494D) {
        fprintf(stderr, "Invalid MIFS signature\n");
        exit(1);
    }

    uint32_t block_size = sb->tamanho_bloco;
    struct arquivo *table = (struct arquivo *)((uint8_t *)map + block_size);

    if (argc == 2) {
        for (uint64_t i = 0; i < sb->nr_arquivos; i++) {
            printf("%.*s\n", 20, table[i].nome);
            printf("%lu\n", (unsigned long)table[i].tamanho);
        }
        exit(0);
    }

    if (argc == 3) {
        int erro;
            for (uint64_t i = 0; i < sb->nr_arquivos; i++) {
            if (strcmp(argv[2], table[i].nome) == 0) {
                printf("%.*s\n", 20, table[i].nome);
                printf("%lu\n", (unsigned long)table[i].bloco_inicial);
                printf("%lu\n", (unsigned long)table[i].tamanho);
            }
        }
        exit(0);
    }
    return 0;
}