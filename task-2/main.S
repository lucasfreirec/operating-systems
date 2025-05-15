.text
.global _start

.section .rodata
   msg: .ascii "Hello, World!\n"
   MSG_SIZE = . - msg

.section .text

_start:
  mov $1, %rax
  mov $1, %rdi
  leaq msg(%rip), %rsi
  movq $MSG_SIZE, %rdx
  syscall

  movq $60, %rax
  mov $0, %rdi
  syscall
