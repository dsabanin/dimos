[org 0x7c00]
KERNEL_OFFSET equ 0x1000
CR equ 0x0d
LF equ 0x0a

	mov [BOOT_DRIVE], dl

	mov bp, 0x9000
	mov sp, bp

	; call clear_screen
	mov bx, MSG_REAL_MODE
	call print_string
	call load_kernel
	call switch_to_pm

	jmp $

%include "boot/print_string.asm"
%include "boot/print_hex.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/print_string_pm.asm"
%include "boot/switch_to_pm.asm"
%include "boot/clear_screen.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET
	mov dh, 15
	mov dl, [BOOT_DRIVE]
	call disk_load

	ret

[bits 32]

; This is  where we  arrive  after  switching  to and  initialising  protected  mode.
BEGIN_PM:
	mov ebx , MSG_PROT_MODE
	call  print_string_pm    ; Use  our 32-bit  print  routine.
	call KERNEL_OFFSET
	jmp $                      ; Hang.

; Global  variables
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Started in 16-bit [real mode]", CR, LF, 0
MSG_PROT_MODE   db "Successfully switched to 32-bit [protected  mode]", 0
MSG_LOAD_KERNEL db "Loading kernel...", CR, LF, 0

; Bootsector  padding
times  510-($-$$) db 0
dw 0xaa55
