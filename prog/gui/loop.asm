_render_loop:
	SIG #0x0101			; Poll events
	SIG #0x0102			; Render everything
	SIG #0x0103			; Get state (windowShouldClose and keyboard state)
	CMP @#0 #1			; Check for windowShouldClose
	SIG #0x0106			; Get input text
	JMP0001 render_loop_end		; If true, end loop
	PSP1				; Start filling the stack with function parameters
	MOV ^ #0			; A pointer to the text of the label
	PSP1
	MOV ^ #0			; The id of the label
	PSP1
	MOV ^ #0x7f			; The position in x direction
	PSP1
	MOV ^ #0x7f			; The position in y direction
	PSP1
	MOV ^ #1			; The label should be x-aligned
	PSP1
	MOV ^ #1			; The label should be y-aligned
	CLR1 _render_label		; Call the label rendering function
	JMP _render_loop		; Jump back again
render_loop_end:
	CLR				; Jump back to calling address
