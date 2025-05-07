#include <stdio.h> 
#include <unistd.h> 

int main() 
{
	int* a;
	int pid = fork();

	if (pid==0){
	printf("\n\nProcesso filho\n");
	//printf("PID filho (fork): %d\n",pid);
	//printf("PID: %d\n", getpid());
	*a = 20;
	//sleep(1);
	printf("%d\n", *a);
	for(int i=0; i<20; ++i){
		*a = *a +1;
		printf("%d\n", *a);
	}
	
	} else {
	printf("\n\nProcesso pai\n");
	//printf("PID pai (fork): %d\n",pid);
	//printf("PID filho: %d\n", getppid());
	*a = 10;
		for(int i=0; i<20; ++i){
		printf("%d\n", *a);
	}
	}
	
	//printf("PID: %d\n", getpid());
	//printf("PPID: %d\n", getppid());

}
