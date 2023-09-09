; Prints a string stored in given address
;
; Arguments:
; ^ Addess of string 

_print_string:
	POP1 addr
print_string_loop:
	CMP @addr #0
	SIG #0x0000 @@addr
	ADD1 addr #1
	JMP010 print_string_loop
	RET


; Returns the length of the string
;
; Arguments:
; ^ Address of string

_strlen:
	POP1 addr
	MOV1 size #0
strlen_loop:
	CMP @addr #0
	JMP001 strlen_end
	ADD1 size #1
	ADD1 addr #1
	JMP strlen_loop
strlen_end:
	PSH11 @size
	RET

size:
	rb 2
addr:
	rb 2
