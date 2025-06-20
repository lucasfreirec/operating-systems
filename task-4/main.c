#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid < 0) return -1;
    
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);

        execlp("/usr/bin/ls", "ls", "-l", NULL);

    } else {
        char buf[1000] = {0};
        close(fd[1]);
        
        while (read(fd[0], buf, sizeof buf - 1) > 0) {
            printf("Lido: %s\n", buf);
        }
        
        wait(NULL);
    }

    return 0;
}
