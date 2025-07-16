#include <stdio.h> 
#include <unistd.h> 

int main() 
{
	printf("PID do processo: %d\n", getpid());
	printf("PID do processo pai: %d\n", getppid());
	
}
