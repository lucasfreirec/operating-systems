.global _start
_start:
    mov $42, %rax

    mov $60, %rax
    mov $0, %rdi
    syscall