_write_string:
	MOV1 @dest_addr #0		; Reset memoy addresses, just in case. 2 byte operation!
	POP1 srce_addr			; Get the function argument: address of string to be printed
loop:
	CMP @@srce_addr #0		; If null character is encountered,
	JMP0001 end			; exit the loop
	MOV @@dest_addr @@srce_addr 	; Copy to address stored in 'dest_addr' (2 bytes) the value stored in the address stored in 'srce_addr'. Note the indirections used
	ADD1 @srce_addr #1		; Else, go to the next character
	ADD1 @dest_addr #1		; Add 1 to the value in 'dest_addr'. Note the usage of Big-Endian in the 2 byte operation
	JMP loop			; And jump back to the loop until the null character is encountered
end:
	PSH11 @dest_addr		; This function returns the length of the string
	RET				; Return to the calling address

srce_addr:
	wb 0x00
	wb 0x00
dest_addr:
	wb 0x00
	wb 0x00
