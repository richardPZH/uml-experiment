#include<REG52.H>

#define COUNTER0_C 3
#define COUNTER1_C 6
#define EightMax 256

#define LED_PORT1 P0
#define LED_PORT2 P2

volatile unsigned char tc0;
volatile unsigned char tc1;

void time( unsigned int ucMs );

int main( void )
{
	LED_PORT1 = 0xFF;
	LED_PORT2 = 0xFF;

	P1 = 0x00; 

	TMOD = 0x66;						 //counter1 counter2 use extern t1 t0 count and automataclly reload
	TH1 = TL1 = EightMax - COUNTER1_C;   //TH1 TL1 with initial value to go
	TH0 = TL0 =	EightMax - COUNTER0_C;   //TH0 TL0 with initial value

	//interrupt control
	ET0 = 1;       //enable timer0 interrupt
	ET1 = 1;       //enable timer1 interrupt

	EA = 1;        //enable global interrupt

	TR0 = 1;       //both start counting
	TR1 = 1;

	tc0 = tc1 = 0;
	while( 1 )
	{
	}

	return 0;
}

//timer/counter0 interrupt function
void counter0( void ) interrupt 1
{
	unsigned char c;
	EA = 0;

	tc0++;
	P1 = ( tc0 << 4 ) | ( tc1 & 0x0F );

	for( c=0 ; c<3 ; c++ )
	{
		LED_PORT1 = 0xF0;
		time( 150 );
		LED_PORT1 = 0xFF;
		time( 150 );
	}

	EA = 1;
}

//timer/counter1 interrupt function
void counter1( void ) interrupt 3
{
	unsigned char c;
	EA = 0;

	tc1++;
	P1 = ( tc0 << 4 ) | ( tc1 & 0x0F );

	for( c=0 ; c<6 ; c++ )
	{
		LED_PORT2 = 0xF0;
		time( 150 );
		LED_PORT2 = 0xFF;
		time( 150 );
	}

	EA = 1;	
}

void time( unsigned int ucMs )
{
#define DELAYTIMES 239
    unsigned char ucCounter;

    while( ucMs != 0 )
    {
	for( ucCounter = 0 ; ucCounter < DELAYTIMES; ucCounter++ ){}
	ucMs--;
    }
}
