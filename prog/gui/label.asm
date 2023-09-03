_render_label:
	POP aligny		; Get the arguments (in reverse order!)
	POP alignx
	POP y-pos
	POP x-pos
	POP id
	CAL _write_string	; Write the text of the label to first bytes in RAM
	POP1 addr		; Get the length of the label text
	CMP1 @addr #1018	; Check if the text is too big
	JMP110 ok		; All ok if it's lower
	MOV1 @addr #1018	; If it is too big, set it to the max value. Our label text will be truncated. Note that this is just to prevent crashes, and to present
; a possible way of handling max buffer sizes. In reality, a label longer than a couple hundred characters won't fit on the screen (1920x1080)
ok:
	MOV @@addr #0		; Ensure there is a null character after the text
	ADD1 @addr #1		; Fill in the other parameters
	MOV @@addr @id
	ADD1 @addr #1
	MOV @@addr @x-pos
	ADD1 @addr #1
	MOV @@addr @y-pos
	ADD1 @addr #1
	MOV @@addr @alignx
	ADD1 @addr #1
	MOV @@addr @aligny
	SIG #0x0105		; And tell the graphics device to create / modify the (existing) label
	RET

addr:
	wb 0x00
	wb 0x00

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
