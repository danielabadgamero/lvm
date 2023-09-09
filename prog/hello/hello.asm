_main:
	PSH hello_msg
	CAL _print_string
	HLT

hello_msg:
	ws Hello,\ World!\n\0
