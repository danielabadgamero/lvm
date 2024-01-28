_test:
	CIN
	ADD #1 -> %1
	COUT
	EQ 'h', %1
	JMT end
end:
	HALT
