DATA_ADDR	EQU 50H   ;数据块首地址
COUNT		EQU 0AH   ;循环次数
VAL			EQU 0AAH  ;赋值ＡＡＨ

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
	MOV R0,#DATA_ADDR	;数据块首地址DATA_ADDR 赋值给地址指针R0
	MOV R2,#COUNT		;循环次数0AH付给计数器R2

LOOP:
	MOV @R0, #VAL		;给（R0）单元赋值VAL
	INC R0				;地址指针R0加1
	DEC	R2				;计算器R2减1
	CJNE R2,#00H,LOOP	;判断R2是否为0，非0转LOOP继续请0
	SJMP $
END
