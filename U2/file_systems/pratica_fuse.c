#define FUSE_USE_VERSION 31
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fuse.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

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

void *map;
struct super *sb;
struct arquivo *table;

static int mifs_getattr(const char *path, struct stat *stbuf,
                        struct fuse_file_info *fi)
{
        //if (strcmp(path, "/teste.txt") != 0) {
        //        return -ENOENT;
        //}

        for (uint64_t i = 0; i < sb->nr_arquivos; i++) {
            if (strcmp(path+1, table[i].nome) == 0) {
                *stbuf = (struct stat) {
                .st_mode = S_IFREG | 0444,
                .st_nlink = 1,
                .st_size = table[i].tamanho,
            }
        return 0;
            }
        }

        return -ENOENT;
        
}

int mifs_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
        for (uint64_t i = 0; i < sb->nr_arquivos; i++) {
        if (strcmp(path+1, table[i].nome) == 0) {
                if (table[i].bloco_inicial )
                memcpy(buf, );

        }
        return 0;
    }
        
    return -ENOENT;
}
//static int mifs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
//off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags)
//{
//}
{
        //if (strcmp(path, "/teste.txt") != 0) {
        //        return -ENOENT;
        //}

        for (uint64_t i = 0; i < sb->nr_arquivos; i++) {
            if (strcmp(path+1, table[i].nome) == 0) {
                *stbuf = (struct stat) {
                .st_mode = S_IFREG | 0444,
                .st_nlink = 1,
                .st_size = table[i].tamanho,
        };
        return 0;
            }
        }

        return -ENOENT;
        
}

int main(int argc, char **argv)
{
        if (argc != 3) { 
                fprintf(stderr, "Uso: %s <imagem> <diretorio>\n", argv[0]);
                exit(1);
        }

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

    map= mmap(NULL, image_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if(map == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    sb = (struct super *)map;

    if (sb->assinatura != 0x5346494D) {
        fprintf(stderr, "Invalid MIFS signature\n");
        exit(1);
    }

    uint32_t block_size = sb->tamanho_bloco;
    table = (struct arquivo *)((uint8_t *)map + block_size);

        static const struct fuse_operations mifs_ops = {
                .getattr = mifs_getattr,
        };
        struct fuse_args args = FUSE_ARGS_INIT(0, NULL);
        fuse_opt_add_arg(&args, argv[0]);
        fuse_opt_add_arg(&args, argv[2]);

        return fuse_main(args.argc, args.argv, &mifs_ops, NULL);
}