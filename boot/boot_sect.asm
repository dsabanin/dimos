[ORG 0x7c00]

KERNEL_OFFSET equ 0x1000
CR equ 0x0d
LF equ 0x0a

mov [BOOT_DRIVE], dl	; Remember what boot drive we booted from

mov bp, 0x9000			; Setting up the initial stack
mov sp, bp

mov bx, MSG_REAL_MODE	; Print 16-bit mode message
call print_string
call load_kernel		; Load kernel from disk
call switch_to_pm		; Switch to 32-bit and jump to KERNEL_OFFSET
jmp $

%include "boot/print_string.asm"
%include "boot/print_hex.asm"
%include "boot/disk_load.asm"
%include "boot/load_kernel.asm"
%include "boot/gdt.asm"
%include "boot/print_string_pm.asm"
%include "boot/switch_to_pm.asm"
%include "boot/clear_screen.asm"

; Global  variables
BOOT_DRIVE      db 0	; Stores the drive we read boot sector from
MSG_REAL_MODE   db "Started in 16-bit [real mode]", CR, LF, 0
MSG_PROT_MODE   db "Successfully switched to 32-bit [protected  mode]", 0

; Bootsector  padding
times  510-($-$$) db 0
dw 0xaa55
