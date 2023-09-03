_print_string:
	CAL clear_buffer		; First clear the first kilobyte in the RAM, just in case
	MOV1 @dest_addr #0		; Reset 'dest_addr'. To make a 2-byte operation, append '1' to the opcode
	POP1 srce_addr			; Pop the passed address form the stack to 'srce_addr'. Again, 2 bytes. Big-Endian!
print_string_loop:
	CMP @@srce_addr #0		; Check if end of string (null character) reached. Note the indirection used
	JMP0001 print_string_end	; If it is, end the loop
	MOV @@dest_addr @@srce_addr	; Else, put a character on the next address. Note both indirections used, because the labels store addresses, not the values themselves!
	ADD1 @dest_addr #1		; Next address
	ADD1 @srce_addr #1		; Next character
	JMP print_string_loop		; Jump back to the loop
print_string_end:
	SIG #0x0002			; Use a device call to print whatever is in the RAM
	RET				; Return to the calling address

clear_buffer:
	MOV1 @dest_addr #0		; First reset the 'dest_addr' label. 2-byte operation!
clear_buffer_loop:
	CMP1 @dest_addr #1024		; Check if reached 1024 bytes
	JMP0001 clear_buffer_end	; If true, end loop
	MOV @@dest_addr #0		; Else clear that byte
	ADD1 @dest_addr #1		; And jump to the next one
	JMP clear_buffer_loop		; Jump back to the loop
clear_buffer_end:
	RET				; Return to the calling address

dest_addr:
	wb 0x00
	wb 0x00

srce_addr:
	wb 0x00
	wb 0x00
