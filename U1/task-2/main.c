#include <stdio.h>
#include <unistd.h>

int main() {
   //printf("Hello, World!\n");
   write(1, "Hello, World!\n", 15);

   return 0;
}
