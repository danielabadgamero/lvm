_write_string:
	MOV @addr #0		; Reset memoy addresses, just in case
	MOV @addr+1 #0		; We need to have both bytes directionable with labels for simplicity
loop:
	CMP @^ #0		; If null character is encountered,
	JMP0001 end		; exit the loop
	MOV @@addr @^		; Copy to address stored in 'addr' (2 bytes) the value stored in the address stored in the stack. Note the indirections used
	ADD ^ #1		; Else, go to the next character
	CMP @addr+1 #0xff	; Checking whether the 'addr+1' byte will overflow (if length of string is greater than 0xff)
	JMP0010 no_overflow	; If not, skip the next instruction
	ADD @addr #1		; Add 1 to the byte before 'addr'. Note the usage of Big-Endian
no_overflow:
	ADD @addr+1 #1		; Add 1 to the destination address
	JMP loop		; And jump back to the loop until the null character is encountered
end:
	MOV ^ @addr		; This function returns the length of the string
	MUL ^ #256		; Use De Morgan's law to load 2 sepparate bytes into the stack (2 bytes): A OR B = NOT (NOT A AND NOT B)
	NOT ^			; NOT A
	NOT @addr+1		; NOT B
	AND ^ @addr+1		; NOT A AND NOT B
	NOT ^			; NOT (NOT A AND NOT B)
	CLR			; Return to the calling address

addr:
	wb 0x00
addr+1:
	wb 0x00
