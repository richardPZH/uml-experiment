#include<REG52.H>

//initial uart run at 19200 baud @11.0592Mhz
void ini_Uart( void );

//send a byte
void sendByte( unsigned char b );

int main( void )
{
	ini_Uart();


	ES = 1;
	TI = 0;
	RI = 0;
	EA = 1;

	while( 1 );

	return 0;
}

void usarInterrupt( void ) interrupt 4
{
	unsigned char tmp;

	EA = 0;

	tmp = SBUF;

	if( TI )
	{
		TI = 0;
	}else
	{
		RI = 0;

		if( tmp >= 'a' && tmp <= 'z' )
		{
			tmp = tmp - 'a' + 'A';
		}

		//Here I can't just call the sendByte directroy 
		//If it supports the inline key word, it may work, because the interrupt function must work efficiently!!!
		SBUF = tmp;
		while( !TI );
		TI = 0;
		//some how this function must be efficient, enough may be call a function is too expensive for 19200 baud!
		//
	}

	EA = 1;
}

//send a byte  
void sendByte( unsigned char b )
{
	TI = 0;
	SBUF = b;
	while( !TI );
	TI = 0;
}

//initial uart run at 19200 baud @11.0592Mhz //use the time1 to generage baud
void ini_Uart( void )
{
	TMOD = 0x20;
	PCON = 0x80;	
	TH1 = TL1 = 0xFD;
	TR1 = 1;

	SCON = 0x50;
	TI = 1;
}

