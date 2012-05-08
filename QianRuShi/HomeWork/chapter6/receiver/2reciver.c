#include<REG52.H>

volatile unsigned long c;

//use the timer1 work at 19200 baud and mode is 01 
void initUart( void );

unsigned char xdata xArrayAt4000[0xFF + 1] _at_ 0x4000;

int main( void )
{
	initUart();

	//wait to receive data
	c = 0;
	ES = 1;
	EA = 1;

	while( 1 );

	return 0;
}

//use timer1 @mode1 19200 baud and no parity
void initUart( void )
{
	TMOD = 0x20;
	PCON = 0x80;
	TH1 = TL1 = 0xFD;
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;

	TR1 = 1;

}

//uart interrupt routine
void uartInterrupt( void ) interrupt 4
{
	EA = 0;

	if( RI )       //receive interrupt
	{
		RI = 0;

		if( c <= 0xFF )
		{
			xArrayAt4000[c] = SBUF;
			c++;
		}
	}
	else           //transfer inteerupt
	{
		TI = 0;
	}

	EA = 1;
}

