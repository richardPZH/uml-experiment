VAL			EQU 0AAH  ;¸³Öµ£Á£Á£È

	ORG		0000H
	LJMP	MAIN	


;
;Main entrance...
;
	ORG	100H
MAIN:
	MOV 30H,#VAL  ;30h -> 0AAH
	MOV 40H,#03H  ;40h -> 02H
	MOV A,30H
	MOV B,40H
	DIV AB		  ;A/B -> A , B
	MOV 50H,A
	MOV 51H,B
	SJMP $
END
