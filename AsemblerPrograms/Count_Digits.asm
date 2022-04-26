;Parasykite programa, kuri atspausdina [desimtainiu] skaitmenu kieki ivestoje simboliu eiluteje;
;Pvz.: ivedus asd122ADt8 turi atspausdinti 4

.MODEL small
.STACK 100h
.DATA

    buffer db 100, ?, 100 dup('$')
	buffer_out db 10, ?, 10 dup('$')
    msg1 db 10, 'Iveskite simboliu eilute $'
	msg2 db 13, 10, 10, 'Skaitmenu eiluteje kiekis: $'
    leng dw 0
	charNum dw 0

.CODE

main:

    mov ax, @data
    mov ds, ax
;================================

	mov charNum, 185
	
	;;============= Butina padaryti cikla tikrinanti ar skaicius nera dvizenklis ir pan.
	
	mov ax, charNum
	mov bx, 10
	mov cx, 0
	numLen:
		div bx
		mov dx, 0
		cmp ax, 0000h
		je quit
		inc cx
	jmp numLen			
	quit:	
	

	
	mov ax, charNum
	mov bx, 10
	mov si, cx
	mov [buffer_out+3+si], '$'
	output:
		div bx
		or dx, 30h		; prideda prie hex skaiciaus 30h ir gauna to skaiciaus ASCII simbolio hex skaiciu. Pvz.: 5h+30h = 35h = '5'
		mov [buffer_out+2+si], dl
		mov dx, 0
		dec si
		cmp si, -1
		je quit2
	jmp output
	
	quit2:
		
	;;==================
	
	
	mov dl, offset msg2
	mov ah, 09h
	int 21h
	
	mov dl, offset buffer_out
	mov ah, 09h
	int 21h

	mov ax,4C00h
	int 21h
end main