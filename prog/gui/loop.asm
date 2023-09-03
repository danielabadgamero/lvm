_render_loop:
	SIG #0x0101			; Poll events
	SIG #0x0102			; Render everything
	SIG #0x0103			; Get state (windowShouldClose and keyboard state)
	CMP @#0 #1			; Check for windowShouldClose
	JMP0001 render_loop_end		; If true, end loop
	SIG #0x0106			; Get input text
	PSH #0				; Start filling the stack with function parameters, first a pointer to the text of the label
	PSH #0				; The id of the label
	PSH #0x80			; The position in x direction
	PSH #0x80			; The position in y direction
	PSH #1				; The label should be x-aligned
	PSH #1				; The label should be y-aligned
	CAL _render_label		; Call the label rendering function
	JMP _render_loop		; Jump back again
render_loop_end:
	RET				; Jump back to calling address
