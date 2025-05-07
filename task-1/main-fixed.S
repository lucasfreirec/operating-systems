.global _start
_start:
    mov $42, %rax

    movq $60, %rax
    mov %rdi, %rdi
    syscall