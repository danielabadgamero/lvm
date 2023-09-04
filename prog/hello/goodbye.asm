.goodbye:
	PSH goodbye_msg
	CAL _print_string
	HLT

goodbye_msg:
	ws Goodbye!\n\0
