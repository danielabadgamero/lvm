.hello:
	PSP1
	MOV ^ title
	CLR1 write_string
	PSP
	SIG #0x0100
loop:
	SIG #0x0101
	SIG #0x0102
	SIG #0x0103
	CMP @#0 #1
	JMP0001 end
	JMP loop
end:
	SIG #0x0104
	HLT1

title:
	ws Hello,\ World!\0

.exit:
	HLT

write_string:
	MOV @ws_pc+1 ^
	DIV ^ #256
	MOV @ws_pc ^
	PSP
ws_loop:
	CMP @^ #0
	JMP1 ws_end
	MOV @@ws_addr @^
	ADD ^ #1
	ADD @ws_addr+1 #1
	JMP ws_loop
ws_end:
	PSP1
	MOV ^ @ws_pc
	MUL ^ #256
	NOT ^
	NOT @ws_pc+1
	AND ^ @ws_pc+1
	NOT ^
	CLR
ws_addr:
	wb 0x00
ws_addr+1:
	wb 0x00
ws_pc:
	wb 0x00
ws_pc+1:
	wb 0x00
