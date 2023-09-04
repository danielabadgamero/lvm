.hello:
	PSH hello_msg
	CAL _print_string
	HLT1

hello_msg:
	ws Hello,\ World!\n\0
