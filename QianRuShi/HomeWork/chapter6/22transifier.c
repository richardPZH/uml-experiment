#include<REG52.H>
#include<IMSutil.h>

volatile unsigned long c;

//use the timer1 work at 19200 baud and mode is 1 no parity 
void initUart( void );


unsigned char xdata xArrayAt3000[0xFF + 1] _at_ 0x3000;

int main( void )
{

	initUart();


	delay_uc( 100 );//let other slave get ready

	//initial the data _at_ 0x3000
	for( c=0; c<=0xFF; c++ )
	{
		xArrayAt3000[c] = c;
	}

	c = 0;

	ES = 1;
	EA = 1;

	TI = 1;  //software trigger the transfier finish
		
	while( 1 );

	return 0;
}

//use timer1 @mode1 run @19200 baud
void initUart( void )
{
	TMOD = 0x20;
	PCON = 0x80;
	TH1 = TL1 = 0xFD;
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;

	TR1 = 1;   //timer1 start 
}

//the transfier routine
void uartInterrupt( void ) interrupt 4
{
	if( TI )  //it's a transfer interrupt
	{
		TI = 0;

		if( c <= 0xFF )
		{
			SBUF = xArrayAt3000[c];
			c++;
		}

	}
	else      //it's a recever interrupt
	{
		RI = 0;
	}

}
