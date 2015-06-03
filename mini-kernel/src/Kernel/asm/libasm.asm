GLOBAL      _read_msw,_lidt

GLOBAL      _timertick_handler
GLOBAL      _keyboard_handler
GLOBAL      _serial_handler
GLOBAL		_syscall_handler

GLOBAL 		_syscall

GLOBAL      _mask_pic_2,_mask_pic_1,_cli,_sti
GLOBAL      _outb
GLOBAL      _inb
GLOBAL		_hlt

EXTERN      timertick_handler
EXTERN      keyboard_handler
EXTERN      serial_handler
EXTERN		syscall_handler

SECTION .text
%macro pushaq 0
    push rax      ;save current rax
    push rbx      ;save current rbx
    push rcx      ;save current rcx
    push rdx      ;save current rdx
    push rbp      ;save current rbp
    push rdi       ;save current rdi
    push rsi       ;save current rsi
    push r8        ;save current r8
    push r9        ;save current r9
    push r10      ;save current r10
    push r11      ;save current r11
    push r12      ;save current r12
    push r13      ;save current r13
    push r14      ;save current r14
    push r15      ;save current r15
%endmacro

%macro popaq 0
        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rsi
        pop rdi
        pop rbp
        pop rdx
        pop rcx
        pop rbx
        pop rax
%endmacro

_outb:
    push    rbp
    mov     rbp, rsp
    mov     rax, rdi
    mov     rdx, rsi
    mov     rdi, [rbp+8]
    mov     rsi, [rbp+12]
    out     dx, al
    mov     rsp, rbp
    pop     rbp
    ret

_inb:
    push    rbp
    mov     rbp, rsp
    mov     rdx, rsi
    mov     rdx, [rbp+8]
    xor     rax, rax
    in      al, dx
    mov     rsp, rbp
    pop     rbp
	ret

_hlt:
	hlt
	ret

_cli:
    cli
    ret

_sti:
    sti
    ret

_mask_pic_1:
    push    rbp
    mov     rbp, rsp
    mov     ax, [rbp+8]          ; ax = 16 bit mask
    out     21h,al
    pop     rbp
    retn

_mask_pic_2:
    push    rbp
    mov     rbp, rsp
    mov     ax, [rbp+8]          ; ax = 16 bit mask
    out     0A1h,al
    pop     rbp
    retn

_timertick_handler:                 ; INT 0x08 Handler (Timertick)

    pushaq                       
    mov     ax, 10h              
    call    timertick_handler
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popaq                            

    iret

_keyboard_handler:                  ; INT 0x09 Handler (Keyboard)
    pushaq
    mov     ax, 10h
    call    keyboard_handler
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popaq
    iret

_syscall_handler:					; INT 0x80 Handler (Syscall)
    
    push    rbp
	call    syscall_handler
    pop     rbp
	push	rax
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
	pop		rax

    iret

_syscall:
    push    rbp
    mov     rbp, rsp
    pushaq
    pushf

    mov     rdi, [rbp+28]
    mov     rsi, [rbp+24]
    mov     rdx, [rbp+20]
    mov     rcx, [rbp+16]
    mov     rbx, [rbp+12]
    mov     rax, [rbp+8]

    int     80h

    popf
    popaq

    mov     rsp, rbp
    pop     rbp
    ret