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
	SETB RS1
	MOV SP,#4FH 
	MOV R0,#DATA_ADDR	;���ݿ��׵�ַDATA_ADDR ��ֵ����ַָ��R0
	MOV R2,#COUNT		;ѭ������0AH����������R2

LOOP:
	MOV @R0, #VAL		;����R0����Ԫ��ֵVAL
	INC R0				;��ַָ��R0��1
	DEC	R2				;������R2��1
	CJNE R2,#00H,LOOP	;�ж�R2�Ƿ�Ϊ0����0תLOOP������0
	SJMP $
END
