.start:
	PSH window_name		; Push to the stack the address of 'window_name'
	CAL _write_string	; And write the string in that label to the beginning of the RAM
	ADD ^ #1		; Next device arguments: window background color
	MOV @^ #0x10		; Color Red
	ADD ^ #1		; Next color
	MOV @^ #0x10		; Color Green
	ADD ^ #1		; Next color
	MOV @^ #0x10		; Color Blue
	POP #0xffff		; Pop the previously added item from the stack
	SIG #0x0100		; Initialize the GUI and create a window
	CAL _render_loop	; Let another function handle the render loop
	SIG #0x0104		; Close the window and free up resources used for the GUI
	HLT1			; Stop command execution without terminating the programme

window_name:
	ws Hello,\ GUI!\0
