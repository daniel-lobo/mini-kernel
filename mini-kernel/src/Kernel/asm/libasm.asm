global libasm

global _syscall

global _pit_handler
global _keyboard_handler, _keyboard_done
global _syscall_handler

global _get_idtr, _set_idt_entry, _mask_pic, _cli, _sti
global _outb
global _inb
global _hlt

extern pit_handler
extern keyboard_handler
extern syscall_handler
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

_hlt:
	hlt
	ret

_cli:
    cli
    ret

_sti:
    sti
    ret

_set_idt_entry:
    shl rdi, 4                  ; quickly multiply rdi by 16
    add rdi, rdx
    stosw                       ; store the low word (15..0)
    shr rsi, 16
    add rdi, 4                  ; skip the gate marker
    stosw                       ; store the high word (31..16)
    shr rsi, 16
    stosd                       ; store the high dword (63..32)
    ret

_get_idtr:
    sidt [rdi]
    ret

_mask_pic:
    in al, 0x21
    mov al, 11111000b           ; Enable Keyboard
    out 0x21, al
    ret

_pit_handler:                 ; INT 0x08 Handler (Timertick)
    push rdi
    push rax
    call pit_handler
    mov al, 0x20             ; Acknowledge the IRQ
    out 0x20, al
    pop rax
    pop rdi
    iretq

_keyboard_handler:                  ; INT 0x09 Handler (Keyboard)
    push rdi
    push rax
    xor eax, eax
    in al, 0x60                ; Get the scancode from the keyboard
    mov rdi, rax
    call keyboard_handler

_keyboard_done:
    mov al, 0x20                ; Acknowledge the IRQ
    out 0x20, al
    pop rax
    pop rdi
    iretq

_syscall_handler:					; INT 0x80 Handler (Syscall)
    push rdi
    push rax
	call syscall_handler
    mov al, 0x20                ; Acknowledge the IRQ
    out 0x20, al
    pop rax
    pop rdi
    iretq

_syscall:
    push rbp
    mov rbp, rsp
    push rbx
    push rsp
    push rbp
    push r12
    push r13
    push r14
    push r15
    int 80h
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rsp
    pop rbx
    mov rsp, rbp
    pop rbp
    ret