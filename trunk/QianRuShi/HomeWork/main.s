DATA_ADDR	EQU 50H   ;���ݿ��׵�ַ
COUNT		EQU 0AH   ;ѭ������
VAL			EQU 0AAH  ;��ֵ������

	ORG		0000H
	LJMP	MAIN	

;****************
;*
;*
;* Main
;*
;*
;****************

	ORG	100H
MAIN:
	CLR C
	CLR RS1
	CLR RS0
	MOV A,#38H
	MOV R0,A
	MOV 29H,R0
	SETB RS0
	MOV R1,A
	MOV 26H,A
	MOV 28H,C
	SJMP $
END
