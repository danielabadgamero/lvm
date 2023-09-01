.hello:
	MOV @#0 #0x61
	SIG #0x0000		; Print single character
	MOV @#0 #0
	MOV @letter #0x20
	PSP1			; Push an item to the stack
ascii_loop:
	MOV @^ @letter		; Move to the address on the top of the stack the value stored in 'letter'
	CMP @letter #0x7e	; Compare the value stored in 'letter' with '~'
	JMP0100 ascii_end	; If it is greater, jump to 'end'
	ADD ^ #1		; Add 1 to the top of the stack
	ADD @letter #0x01	; Add 1 to the value stored in 'letter'
	JMP ascii_loop		; Else, jump back to 'loop'
ascii_end:
	SIG #0x0001
	SIG #0x0003
	CLR1 clear_buf
	MOV ^ hello
	MOV @addr #0
	MOV @addr_2 #0
loop:
	CMP @^ #0
	JMP0001 end
	MOV @@addr @^
	ADD ^ #1
	ADD @addr_2 #1
	JMP loop
end:
	SIG #0x0002
	PSP			; Pop the previously added item from the stack
	HLT1			; Exit from the command and stop programme execution, saving the state

.goodbye:
	CLR1 clear_buf
	PSP1
	MOV ^ goodbye
	MOV @addr #0
	MOV @addr_2 #0
goodbye_loop:
	CMP @^ #0
	JMP0001 goodbye_end
	MOV @@addr @^
	ADD ^ #1
	ADD @addr_2 #1
	JMP goodbye_loop
goodbye_end:
	SIG #0x0002
	PSP
	HLT

clear_buf:
	PSP1
clear_buf_loop:
	CMP ^ #1024
	JMP0001 clear_buf_end
	MOV @^ #0
	ADD ^ #1
	JMP clear_buf_loop
clear_buf_end:
	PSP
	CLR

letter:
	wb 0x20			; Write two bytes containing 0x2000
addr:
	wb 0x00
addr_2:
	wb 0x00
hello:
	ws Hello,\ World!\0
goodbye:
	ws Goodbye!\0
