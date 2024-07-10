.section .data
hello:
    .ascii "Hello, World!\n"

.section .text
    .global _start

_start:
    # Wywołanie systemowe sys_write
    mov $4, %eax                  # numer wywołania systemowego (sys_write)
    mov $1, %ebx                  # deskryptor pliku (1 = stdout)
    mov $hello, %ecx              # adres napisu do wyświetlenia
    mov $13, %edx                 # długość napisu
    int $0x80                     # wywołanie systemowe

    # Wywołanie systemowe sys_exit
    mov $1, %eax                  # numer wywołania systemowego (sys_exit)
    xor %ebx, %ebx                # kod zakończenia (0)
    int $0x80                     # wywołanie systemowe

