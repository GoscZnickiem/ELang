section .data
    hello db 'Hello, World!',0x0A  ; napis do wyświetlenia, zakończony nową linią

section .text
    global _start                  ; punkt wejścia programu

_start:
    ; Wywołanie systemowe sys_write
    mov eax, 4                     ; numer wywołania systemowego (sys_write)
    mov ebx, 1                     ; deskryptor pliku (1 = stdout)
    mov ecx, hello                 ; adres napisu do wyświetlenia
    mov edx, 13                    ; długość napisu
    int 0x80                       ; wywołanie systemowe

    ; Wywołanie systemowe sys_exit
    mov eax, 1                     ; numer wywołania systemowego (sys_exit)
    xor ebx, ebx                   ; kod zakończenia (0)
    int 0x80                       ; wywołanie systemowe

