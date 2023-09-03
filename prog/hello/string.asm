_print_string:
	CAL clear_buffer
	MOV1 @dest_addr #0
	POP1 srce_addr
print_string_loop:
	CMP @@srce_addr #0
	JMP0001 print_string_end
	MOV @@dest_addr @@srce_addr
	ADD1 @dest_addr #1
	ADD1 @srce_addr #1
	JMP print_string_loop
print_string_end:
	SIG #0x0002
	RET

clear_buffer:
	MOV1 @dest_addr #0
clear_buffer_loop:
	CMP1 @dest_addr #1024
	JMP0001 clear_buffer_end
	MOV @@dest_addr #0
	ADD1 @dest_addr #1
	JMP clear_buffer_loop
clear_buffer_end:
	RET

dest_addr:
	wb 0x00
	wb 0x00

srce_addr:
	wb 0x00
	wb 0x00
