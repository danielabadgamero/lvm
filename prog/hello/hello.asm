.hello:
	PSH hello_msg		; Push an item to the stack with the address of the label 'hello_msg'`
	CAL _print_string	; Call function to print the string
	HLT1			; Stop execution without terminating the programme

hello_msg:
	ws Hello,\ World!\0
