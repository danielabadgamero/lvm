_print_string:
	CLR1 clear_buffer
	MOV @addr #0
	MOV @addr+1 #0
print_string_loop:
	CMP @^ #0
	JMP0001 print_string_end
	MOV @@addr @^
	CMP @addr+1 #0xff
	JMP0010 add
	ADD @addr #1
add:
	ADD @addr+1 #1
	ADD ^ #1
	JMP print_string_loop
print_string_end:
	SIG #0x0002
	PSP
	CLR

clear_buffer:
	PSP1
clear_buffer_loop:
	CMP ^ #1024
	JMP0001 clear_buffer_end
	MOV @^ #0
	ADD ^ #1
	JMP clear_buffer_loop
clear_buffer_end:
	PSP
	CLR

addr:
	wb 0x00
addr+1:
	wb 0x00
