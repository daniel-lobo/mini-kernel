global _enable_pic
global _set_handlers
global _outb
global _inb
global _hlt

extern pit_handler
extern keyboard_handler
extern syscall_handler
extern write_handler
extern read_handler
extern rtc_handler
extern rtc_set_handler
extern ss_test_handler
extern ss_set_handler
extern format_set_handler
extern ss_status_handler

_outb:
    push    rbp
    mov     rbp, rsp
    mov     rax, rdi
    mov     rdx, rsi
    out     dx, al
    mov     rsp, rbp
    pop     rbp
    ret

_inb:
    push    rbp
    mov     rbp, rsp
    mov     rdx, rdi
    xor     rax, rax
    in      al, dx
    mov     rsp, rbp
    pop     rbp
	ret

create_gate:
    push rdi
    push rax
    shl rdi, 4                   ; quickly multiply rdi by 16
    stosw                                   ; store the low word (15..0)
    shr rax, 16
    add rdi, 4                   ; skip the gate marker
    stosw                                   ; store the high word (31..16)
    shr rax, 16
    stosd                                  ; store the high dword (63..32)
    pop rax
    pop rdi
    ret

IDTR:                                         ; Interrupt Descriptor Table Register
    dw 256*16-1                    ; limit of IDT (size minus one) (4096 bytes - 1)
    dq 0x0000000000000000          ; linear address of IDT

_set_handlers:
    mov rdi, 0x80                ; Set up Software Interrups handler
    mov rax, sys_soft
    call create_gate
    mov rdi, 0x21                ; Set up Keyboard handler
    mov rax, sys_keyboard
    call create_gate
    mov rdi, 0x20                ; Set up PIT handler
    mov rax, sys_pit
    call create_gate
    lidt [IDTR]                    ; load IDT register
    ret

align 16
sys_soft:                                 ; Interrupciones de software, int 80h
    push rdi
    cmp rdi, 0
    jz sys_write
    cmp rdi, 1
    jz sys_read
    cmp rdi, 2
    jz sys_rtc
    cmp rdi, 3
    jz sys_rtc_set
    cmp rdi, 4
    jz sys_ss_test
    cmp rdi, 5
    jz sys_ss_set
    cmp rdi, 6
    jz sys_set_color
    cmp rdi, 7
    jz sys_is_ss_on
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_write:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call write_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_read:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call read_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_rtc:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call rtc_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_rtc_set:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call rtc_set_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_ss_test:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call ss_test_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_ss_set:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call ss_set_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_set_color:
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call format_set_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq

hang:
    cli
    hlt                                    
    jmp hang

_enable_pic:
    in al, 0x21
    mov al, 11111000b           ; Enable Keyboard
    out 0x21, al
    sti
    ret

sys_pit:                 ; INT 0x08 Handler (Timertick)
    push rdi
    push rax
    call pit_handler
    mov al, 0x20             ; Acknowledge the IRQ
    out 0x20, al
    pop rax
    pop rdi
    iretq

sys_keyboard:                  ; INT 0x09 Handler (Keyboard)
    cli
    push rdi
    push rax
    xor eax, eax
    in al, 0x60                ; Get the scancode from the keyboard
    mov rdi, rax
    call keyboard_handler
    mov al, 0x20                ; Acknowledge the IRQ
    out 0x20, al
    pop rax
    pop rdi
    sti
    iretq

sys_is_ss_on:
     mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov rcx, r8
    call ss_status_handler
    push rax
    mov al, 0x20
    out 0x20, al
    pop rax
    pop rdi
    iretq