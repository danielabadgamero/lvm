.hello:
	PSP0001			; Push an item to the stack
loop:
	MOV @^ @double_letter	; Move to the address on the top of the stack the value stored in 'letter'
	ADD ^ #1		; Add 1 to the top of the stack
	ADD @letter #1		; Add 1 to the value stored in 'letter'
	CMP @letter #0x7e	; Compare the value stored in 'letter' with Z
	JMP0100 end		; If it is greater, jump to 'end'
	JMP loop		; Else, jump back to 'loop'
end:
	SIG #0x0000
	PSP			; Pop the previously added item from the stack
	HLT			; Exit from the command and stop programme execution, saving the state
double_letter:
	wb 0x00
letter:
	wb 0x20			; Write a byte containing 'A'
