.hello:
	PSP1
	MOV ^ title
	CLR1 write_string
	SUB ^ title
	ADD ^ #1
	MOV @^ #0x10
	ADD ^ #1
	MOV @^ #0x10
	ADD ^ #1
	MOV @^ #0x10
	PSP
	SIG #0x0100
loop:
	SIG #0x0101
	SIG #0x0102
	SIG #0x0103
	CMP @#0 #1
	JMP0001 end

	CMP @#54 #1
	JMP0010 decel-rel
	CMP @vel #0xff
	JMP0001 no-decel
	CMP @d-rel #0
	JMP0001 no-decel
	MOV @d-rel #0
	ADD @vel #1
	JMP no-decel
decel-rel:
	MOV @d-rel #1
no-decel:
	CMP @#55 #1
	JMP0010 accel-rel
	CMP @vel #0xdf
	JMP0001 no-accel
	CMP @a-rel #0
	JMP0001 no-accel
	MOV @a-rel #0
	SUB @vel #1
	JMP no-accel
accel-rel:
	MOV @a-rel #1
no-accel:
	CMP @#79 #1
	JMP0010 no-right
	ADD @x-pos+1 #1
	CMP @x-pos+1 @vel
	JMP0110 no-right
	ADD @x-pos #1
no-right:
	CMP @#80 #1
	JMP0010 no-left
	SUB @x-pos+1 #1
	CMP @x-pos+1 @vel
	JMP0110 no-left
	SUB @x-pos #1
no-left:
	CMP @#81 #1
	JMP0010 no-down
	ADD @y-pos+1 #1
	CMP @y-pos+1 @vel
	JMP0110 no-down
	ADD @y-pos #1
no-down:
	CMP @#82 #1
	JMP0010 no-up
	SUB @y-pos+1 #1
	CMP @y-pos+1 @vel
	JMP0110 no-up
	SUB @y-pos #1
no-up:
	PSP1
	MOV ^ title
	CLR1 write_string
	SUB ^ title
	MOV @^ #0x20
	ADD ^ #1

	MOV @buf @vel
	DIV @buf #16
	CMP @buf #0xa
	JMP0110 is_num
	ADD @buf #0x27
is_num:
	ADD @buf #0x30
	MOV @^ @buf
	ADD ^ #1
	MOV @buf @vel
	AND @buf #0xf	
	CMP @buf #0xa
	JMP0110 is_num_2
	ADD @buf #0x27
is_num_2:
	ADD @buf #0x30
	MOV @^ @buf

	ADD ^ #2
	MOV @^ #0

	ADD ^ #1
	MOV @^ @x-pos

	ADD ^ #1
	MOV @^ @y-pos

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
buf:
	wb 0x00
title:
	ws Hello,\ World!\0
x-pos:
	wb 0x00
x-pos+1:
	wb 0x00
y-pos:
	wb 0x00
y-pos+1:
	wb 0x00
vel:
	wb 0xff
a-rel:
	wb 0x00
d-rel:
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
