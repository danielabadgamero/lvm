_render_label:
	MOV @aligny ^
	PSP
	MOV @alignx ^
	PSP
	MOV @y-pos ^
	PSP
	MOV @x-pos ^
	PSP
	MOV @id ^
	PSP
	CLR1 _write_string
	ADD ^ #1
	MOV @^ @id
	ADD ^ #1
	MOV @^ @x-pos
	ADD ^ #1
	MOV @^ @y-pos
	ADD ^ #1
	MOV @^ @alignx
	ADD ^ #1
	MOV @^ @aligny
	SIG #0x0105
	CLR

id:
	wb 0x00
x-pos:
	wb 0x00
y-pos:
	wb 0x00
alignx:
	wb 0x00
aligny:
	wb 0x00
