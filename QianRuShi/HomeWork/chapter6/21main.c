#include<REG52.H>
#include"IMSutil.h"


//use the timer1 work at 19200 baud and mode is 2 with even parity 
void initUart( void );

void sendByte( unsigned char b );

unsigned char xdata xArrayAt3000[0xFF + 1] _at_ 0x3000;

int main( void )
{
	unsigned long c;

	initUart();

	delay_uc( 100 );//let other slave get ready

	//initial the data _at_ 0x3000

	for( c=0; c<=0xFF; c++ )
	{
		xArrayAt3000[c] = c;
	}

	//use the query to send data with even parity eagaged
	for( c=0 ; c<= 0xFF; c++ )
	{
		sendByte( xArrayAt3000[c] );
		//delay_uc( 100 );
	}

	while( 1 );

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

}

//send a byte with TB using the even parity
void sendByte( unsigned char b )
{
	unsigned char sc;
	unsigned char cnt;
	unsigned char bs;

	TI = 0;

	sc = 0;
	bs = b;
	for( cnt=0 ; cnt<8 ; cnt++ )
	{
		sc += ( bs>>cnt ) & 0x01;
	}

	if( ( sc % 2) == 0 )
	{
		TB8 = 0;
	}else
	{
		TB8 = 1;
	}

	SBUF = b;
	while( !TI );
	TI = 0;
}

