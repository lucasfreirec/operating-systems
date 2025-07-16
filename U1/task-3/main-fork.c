#include <stdio.h> 
#include <unistd.h> 

int main() 
{

	int pid = fork();

	if (pid==0){
		printf("\nProcesso filho\n");

		printf("PID: %d\n", getpid());
		printf("PID do pai: %d\n", getppid());
	} 
	else {
		printf("\nProcesso pai\n");
		printf("PID: %d\n", getpid());
	}
	
}
