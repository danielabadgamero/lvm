; Prints a string stored in given address
;
; Arguments:
; ^ Addess of string 

_print_string:
	POP1 addr
print_string_loop:
	CMP @addr #0
	SET0000
	SIG #0 @@addr
	ADD1 addr #1
	JMP010 print_string_loop
	RET

addr:
	rb 2
