_render_label:
	POP @aligny		; Get the arguments (in reverse order!)
	POP @alignx
	POP @y-pos
	POP @x-pos
	POP @id
	CAL _write_string	; Write the text of the label to first bytes in RAM
	ADD ^ #1		; Fill in the other parameters
	MOV @^ @id
	ADD ^ #1
	MOV @^ @x-pos
	ADD ^ #1
	MOV @^ @y-pos
	ADD ^ #1
	MOV @^ @alignx
	ADD ^ #1
	MOV @^ @aligny
	SIG #0x0105		; And tell the graphics device to create / modify the (existing) label
	RET

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
