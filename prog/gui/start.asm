.start:
	PSP1			; Push an empty item to the stack
	MOV ^ window_name	; Set the stack to the address of 'window_name'
	CLR1 _write_string	; And write the string in that label to the beginning of the RAM
	ADD ^ #1		; Next device arguments: window background color
	MOV @^ #0x10		; Color Red
	ADD ^ #1		; Next color
	MOV @^ #0x10		; Color Green
	ADD ^ #1		; Next color
	MOV @^ #0x10		; Color Blue
	PSP			; Pop the previously added item from the stack
	SIG #0x0100		; Initialize the GUI and create a window
	CLR1 _render_loop	; Let another function handle the render loop
	SIG #0x0104		; Close the window and free up resources used for the GUI
	HLT1			; Stop command execution without terminating the programme

window_name:
	ws Hello,\ GUI!\0
