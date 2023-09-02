.goodbye
	CLR1 _clear_buffer	; Call function to clear the first kilobyte in RAM
	PSP1			; Push an item to the stack
	MOV ^ goodbye_msg	; Set it to the address of 'goodbye_msg'
	CLR1 _print_string	; Print the message
	PSP			; Pop the previously added item from the stack
	HLT			; Terminate execution and exit the programme
	
goodbye_msg:
	ws Goodbye!\0
