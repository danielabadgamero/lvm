; Set all bytes in a memory position up to a specified size to a given value.
;
; Arguments:
; ^ Value
; | Size
; | Start

_memset:
	POP val
	POP1 size
	POP1 start
	MOV1 count #0
memset_loop:
	CMP1 count @size
	JMP001 memset_end
	MOV @start @val
	ADD1 count #1
	JMP memset_loop
memset_end:
	RET

val:
	rb 1
count:
	rb 2
size:
	rb 2
start:
	rb 2


; Set 'size' bytes starting from 'start' to 0.
;
; Arguments:
; ^ Size
; | Start

_memclear:
	PSH #0
	CAL _memset
	RET


; Copy a given number of bytes from source address to destination address.
;
; Arguments:
; ^ Source address
; | Dest address
; | Size


_memcpy:
