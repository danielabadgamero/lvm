.goodbye:
	PSP1			; Push an item to the stack
	MOV ^ goodbye_msg	; Set it to the address of 'goodbye_msg'
	CLR1 _print_string	; Print the message
	HLT			; Terminate execution and exit the programme

goodbye_msg:
	ws Goodbye!\0
