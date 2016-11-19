clear_screen:
	pusha
	; put cursor in the top left corner
	mov ah, 0x02
	mov dh, 0x0
	mov dl, 0x0
	mov bh, 0x0
	int 0x10
	; get current video mode
	mov ah, 0x0f
	int 0x10
	; AH         Number of character columns per line
    ; BH         Active display page number
	xor bx, bx
	mov bl, ah
	xor ax, ax
	mov al, 25
	mul bl
	mov cx, ax
	mov ah, 0x0a
	mov al, 0x20
	mov bh, 0x0
	int 0x10
	popa
	ret
