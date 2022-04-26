; Parašykite programą, kuri atspausdina įvestos simbolių eilutės ASCII kodus dvejetainiu pavidalu;
;       Pvz.: įvedus abC1 turi atspausdinti 1100001 1100010 1000011 110001
;		Pvz.: įvedus abC756 atspausdins 1100001 1100010 1000011 110111 110101 110110
;		Pvz.: įvedus Fg2. atspausdins 1000110 1100111 110010 101110
;		Pvz.: įvedus 0#-$H. atspausdins 110000 100011 101101 100100 1001000

; Pagrindine ideja:    Registruose duomenys saugomi dvejetainiu kodu, bet koduojami ir pateikiami sesioliktainiu.
;                           Naudodami shl komanda galime panaudoti carryFlaga kaip dvejetainio kodo kairiausio bito rodikli
;==============================================================================================================================



.model small

.stack 100h

.data

message1 db 10, 'Iveskite simboliu eilute be $ zenklu: $'					; 10 - newLine, 13 - writeOnCurrentLine
message2 db 10, 'Ivesto simbolio dvejetainis kodas: $'
message3 db 10, 'Bloga ivestis. Iveskite tik skaiciaus simboli $'
input_buffer db 100, ?, 100 dup('$')
chars dw 2
var1 dw 0

.code

main:

	mov ax, @data
	mov ds, ax
	;=============================================================
	
	mov ah, 09h
	mov dx, offset message1
	int 21h
	
	mov ah, 0Ah						; iniciuoja simboliu eilutes ivedima
	mov dx, offset input_buffer
	int 21h

	
	mov ah, 09h
	mov dx, offset message2
	int 21h


	mov bx, 2						; praleidzia pirmus du narius, nes jie nera inputas
	leng:
	cmp input_buffer[bx], '$'		; tikrina ar inputo narys nera $, nes jis zymi inputo pabaiga pagal bufferio apibrezima
	je output						
	inc bx
	inc chars						; skaiciuoja paskutinio ivesto simbolio vieta
	inc var1						; skaiciuoja ivestu simboliu skaiciu
	jmp leng
	
	
	output:
		mov bx, chars					;
		sub bx, var1					; skaiciuoja isvedamo simbolio vieta nuo pirmo [2] iki paskutinio
		mov bl, input_buffer[bx]
	
		mov cx, 8						; =8, nes simboliai koduojami 8 bitais
		
		skipZero:
		shl bl, 1
		jc printOne						; jeigu bitas = 1, tai soka i printCycle ir isvedes 1, tesia isvedima
		dec cx							; jeigu bito isvedimas praleidziamas, tao loop ciklo zingsnis cx sumazinamas 1
		jnc skipZero					; jeigu bitas = 0, jo isvedimas nevykdomas, kol neprieinamsa pirmas bitas = 1
			
		printCycle:
			shl bl, 1 						; pastums bl esancius bitus i kaire puse per 1 ir buves kairiausias bitas nurodys carryFlago reiksme
			
			jc printOne						; jeigu carryFlagas != 1, tai isves 0, jeigu = 1 - persoks i zyme
			mov dl, 48
			jmp printOut					; jeigu dl priskira 0, tai iskart soka i isvedima
			
			printOne:						
			mov dl, 49						; kai carryFlagas = 1, tai dl priskiriama '1' simbolio reiksme = 49dec / 31h
			
			printOut:
			mov ah, 02h						; iniciuoja vieno simbolio isvedima
			int 21h							; isveda dl registra
		loop printCycle					; kol cx != 0, kartoja cikla ir daro zingsni cx--
		
		mov dl, ' '
		mov ah, 02h						; iniciuoja vieno tarpo isvedima
		int 21h							; isveda dl registra
		
		dec var1						; mazina likusiu simboliu skaiciu
		
	cmp var1, 1							; kai lieka 1 simbolis, tai visi simboliai jau isvesti, nes pradedama nuo 0. Isvedimas nebekartojamas
	jne output
	
	mov ah, 4ch							; iniciuoja programos uzbaigima
	int 21h

end main