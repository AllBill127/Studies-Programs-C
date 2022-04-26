;13. Žingsninio režimo pertraukimo (int 1) apdorojimo procedūra, atpažįstanti komandą INC r/m.
;Ši procedūra turi patikrinti, ar pertraukimas įvyko prieš vykdant komandos INC pirmąjį variantą,
;jei taip, į ekraną išvesti perspėjimą, ir visą informaciją apie komandą: adresą, kodą, mnemoniką, operandus.
;
; INC r/m OPK = 1111 111w mod 000 r/m [offset] (offset can be 1 or 2 bytes depending on mod)
;
;Reikalavimai:
;@@Savo darbo pradžioje programa turi išvesti trumpą aprašymą: autorių ir ką ji daro. Komandinės eilutės parametrų apdoroti nebūtina.
;@@Programa turi apdoroti programos variante nurodytą pertraukimą:
;	@Įsiminti esamą pertraukimo apdorojimo procedūros adresą;
;	@Į pertraukimo vektorių lentelę įrašyti Jūsų parašytos pertraukimo apdorojimo procedūros adresą;
;	@Atlikti keletą operacijų, kurių metu gali kilti pertraukimas;
;	@Atstatyti buvusį pertraukimo apdorojimo procedūros adresą;
;@@Atsiskaitant būtina sugebėti atsakyti į visus su užduotimi susijusius klausimus (pertraukimo mechanizmas, operacijų kodai,...).

;Pvz.: Į ekraną išvedama informacija galėtų atrodyti taip: Zingsninio rezimo pertraukimas! 0000:0128  FEC0  inc al ; al= 01



.model small

IntNR EQU 1h

.stack 100h

.data
	IntroMSG db 'Made by Aleksandras Bilevicius', 0Dh, 0Ah, 'This program changes the INT1 interrupt and processes the INC r/m or DEC r/m command', 0Dh, 0Ah, 0Ah, '$'
	var1 db 0Ah
	var2 dw 0122h
	var3 db 'Hello$'
	
	AXreg dw ?
	BXreg dw ?
	CXreg dw ?
	DXreg dw ?
	SIreg dw ?
	DIreg dw ?
	BPreg dw ?
	IPreg dw ?
	CSreg dw ?
	
	OPK1 db ?
	OPK2 db ?
	OPKw db ?
	OPKmod db ?
	OPKreg db ?
	OPKr_m db ?
	offset1 db ?
	offset2 db ?
	
	MsgState db 0h
	
	NotIncMSG db 'Command which called the INT1 interrupt was not INC r/m or DEC r/m', 0Dh, 0Ah, '$'
	IntMSG db 'Single step interrupt! $'
	IncMSG db '=> inc $'
	DecMSG db '=> dec $'
	W0MSG db 'byte ptr [$'
	W1MSG db 'word ptr [$'
	RegNAME db 'al$ cl$ dl$ bl$ ah$ ch$ dh$ bh$'
	MSG000 db 'bx + si$'
	MSG001 db 'bx + di$'
	MSG010 db 'bp + si$'
	MSG011 db 'bp + di$'
	MSG100 db 'si$'
	MSG101 db 'di$'
	MSG110 db 'bp$'
	MSG111 db 'bx$'
	
	AddMSG db " + $"
	EquMSG db ' = $'
	BXregMSG db ' bx = $'
	SIregMSG db ' si = $'
	DIregMSG db ' di = $'
	BPregMSG db ' bp = $'

.code

main:
	;==================== Moves @data segment address to DS register ======
	MOV	ax, @data
	MOV	ds, ax

	;===================== Moves 0, to ExtraSegment register =============
	MOV	ax, 0
	MOV	es, ax						; es = 0 is the address to the interrupt vector table
	
	;===================== Print program introduction=====================
	mov ah, 09h
	mov dx, offset IntroMSG
	int 21h
	
	;=============== Save original interupt vector for INC command =======
	PUSH es:[IntNR * 4]				; saves the original interrupt procedure address
	PUSH es:[IntNR * 4 + 2]			; saves the original interrupt procedure code segment
	
	;========== Change the interrupt procedure address to a new one =======
	MOV	word ptr es:[IntNR * 4], offset NewInterrupt	; change the interrupt procedure address to the new address
	MOV	es:[IntNR * 4 + 2], cs							; saves the segment in which the new interrupt procedure can be found (CodeSegment = .code)

;====== Stress testing the new interrup procedure ======
	;================ Setting TrapFlag to 1 ==========================
									; default flag sum = 0002h if all flags = 0
	PUSHF							; saves all flags before testing in StatusFlag (SF = Flag Mask sum = word size hex code)
	
	PUSHF							; saves StatusFlag again so we get all flags sum and therefore check TF
	POP ax							; gets StatusFlag sum from the stack
	OR ax, 0100h					; adds TrapFlag to SF sum (TrapFlag mask is 0100h)
	PUSH ax							; saves changed StatusFlag sum to the stack
	POPF							; returns changed flags (TF = 1)
	
	;====================== Testing the interrup =====================
	mov bx, 0000h					; ????(NOP) irrelavant command because the first interrupt happens after this one
	
	inc bx
	mov cl, 02h
	inc cl
	inc var1
	inc var2+2
	dec word ptr [bx+si+2]
	mov si, 7209h
	dec word ptr [bp]
	mov di, offset var2
	inc byte ptr [bp+di]+2051h
	mov bx, offset var3
	inc word ptr [bx+si+2]
	mov si, 7209h
	inc word ptr [bp]
	
	POPF							; returns StatusFlag before testing which also returns TF = 0
	
	;============= Return the original interrupt procedure address ========
	POP	es:[IntNR * 4 + 2]			; returns the original interrupt procedure address
	POP	es:[IntNR * 4]				; returns the original interrupt procedure code segment

	MOV ax, 4C00h
	INT 21h

;======================== Procedures ===================
	;================= NewInterrupt procedure =====================
	PROC NewInterrupt
		PUSH ax
		PUSH bx
		PUSH dx
		PUSH bp
		PUSH es
		PUSH ds
		
		MOV AXreg, ax
		MOV BXreg, bx
		MOV CXreg, cx
		MOV DXreg, dx
		MOV SIreg, si
		MOV DIreg, di
		MOV BPreg, bp

		; moves the @data segment address to DS register if we need to use it while in the interrupt procedure
		MOV	ax, @data
		MOV	ds, ax

		; gets and saves full Operation code including 2 bytes of offset (later we check if we need to use the offset at all)
		MOV bp, sp					; gets the address of the line that called the interrupt
		ADD bp, 12					; adds 12 bytes because we pushed 6 words in to the stack before
		MOV bx, [bp]				; stores the address of the line (IP) from which the interrupt was called so we can take information from there
		MOV IPreg, bx				; saves the address of the line (IP) from which the interrupt was called so we can print the full address later
		MOV es, [bp+2]				; stores the segment from which the interrupt was called so we can take information from there
		MOV CSreg, es				; saves the segment from which the interrupt was called so we can print full address later
		MOV dl, es:[bx]				; gets the Operation Code used in the segment:line that called the interrupt
		MOV OPK1, dl
		MOV dh, es:[bx+1]			; reads the next byte for more info about the operation
		MOV OPK2, dh
		MOV cx, es:[bx+2]			; gets possible offset
		MOV offset1, cl				; saves the second offset byte
		MOV offset2, ch				; saves the first offset byte
		
		; get w bit, mod, reg and r/m of the command before the interrupt
		AND dl, 01h					; if last bit was 1 dl = 01h, if last bit was 0 dl = 00h 
		MOV OPKw, dl				; saves w value
		MOV dl, OPK2
		SHL dl, 2					; clears the first two bits
		SHR dl, 5					; clears the 3 last bits and moves the reg bits to the right
		MOV OPKreg, dl				; saves reg value
		MOV dl, OPK2
		SHR dl, 6					; clears all bits exept mod and moves mod to the right
		MOV OPKmod, dl				; saves mod value
		MOV dl, OPK2
		SHL dl, 5					; clears all bits exept r/m
		SHR dl, 5					; moves r/m to the right
		MOV OPKr_m, dl				; saves r/m value
		
		; checks if operation code was INC r/m or DEC r/m
		; if Operation Code was not 1111 1111 then check if it was 1111 1110 before exiting
		cmp OPK1, 0FEh
		je CHECK_REG				; if OPK1 was not FE, then dont jump and check if it was FF
		cmp OPK1, 0FFh
		je CHECK_REG				; if OPK1 was not FF either then there is no need to check OPKreg value
		jmp NOT_INC_DEC					; if OPK1 was not FE or FF then print message and exit
		
				CHECK_REG:
				mov MsgState, 0h			; zeroes the state before checking
				; if Operation Code was FE or FF check if reg was 000 or 001. If not then then print message and exit
				cmp OPKreg, 00h				; if OPK1 was FE or FF and reg was 000 then MsgState = 0 -> IncMSG
				je SKIP						; if OPK1 was FE or FF and reg was 000 then Skip to info printing
				cmp OPKreg, 01h				; if OPK1 was FE or FF but reg was not 000 then check if it was 001
				jne NOT_INC_DEC				; if OPK1 was FE or FF but reg was not 000 or 001 then print message and exit 
				mov MsgState, 1h			; MsgState = 1 -> DecMSG
		
		jmp SKIP
		NOT_INC_DEC:
		mov ah, 09h
		mov dx, offset NotIncMSG
		int 21h
		jmp EXIT
		
		SKIP:
		
		
	; print all information about the "INC r/m" command that called the interrupt
		; print a message about the single step interrupt happening
		mov ah, 09h
		mov dx, offset IntMSG
		int 21h
		
		; print the full address of the line after which the interrupt was called
		call PrintAddress
		
		; print the full mashine code of the command which called the interrupt
		call PrintOPK
		
				; print the mnemonic of the command that called the interrupt
				cmp MsgState, 0
				jne DEC_MSG
				mov ah, 09h
				mov dx, offset IncMSG
				int 21h
				jmp SKIP_DEC_MSG
				DEC_MSG:
				mov ah, 09h
				mov dx, offset DecMSG
				int 21h
				
				SKIP_DEC_MSG:
		
		; check if mod was 11 and if true print register and its value used by the INC r/m command
		cmp OPKmod, 03h
		jne NOT_11
		call PrintMod11
		jmp NEW_LINE
		
		; mod was not 11 so we check other options
		NOT_11:
		; because mod was not 11 command doesnt increment a register so we need to specify the size of memory we will increment
		cmp OPKw, 00h				; if w bit is 0 then "byte ptr [", if w bit is 1 then "word ptr ["
		jne W_NOT_0					
		mov dx, offset W0MSG		; because this prints the opening "[" next 3 procedures dont print it
		jmp SKIP_W_1
		W_NOT_0:
		mov dx, offset W1MSG
		SKIP_W_1:
		mov ah, 09h
		int 21h
		
		; check if mod was 00 and then check r/m to print appropriate register operands (BX, SI, DI, BP) and their values
		cmp OPKmod, 00h
		jne NOT_00
		call PrintMod00
		jmp NEW_LINE
		
		; mod was not 11 or 00 so we check other options
		NOT_00:
		; check if mod was 10 and then check r/m to print appropriate register operands (BX, SI, DI, BP), 2byte offset and register values
		cmp OPKmod, 02h
		jne NOT_10
		call PrintMod10
		jmp NEW_LINE
		
		; mod was not 11, 00 or 10 so we assume that it is 01 as it is the last possible mod value
		NOT_10:
		; check r/m to print appropriate register operands, 1byte offset and register values
		call PrintMod01
		jmp NEW_LINE
		
		NEW_LINE:
		; print a new line after the information about the single step interrupt
		mov ah, 02h
		mov dl, 0Ah
		int 21h
		
		EXIT:
		
		POP ds
		POP es
		POP bp
		POP	dx
		POP bx
		POP	ax
		IRET			;pabaigoje būtina naudoti grįžimo iš pertraukimo apdorojimo procedūros komandą IRET
						;paprastas RET netinka, nes per mažai informacijos išima iš steko
	NewInterrupt ENDP

;=================== Helping procedures while in the interrupt =============
	;======== Print byte simbols to screen ========
	PrintByte PROC
		; this process takes the AL register before the call and prints it as ASCII symbols
		push dx
		push ax
		push bx
		
		mov bl, 10h
		mov ah, 00h
		div bl						; Dalina skaiciu esanti AX registre; AL = Dalmuo (pirmas baito hexCode simbolis), AH = liekana (antras baito hexCode simbolis)
			
			push ax					; Issaugo simbolio reiksme po dalybos, nes isvedant i ekrana ax gali pasikeisti
			cmp al, 0Ah
			jge CHAR1       		; Jeigu simbolis yra raide soka i raides skaiciavima
			add al, 30h     		; Jeigu simbolis yra skaicius tai jo hexCode = reg (simbolis) + 30h
			jmp SKIP_CHAR1  		; Jeigu simbolis buvo skaicius tai praleidzia raides skaiciavima
			CHAR1:
			add al, 37h				; Jeigu simbolis yra didzioji raide tai jo hexCode = reg (simbolis) + 37h
			
		SKIP_CHAR1:
		mov dl, al
		mov ah, 02h
		int 21h
		
			pop ax
			cmp ah, 0Ah     		;
			jge CHAR2				;
			add ah, 30h				; Kartoja simboliu vertima i hexCode su antru baito simboliu esanciu DX
			jmp SKIP_CHAR2			;
			CHAR2:					;
			add ah, 37h				;
			
		SKIP_CHAR2:
		mov dl, ah
		mov ah, 02h
		int 21h
		
		pop bx
		pop ax
		pop dx
		ret
	PrintByte ENDP
	
	;======== Print full address of the command that called the interrupt ========
	PrintAddress PROC
		push ax
		push dx
		
		mov dx, CSreg
		mov al, dh
		call PrintByte
		mov al, dl
		call PrintByte
		mov ah, 02h
		mov dl, ':'
		int 21h
		mov dx, IPreg
		mov al, dh
		call PrintByte
		mov al, dl
		call PrintByte
		mov ah, 02h
		mov dl, ' '
		int 21h
		
		pop dx
		pop ax
		ret
	PrintAddress ENDP
	
	;======== Print full mashine code of the command that called the interrupt ========
	PrintOPK PROC
		push ax
		
		mov al, OPK1
		call PrintByte
		mov al, OPK2
		call PrintByte
			; check what size offset was used
			cmp OPKmod, 03h
			je SKIP_OFFSET			; if mod was not 11 (03h) then check if it was 00
			cmp OPKmod, 00h		;/
			jne	CHECK_OFFSET	;|	; if mod was 00 (00h) then check if r/m was 110
			cmp OPKr_m, 06h		;|
			je TWO_BYTES		;|	; if mod was 00 and r/m was 110 then offset was 2 bytes
			jmp SKIP_OFFSET		;\	; if mod was 00 and r/m was not 110 then skip offset printing
			
			CHECK_OFFSET:
			cmp OPKmod, 02h			; if mod was not 11 or 00 then check if mod is 10 (02h)
			je TWO_BYTES			; if mod was 10 then print two offset bytes 
			cmp OPKmod, 01h			; if mod was not 11, 00 or 10 then check if mod is 01 (01h)
			je ONE_BYTE				; at this point all mod varients are checked so there is no extra condition
		
		TWO_BYTES:
		mov al, offset1
		call PrintByte
		mov al, offset2
		call PrintByte
		jmp SKIP_OFFSET
		
		ONE_BYTE:
		mov al, offset1
		call PrintByte
		jmp SKIP_OFFSET
		
		SKIP_OFFSET:
		mov ah, 02h
		mov dl, ' '
		int 21h
		
		pop ax
		ret
	PrintOPK ENDP
	
	;======== Print register name if mod was 11 ========
	PrintMod11 PROC
		push ax
		push cx
		push bx
		push dx
		
		; loop through possible r/m values and when a match is found exit loop and print reg name
		mov cx, 0008h
		mov ax, 0000h
		REG_NR:						; we dont check if w is 1 or 0 because INC word sized reg has a different OPK then INC r/m
			cmp OPKr_m, al			; check if r/m matches current al (reg value)
			je PRINT_REG			; if a match was found the jump to printing the reg name
			inc al					; if current value didnt match then increase the value
		loop REG_NR					; loop over all 8 possible values
		
		PRINT_REG:
		mov bl, 04h					; counts the additional offset for RegNAME to get the right reg name
		mul bl						; every 4 symbols there is a new reg name so we multiply r/m value which is in AL by 5
		mov dx, offset RegNAME
		add dl, al					; adds the additional offset to set the correct offset for printing the right reg name
		push dx						; saves register name offset in the stack for later printing
		mov ah, 09h
		int 21h
		
		; print register name and its value
		mov ah, 02h
		mov dl, ';'
		int 21h
		mov ah, 02h
		mov dl, ' '
		int 21h
		mov ah, 09h
		pop dx						; returns register name offset from the stack
		int 21h
		mov ah, 09h
		mov dx, offset EquMSG		; prints " = "
		int 21h
		call PrintMod11Reg			; checks and prints byte size reg value depending on r/m value
		mov ah, 02h
		mov dl, 'h'
		int 21h
		
		pop dx
		pop bx
		pop cx
		pop ax
		ret
	PrintMod11 ENDP
	
	;======== Check r/m value and print appropriate register and its value used in command ========
	PrintMod11Reg PROC
		push ax
		push dx
		
		cmp OPKr_m, 00h
		jne NEXT_REG1
		mov dx, AXreg
		mov al, dl
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG1:
		cmp OPKr_m, 01h
		jne NEXT_REG2
		mov dx, CXreg
		mov al, dl
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG2:
		cmp OPKr_m, 02h
		jne NEXT_REG3
		mov dx, DXreg
		mov al, dl
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG3:
		cmp OPKr_m, 03h
		jne NEXT_REG4
		mov dx, BXreg
		mov al, dl
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG4:
		cmp OPKr_m, 04h
		jne NEXT_REG5
		mov dx, AXreg
		mov al, dh
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG5:
		cmp OPKr_m, 05h
		jne NEXT_REG6
		mov dx, CXreg
		mov al, dh
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG6:
		cmp OPKr_m, 06h
		jne NEXT_REG7
		mov dx, DXreg
		mov al, dh
		call PrintByte
		jmp END_REG_CHECK
		
		NEXT_REG7:
		mov dx, BXreg
		mov al, dh
		call PrintByte
		
		END_REG_CHECK:
		
		pop dx
		pop ax
		ret
	PrintMod11Reg ENDP
	
	;======== Print all registers, offset and register values used in command if mod was 00 ========
	PrintMod00 PROC
		push ax
		push dx
		
		cmp OPKr_m, 06h				;| if mod was 00 and r/m was 110 then exception of dirrect addressing mode -> must print two bytes of offset
		jne RM_CHECK				;\ if mod was 00 but r/m was not 110 then jump to general r/m check. In it the r/m = 110 wont happen so no problem
		mov al, offset2
		call PrintByte
		mov al, offset1
		call PrintByte
		mov ah, 02h
		mov dl, 'h'
		int 21h
		mov ah, 02h
		mov dl, ']'
		int 21h
		jmp END_MOD_00
		
		RM_CHECK:
		call RMcheck				; check r/m value and print appropriate register operands used in the command
		mov ah, 02h
		mov dl, ']'
		int 21h
		call PrintRMcheck			; check r/m value and print appropriate register operand values used in the command
		
		END_MOD_00:
		
		pop dx
		pop ax
		ret
	PrintMod00 ENDP
	
	;======== Print all registers, offset and register values used in command if mod was 10 ========
	PrintMod10 PROC
		push ax
		push dx
		
		call RMcheck
		mov ah, 09h
		mov dx, offset AddMSG
		int 21h
		mov al, offset2				; prints offset bytes backwards because DOS reads lower byte first
		call PrintByte				; offset1 is lower byte and offset2 is higher byte
		mov al, offset1
		call PrintByte
		mov ah, 02h
		mov dl, 'h'
		int 21h
		mov ah, 02h
		mov dl, ']'
		int 21h
		call PrintRMcheck			; prints "; reg name = ...h, reg name = ...h"
		
		pop dx
		pop ax
		ret
	PrintMod10 ENDP
	
	;======== Print all registers, 1 byte offset and register values used in command if mod was 01 ========
	PrintMod01 PROC
		push ax
		push dx
		
		call RMcheck				; prints reg names that were used in addressing memory by the command
		mov ah, 09h
		mov dx, offset AddMSG
		int 21h
		mov al, offset1
		call PrintByte				; prints 1byte offset used in addressing memory
		mov ah, 02h
		mov dl, 'h'
		int 21h
		mov ah, 02h
		mov dl, ']'
		int 21h
		call PrintRMcheck			; prints "; reg name = ...h, reg name = ...h"
		
		pop dx
		pop ax
		ret
	PrintMod01 ENDP
	
	;======== Check r/m value and print appropriate register operands used in addressing memory ========
	RMcheck PROC
		push ax
		push dx
		
		cmp OPKr_m, 00h
		jne NEXT1
		mov ah, 09h
		mov dx, offset MSG000
		int 21h
		jmp END_RM_CHECK
		
		NEXT1:
		cmp OPKr_m, 01h
		jne NEXT2
		mov ah, 09h
		mov dx, offset MSG001
		int 21h
		jmp END_RM_CHECK
		
		NEXT2:
		cmp OPKr_m, 02h
		jne NEXT3
		mov ah, 09h
		mov dx, offset MSG010
		int 21h
		jmp END_RM_CHECK
		
		NEXT3:
		cmp OPKr_m, 03h
		jne NEXT4
		mov ah, 09h
		mov dx, offset MSG011
		int 21h
		jmp END_RM_CHECK
		
		NEXT4:
		cmp OPKr_m, 04h
		jne NEXT5
		mov ah, 09h
		mov dx, offset MSG100
		int 21h
		jmp END_RM_CHECK
		
		NEXT5:
		cmp OPKr_m, 05h
		jne NEXT6
		mov ah, 09h
		mov dx, offset MSG101
		int 21h
		jmp END_RM_CHECK
		
		NEXT6:
		cmp OPKr_m, 06h
		jne NEXT7
		mov ah, 09h
		mov dx, offset MSG110
		int 21h
		jmp END_RM_CHECK
		
		NEXT7:
		; no need to compare again because at this point it must match 
		mov ah, 09h
		mov dx, offset MSG111
		int 21h
		
		END_RM_CHECK:
		
		pop dx
		pop ax
		ret
	RMcheck ENDP
	
	;======== Check r/m value and print appropriate register operand values used in addressing memory ========
	PrintRMcheck PROC
		push ax
		push dx
		
		mov ah, 02h
		mov dl, ';'
		int 21h
		
		cmp OPKr_m, 00h
		jne PRINT_NEXT1
		mov di, offset BXregMSG
		mov bx, BXreg
		call PrintReg
		mov ah, 02h
		mov dl, ','
		int 21h
		mov di, offset SIregMSG
		mov bx, SIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT1:
		cmp OPKr_m, 01h
		jne PRINT_NEXT2
		mov di, offset BXregMSG
		mov bx, BXreg
		call PrintReg
		mov ah, 02h
		mov dl, ','
		int 21h
		mov di, offset DIregMSG
		mov bx, DIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT2:
		cmp OPKr_m, 02h
		jne PRINT_NEXT3
		mov di, offset BPregMSG
		mov bx, BPreg
		call PrintReg
		mov ah, 02h
		mov dl, ','
		int 21h
		mov di, offset SIregMSG
		mov bx, SIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT3:
		cmp OPKr_m, 03h
		jne PRINT_NEXT4
		mov di, offset BPregMSG
		mov bx, BPreg
		call PrintReg
		mov ah, 02h
		mov dl, ','
		int 21h
		mov di, offset DIregMSG
		mov bx, DIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT4:
		cmp OPKr_m, 04h
		jne PRINT_NEXT5
		mov di, offset SIregMSG
		mov bx, SIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT5:
		cmp OPKr_m, 05h
		jne PRINT_NEXT6
		mov di, offset DIregMSG
		mov bx, DIreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT6:
		cmp OPKr_m, 06h
		jne PRINT_NEXT7
		mov di, offset BPregMSG
		mov bx, BPreg
		call PrintReg
		jmp END_PRINT_RM_CHECK
		
		PRINT_NEXT7:
		; no need to compare again because at this point it must match 
		mov di, offset BXregMSG
		mov bx, BXreg
		call PrintReg
		
		END_PRINT_RM_CHECK:
		
		pop dx
		pop ax
		ret
	PrintRMcheck ENDP
	
	;======== Print reg name and its value " reg = 0000h" ========
	PrintReg PROC
		push ax
		push dx
		
		mov dx, di					; di is set to the reg message offset before the call of this procedure
		mov ah, 09h
		int 21h
		
		mov al, bh					; bx is set to the reg value before the call of this procedure
		call PrintByte
		mov al, bl
		call PrintByte
		
		mov ah, 02h
		mov dl, 'h'
		int 21h
		
		pop dx
		pop ax
		ret
	PrintReg ENDP
	
end main