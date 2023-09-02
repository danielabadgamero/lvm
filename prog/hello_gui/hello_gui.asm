.hello:
	PSP1
	MOV ^ title
	CLR1 write_string
	SUB ^ title
	ADD ^ #1
	MOV @^ #0
	ADD ^ #1
	MOV @^ #0
	ADD ^ #1
	MOV @^ #0
	PSP
	SIG #0x0100
loop:
	SIG #0x0101
	SIG #0x0102
	SIG #0x0103
	CMP @#0 #1
	JMP0001 end

	PSP1
	MOV ^ title
	CLR1 write_string
	SUB ^ title

	ADD ^ #1
	MOV @^ #0

	ADD @counter #1
	CMP @counter #0x7f
	JMP0010 next
	MOV @counter #0
	ADD @x-pos #1

next:
	ADD ^ #1
	MOV @^ @x-pos

	ADD ^ #1
	MOV @^ #0x7f

	ADD ^ #1
	MOV @^ #1

	ADD ^ #1
	MOV @^ #1

	SIG #0x0105
	PSP

	JMP loop
end:
	SIG #0x0104
	HLT1

title:
	ws Hello,\ World!\0
counter:
	wb 0x00
x-pos:
	wb 0x00

.exit:
	HLT

write_string:
	MOV @ws_addr #0
	MOV @ws_addr+1 #0
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
