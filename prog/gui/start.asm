.start:
	PSH window_name		; Push to the stack the address of 'window_name'
	CAL _write_string	; And write the string in that label to the beginning of the RAM
	POP1 addr		; Get the return value: size of 'window_name' string
	ADD1 @addr #1		; Next device arguments: window background color
	MOV1 @@addr #0x10	; Color Red
	ADD1 @addr #1		; Next color
	MOV1 @@addr #0x10	; Color Green
	ADD1 @addr #1		; Next color
	MOV1 @@addr #0x10	; Color Blue
	SIG #0x0100		; Initialize the GUI and create a window
	CAL _render_loop	; Let another function handle the render loop
	SIG #0x0104		; Close the window and free up resources used for the GUI
	HLT1			; Stop command execution without terminating the programme

addr:
	wb 0x00
	wb 0x00

window_name:
	ws Hello,\ GUI!\0
