;Parašykite programa, kuri vieno failo baitus(aka - simbolius) užrašo šešioliktainiais skaitmenim i kita faila.
;       Pvz.: Jei „duom.txt“ failas yra toks, tai komanda "antra duom.txt rez.txt" sukurtu toki „rez.txt“ faila.
;
;		Papildomi reikalavimai:
;@@ Visi parametrai programai turi buti paduodami komandine eilute, o ne prašant juos ivesti iš klaviaturos. Pvz.: antra duom.txt rez.txt
;@@ Jeigu programa paleista be parametru arba parametrai nekorektiški, reikia atspausdinti pagalbos pranešima toki pati, kaip paleidus programa su parametru /?.
;@@ Programa turi apdoroti ivedimo išvedimo (ir kitokias) klaidas. Pavyzdžiui, nustacius, kad nurodytas failas neegzistuoja - ji turi išvesti pagalbos pranešima ir baigti darba.
;@@ Failu skaitymo ar rašymo buferio dydis turi buti nemažesnis už 10 baitu.
;@@ Failo dydis gali viršyti skaitymo ar rašymo buferio dydi.
;@@ Panaudot funkcija.


.model small

BUFsize EQU 20								; Veikia kaip #define C kalboje
BUFsizeOUT EQU 60							; 60 simboliu ir 60 '\n'

.stack 100h

.data

helpMSG db 'Write data and results file names after the program name exactly like this:', 13, 10, 'program.exe data.txt results.txt', '$'
tooLongMSG db 'The arguments line is too long. Shorten the file names', '$'

	dataNAME db 13 dup (0), 0, '$'					; Uzpildo 0, nes dirbant su failu jo vardas turi baigtis 0
	resultsNAME db 13 dup (0), 0, '$'				; 8 baitai pavadinimui, 4 baitai pletiniui ir vienas 0; ne daugiau kaip 13 baitu

openCrashMSG db 'file failed to be opened/created. Error code is ', '$'

	dataHANDLE dw ?
	resultsHANDLE dw ?

	readBUFFER db BUFsize dup (?), '$'

ReadErrorMSG db 'Error occured while reading the file', '$', 10

	writeBUFFER db BUFsizeOUT dup (?), '$'

WriteErrorMSG db 'Error occured while writing the file', '$', 10
PartWriteErrorMSG db 'While writing the file some data was lost', '$', 10

closeCrashMSG db 'file failed to be closed. Error code is ', '$'

.code

main:
	mov ax, @data
	mov ds, ax
	
	;======================Check for help===================
	mov cx, 0000h
	mov cl, es:[0080h]						; 80h = 128dec, o sioje vietoje saugoma kiek buvo nuskaityta baitu agumentu eiluteje
	
	cmp cx, 0h								; Jeigu argumentai neivesti ismeta pagalbos pranesima
	je HELP
	
	mov si, 0082h
	CHECK_HELP:
		mov ax, es:[si]
		cmp ax, '?/'
		je HELP
		inc si
	loop CHECK_HELP
	
	;=====================Help Message================
	jmp SKIP_HELP										; Pagalbos neprireike
	HELP:
		mov ah, 09h
		mov dx, offset helpMSG
		int 21h
		jmp EXIT
		
	SKIP_HELP:
	
	;====================Read file names==============
	mov si, 0082h										; Vieta nuo kurios prasideda realus argumentai (praleidzia pirma tarpa po .exe)
	mov bx, offset dataNAME
	mov cx, 0000h										; Skaitliukas failo pavadinimo ilgiui skaiciuoti
	READ_ARG1:
		cmp cx, 0Dh										; Patikrina ar nebuvo ivestas per ilgas pavadinimas, nes DOS leidzia max 9 baitu pavadinima + 4 baitu pletini (offset + 13)
		jge CRASH_LONG									; Jeigu pavadinimas per ilgas DOS supratimui, tai paraso pranesima ir uzbaigia programa
		mov al, es:[si]
		cmp al, ' '										; Jeigu tarpas tai soka i naujo failo pavadinimo skaityma
		je END_READ1
		mov [bx], al									; Iraso i dataNAME segmenta simboli is ARG_eilutes
		inc si
		inc bx
		inc cx											; Padidina failo pavadinimo ilgio skaitliuka
	jmp READ_ARG1
	
	END_READ1:
	
	inc si												; Praleidzia tarpo irasima
	mov bx, offset resultsNAME							; Vieta kur prasideda failo pavadinimo bufferis
	mov cx, 0000h
	READ_ARG2:
		cmp cx, 0Dh
		jge CRASH_LONG									; Jeigu nuskaite daugiau nei 13 baitu tai parasys pranesima ir programa baigsis 
		mov al, es:[si]						
		cmp al, 0Dh										; Tikrina ar nepasieke CariageReturn, nes jis zymi argumentu eilutes pabaiga
		je END_READ2
		mov [bx], al
		inc si											; Padidina pirmo failo pavadinimo rasymo skaitliuka
		inc bx											; Padidina pirmo failo pavadinimo rasymo skaitliuka
		inc cx
	jmp READ_ARG2
	
	;=====================File names are too long Crash Message================
	jmp END_READ2
	CRASH_LONG:
		mov ah, 09h
		mov dx, offset tooLongMSG
		int 21h
		jmp EXIT
	
	END_READ2:
	
	;===================Open data file=================
	mov ax, 3d00h										; AH = 3d -> atidaro faila pagal DX esanti pavadinima; AL = 00 -> atidaro faila 'read only' formatu
	mov dx, offset dataNAME	
	int 21h
	jc OPEN_CRASH										; Jeigu CF = 1, tai failo atidaryti nepavyko; AX = error kodas
	mov dataHANDLE, ax 									; Issaugo atidaryto failo descriptor'iu
	
	;===================Create/Clear results file=================
	mov ax, 3c00h										; AH = 3c -> sukuria faila pagal DX esanti pavadinima
	mov cx, 0h											; CX = 0h -> sukuria faila 'read only' formatu
	mov dx, offset resultsNAME
	int 21h
	jc OPEN_CRASH										; Jeigu CF = 1, tai failo sukurti nepavyko; AX = error kodas
	mov resultsHANDLE, ax								; Issaugo sukurto failo descriptor'iu
	
	;=====================File couldn't be opened Crash Message================
	jmp READ_FILE
	OPEN_CRASH:
		push ax                                         ; Issaugo error koda
		mov ah, 09h										; Po JC DX registre vis dar yra failo pavadinimas
		int 21h
		mov ah, 09h
		mov dx, offset openCrashMSG
		int 21h
		pop ax                                          ; Grazina error koda i AX
		mov dx, ax
		add dx, 30h                                     ; Pavercia eror koda i ascii simboli
		mov ah, 02h
		int 21h
		jmp EXIT

	;==================Read data file====================
	READ_FILE:
		mov bx, dataHANDLE				; BX perduoda ReadBuf procedurai failo deskriptor'iu
		call ReadBuf					; I AX grazina kiek baitu nuskaite
		cmp ax, 0						; kai faile nebeliks ka skaityt is funkcijos gris AX = 0
		je READ_CLOSE
		
		;===================Work with given data=====================
		mov cx, ax						; nurodo kiek duomenu reiks apdoroti nuskaitytame buferyje
		mov di, offset readBUFFER
		mov si, offset writeBUFFER
		push ax							; Issaugo nuskaitytu duomenu skaiciu stacke
		
		mov bx, 10h
		CHANGE:
			mov dx, 0000h
			mov al, [di]				; Paima simboli is readBUFFER
			push ax						; Issaugo nuskaityta simboli stack'e
			div bx						; Dalina bendra skaiciu susidaranti is DX ir AX registru; AX = Dalmuo (pirmas baito hexCode simbolis), DX = liekana (antras baito hexCode simbolis)
			    
			    cmp ax, 0Ah      
			    jge CHAR1       		; Jeigu simbolis yra raide soka i raides skaiciavima
			    add ax, 30h     		; Jeigu simbolis yra skaicius tai jo hexCode = reg (simbolis) + 30h
			    jmp SKIP_CHAR1  		; Jeigu simbolis buvo skaicius tai praleidzia raides skaiciavima
			    CHAR1:
			    add ax, 37h				; Jeigu simbolis yra didzioji raide tai jo hexCode = reg (simbolis) + 37h
			    
			SKIP_CHAR1:
			
				cmp dx, 0Ah     		;
			    jge CHAR2				;
			    add dx, 30h				; Kartoja simboliu vertima i hexCode su antru baito simboliu esanciu DX
			    jmp SKIP_CHAR2			;
			    CHAR2:					;
			    add dx, 37h				;
			    
			SKIP_CHAR2:					
			
			mov [si], ax				;|
			inc si						;| Iraso baito (simbolio) hexCode simbolius ir tarpo simboli
			mov [si], dx				;|
			inc si						;|
			pop ax						;| Grazina nuskaityta simboli is stack'o	
				cmp al, 0Ah				;| Patikrina ar simbolis yra '\n'
				jne SKIP_NL
				mov [si], byte ptr 0Ah	;| Iraso vietoj tarpo simbolio '\n' po '\n' hex code
				jmp SKIP_SPACE
				
			SKIP_NL:
			mov [si], byte ptr 20h		;| Iraso tarpo simboli po kiekvieno hex code, nebent tai buvo '\n' simbolis
			
			SKIP_SPACE:
			inc si						;\ 
			inc di						; Pastumia is readBUFFER imamo simbolio vieta
		loop CHANGE
	
		;====================Write results file=====================
		pop ax							; istraukia is stacko nuskaitytu duomenu skaiciu
		mov bl, 3h						
		mul bl							; sudaugina is 3, nes du simboliai ir tarpas
		mov cx, ax						; ax parodo kiek duomenu reikes irasyti i WriteBUFFER
		mov bx, resultsHANDLE			; nurodo faila i kuri reikia rasyti per jo descriptoriu
		call WriteBUF
		cmp ax, BUFsizeOUT				; Tikrina ar buvo atliktas darbas su visu output bufferiu
	je READ_FILE
		
	;==================Close results file===================
	WRITE_CLOSE:
	    mov dx, offset resultsNAME		; Pries uzdarant faila i DX registra irasomas failo pavadinimas
		mov ah, 3eh
		mov bx, resultsHANDLE			; Uzdarant failus descriptor'iu BUTINA irasyti i BX, o ne  DX
		int 21h
		jc CLOSE_CRASH
		
	;===================Close data file=====================
	READ_CLOSE:
	    mov dx, offset dataNAME			; Pries uzdarant faila i DX registra irasomas failo pavadinimas
		mov ah, 3eh
		mov bx, dataHANDLE
		int 21h
		jc CLOSE_CRASH					; Jeigu failo uzdaryti nepavyko AX = error kodas
    
    ;=================File closing Crash Message==============
	jmp EXIT
	CLOSE_CRASH:
		push ax                                         ; Issaugo error koda
		mov ah, 09h										; Pries uzdarant faila i DX registra irasomas failo pavadinimas
		int 21h
		mov ah, 09h
		mov dx, offset CLOSECrashMSG
		int 21h
		pop ax                                          ; Grazina error koda i AX
		mov dx, ax
		add dx, 30h                                     ; Pavercia eror koda i ascii simboli
		mov ah, 02h
		int 21h
		jmp EXIT
		
	;===================End of program===================
	EXIT:
		mov ah, 4ch
		int 21h

	;======================Read buffer function=======================
	ReadBuf PROC
		push dx
		push cx
		
		mov ah, 3fh						; Pries iskvieciant proc. BX perduoda failo deskriptoriu
		mov cx, BUFsize					; CX nurodo kiek simboliu reikia nuskaityt is failo
		mov dx, offset readBUFFER
		int 21h
		jc READ_ERROR					; Jeigu nuskaityt failo nepavyko, tai paraso pranesima ir uzbaigia programa
		
		ENDING1:
			pop cx
			pop dx
			ret
		
		READ_ERROR:
			mov ah, 09h
			mov dx, offset ReadErrorMSG
			int 21h
			mov ax, 0					; Grazina 0, nes nenuskaite failo
			jmp ENDING1
	ReadBuf ENDP
	
	;======================Write buffer function=======================
	
	WriteBUF PROC
		push dx
		
		mov ah, 40h						; Pries iskvieciant proc. BX perduoda failo deskriptoriu
		mov dx, offset writeBUFFER
		int 21h
		jc WRITE_ERROR					; Jeigu nurasyt buferio nepavyko, tai paraso prenesima ir uzbaigia programa
		
		cmp cx, ax						; AX - kiek simboliu irase po int 21h, CX - kiek turejo irasyti simboliu
		jne PART_WRITE_ERROR			; Jei dalis simboliu buvo neisvesta, programa apie tai pranesa, bet tesia savo darba iki galo
		
		ENDING2:
			pop dx
			ret
		
		PART_WRITE_ERROR:
			mov ah, 09h
			mov dx, offset PartWriteErrorMSG
			int 21h
			jmp ENDING2
		
		WRITE_ERROR:
			mov ah, 09h
			mov dx, offset WriteErrorMSG
			int 21h
			mov ax, 0
			jmp ENDING2
	WriteBUF ENDP

end main