.hello:
	CLR1 _clear_buffer	; Call function to clear the first kilobyte in RAM
	PSP1			; Push an item to the stack
	MOV ^ hello_msg		; Set it to the address of the string in 'hello_msg'
	CLR1 _print_string	; Call function to print the string
	PSP			; Pop the previously added item from the stack
	HLT1			; Stop execution without terminating the programme

hello_msg:
	ws Hello,\ World!\0
