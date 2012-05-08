#include<REG52.H>
#include<stdio.h>

//use the timer1 work at 19200 baud and mode is 2 with even parity 
void initUart( void );

unsigned char receiveByte( void );

unsigned char xdata xArrayAt4000[0xFF + 1] _at_ 0x4000;

int main( void )
{
	unsigned long c;
	unsigned char recByte;

	initUart();

	//wait to receive data
	c = 0;
	RI = 0;
	while( 1 )
	{
		recByte = receiveByte();
		xArrayAt4000[c++] = recByte;
	}

	return 0;
}

//use timer1 @mode2
void initUart( void )
{
	TMOD = 0x20;
	PCON = 0x80;
	TH1 = TL1 = 0xFD;
	SM0 = 1;
	SM1 = 1;
	SM2 = 0;
	REN = 1;

	TR1 = 1;

	TI = 1; //to get the printf work

}

unsigned char receiveByte( void )
{
	unsigned char rec;
	unsigned char sc;
	unsigned char cnt;
	bit rb;

	while( !RI );
	RI = 0;
	

	rb = RB8;
	rec = SBUF;



	sc = 0;
	for( cnt=0 ; cnt<8 ; cnt++ )
	{
		sc += ( rec>>cnt ) & 0x01;
	}

	if( rb )
		sc++;
	
	if( ( sc % 2  ) != 0 )
	{
		//printf("Receive error!! even parity fail\n");
		return 0xff;
	}

	
	return rec;
}






