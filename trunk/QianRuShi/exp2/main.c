#include<REG52.H>
#include<intrins.h>

//Global define switches to control the program behavior to satified the needs.
#define USE_MODULE1 1 
#define USE_MODULE2 0
#define USE_MODULE3 0

#define LED_PORT1 P1

#define DEAY_MS 150


//delay function, no timer engaged which means it's inaccurate.
void delay( unsigned int ucMs );

int main( void )
{

	unsigned char cycle;
	unsigned char ledStatus;


	//module 1 entrance
#if USE_MODULE1

	//choose the external trigger mode
	IT1 = 0; //Can we use bit addressing? It's 88H. Now that int0 int1 are edage trigger
	IT0 = 1; //Try it. Man

	//external interrupt enable
	EX0 = 1; //external 0 allowed
	EX1 = 1; //external 1 allowed

	//global interrupt enable
	EA = 1;  //Global interrupt on!

	while( 1 )
	{
		ledStatus = 0x01;
		LED_PORT1 = ~ledStatus;
		delay( DEAY_MS );

		for( cycle=1 ; cycle<8 ; cycle++ )
		{
			ledStatus <<=1;
			LED_PORT1 =~ledStatus;
			delay( DEAY_MS );
		}

	}

#endif

	//module 2 entrance
#if USE_MODULE2

	//choose the external trigger mode
	//IT1 = 0; //Can we use bit addressing? It's 88H. Now that int0 int1 are edage trigger
	IT0 = 1; //Try it. Man

	//external interrupt enable
	EX0 = 1; //external 0 allowed
	//EX1 = 1; //external 1 allowed

	//global interrupt enable
	EA = 1;  //Global interrupt on!

	ledStatus = 0x01;
	LED_PORT1 = ~ledStatus;
	delay( DEAY_MS );

	while( 1 )
	{

		for( cycle=1 ; cycle<8 ; cycle++ )
		{
		    ledStatus = crol( ledStatus , 1 );	
			LED_PORT1 =~ledStatus;
			delay( DEAY_MS );
		}

	}
#endif
	


	return 0;
}


//a delay function
void delay( unsigned int ucMs )
{
//running at 11.0592MHz ?
#define DELAYTIMES 239 

	unsigned char ucCounter;

	while( ucMs != 0 )
	{
		for( ucCounter=0; ucCounter < DELAYTIMES ; ucCounter ++ ){}
		ucMs--;
	}
}


#if USE_MODULE1

//external 0 interrupt edge trigger no need to clear the interrupt bit
void ext0( void ) interrupt 0
{
    unsigned char c;

	EA = 0; //no interrupt me

	for( c=0 ; c<3 ; c++ )
	{
		P0 = 0xF0;
		delay( DEAY_MS );
		P0 = 0xFF;
		delay( DEAY_MS );
	}
	
	EA = 1; //interrupt on
}

//external 1 interrupt edge trigger no need to clear the interrupt bit
void ext1( void ) interrupt 2
{
	unsigned char c;

	EA = 0; //no interrupt me

	for( c=0 ; c<6 ; c++ )
	{
		P2 = 0xF0;
		delay( 50 );
		P2 = 0xFF;
		delay( DEAY_MS );

	}


	EA = 1; //interrupt on
}

#endif

#if USE_MODULE2

#endif

