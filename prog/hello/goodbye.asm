.goodbye:
	PSH goodbye_msg		; Push the address of 'goodbye_msg' to the stack
	CAL _print_string	; Print the message
	HLT			; Terminate execution and exit the programme

goodbye_msg:
	ws Goodbye!\0
