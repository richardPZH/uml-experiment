#include<REG52.H>
#include<string.h>
#include<IMSutil.h>

#define TRANS 0
#define RECEI 1
#define ACK 0xAA

//read the port1 ant 0 bit
sbit type = P1^0;

//global evil variable, I should change my behavior
volatile unsigned long location;
volatile unsigned char idata *p;
volatile unsigned char whoAmI;


void initUart( void );

int main( void )
{
	char *hello = "Greeting From IMS@SCUT. rem";
	unsigned char cnt;

	//1200baud use timer1 
	initUart( );

	cnt = 0x4F - 0x30 + 1;
	location = 0;

	ES = 1; //enable uart interrupt
	EA = 1; //enable golbal inteerupt

	whoAmI = (unsigned char )type;

	if( TRANS == whoAmI )   //I am the transifer I will transfer 30H~4FH idata
	{
		//let the receiver get a little ready
		delay_uc( 50 );

		//first initial the 30H~4FH idata to the data I want
		p = 0x30;
		memcpy( p , hello , strlen(hello) );
		memset( p+strlen(hello) , 'L' , cnt - strlen(hello ));

		SBUF = p[location++];
		//use interrupt to transfer;

	}
	else					//I am the receiver I will receive an put at 50H~6FH
	{
		p = 0x50;
		memset( p , 'R' , cnt );

	}



	while( 1 );


	return 0;
}

//interrupter uart routine 
void uartInterrupt( void ) interrupt 4
{
	unsigned char rec;

	EA = 0;

	switch( whoAmI )
	{
		case RECEI : {
						 if( RI )
						 {
							 RI = 0;
							 p[location++] = SBUF;
							 SBUF = ACK; 
						 }else
						 {
							 TI = 0;
						 }

					 }
			;break;

		case TRANS : {
						 if( RI )
						 {
							 RI = 0;
							 rec = SBUF;
							 if( rec == ACK )
								 SBUF = p[location++];
						 }
						 else
						 {
							 TI = 0;
						 }

					 }
			;break;
		default : ;
	}

	EA = 1;
}

//use timer1 @mode1 @1200 baud
void initUart( void )
{
	TMOD = 0x20;
	TH1 = TL1 = 0xE8;
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;

	TR1 = 1;
}

