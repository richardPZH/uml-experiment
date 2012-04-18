#include<REG52.H>
#include<intrins.h>

//Global define switches to control the program behavior to satified the needs.
#define USE_MODULE1 0 
#define USE_MODULE2 0
#define USE_MODULE3 1

#if ( USE_MODULE1 + USE_MODULE2 + USE_MODULE3 ) > 1 || ( USE_MODULE1 + USE_MODULE2 + USE_MODULE3 ) == 0
#error "error of switches. Try to modify the USE_MODULE1 or USE_MODULE2 or USE_MODULE3" //only 001 010 100 is acceptable
#endif


#define LED_PORT1 P1


//delay function, no timer engaged which means it's inaccurate.
void delay( unsigned int ucMs );


//module 1 entrance
#if USE_MODULE1

#define DEAY_MS 150

int main( void )
{

	unsigned char cycle;
	unsigned char ledStatus;

	//choose the external trigger mode
	IT1 = 0; //Can we use bit addressing? It's 88H. Now that int0 int1 are edage trigger
	IT0 = 1; //Try it. Man

	//external interrupt enable
	EX0 = 1; //external 0 allowed
	EX1 = 1; //external 1 allowed

	//global interrupt enable
	EA = 1;  //Global interrupt on!

	//initial off all the leds
	P0 = 0xFF;
	P2 = 0xFF;
	LED_PORT1 = 0xFF;

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


	return 0;
}

#endif


//module 2 entrance
#if USE_MODULE2

#define DEAY_MS 250

volatile bit mode;                  //evil global variable, indicate the mode we use here
volatile unsigned long ledStatus;   //We handle with care, But is there a risk of race like that in Operaton System?

int main( void )
{
	//choose the external trigger mode
	//Can we use bit addressing? It's 88H. Now that int0 edage trigger
	IT0 = 1; //Try it. Man

	//external interrupt enable
	EX0 = 1; //external 0 allowed

	//global interrupt enable
	EA = 1;  //Global interrupt on!
	

	//initial off all the leds
	P0 = 0xFF;
	P2 = 0xFF;
	LED_PORT1 = 0xFF;

	ledStatus = 0x01;
	mode = 0;
	LED_PORT1 = ~ledStatus;
	delay( DEAY_MS );

	while( 1 )
	{
	    ledStatus = _crol_( ledStatus , 1 + ( unsigned char )mode );	  //C51 libary provide a lot of useful function. Shall Google it...
		LED_PORT1 =~ledStatus;
		delay( DEAY_MS );
	}

	return 0;
}

#endif


#if USE_MODULE3

#define DEAY_MS 180


volatile unsigned char ledStatus;    //change the ledStatus
volatile mode;                       //has been interrupted?

int main( void )
{

	unsigned char cycle;

	//off all the leds
	P0 = 0xFF;
	P2 = 0xFF;
	LED_PORT1 = 0xFF;

	//choose the external trigger mode
	//Can we use bit addressing? It's 88H. Now that int1 edage trigger
	IT1 = 1; //Try it. Man

	//external interrupt enable
	EX1 = 1; //external 1 allowed

	//global interrupt enable
	EA = 1;  //Global interrupt on!

	mode = 0;

	while( 1 )
	{
		if( ! mode )
		{
			ledStatus = 0xFF;
			P0 = 0xFF;
			P2 = 0xFF;

			LED_PORT1 = ~ledStatus;
			delay( DEAY_MS );

			for( cycle=0 ; cycle < 8 ; cycle++ )
			{
				ledStatus >>= 1;
				LED_PORT1 = ~ledStatus;
				delay( DEAY_MS );
			}
		}
		else{

			ledStatus = 0x03;
			LED_PORT1 = ~ledStatus;
			P0 = 0xF0;
			P2 = 0xFF;
			delay( DEAY_MS );

			for( cycle=0 ; cycle < 4 ; cycle++ )
			{
				ledStatus <<=2;

				LED_PORT1 = ~ledStatus;
				P0 = ~P0;
				P2 = ~P2;
				delay( DEAY_MS );
			}


		}

	}


	return 0;

}

#endif
	


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

//external 1 interrupt level trigger no much we can do if the levle is still on 
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

//external 0 interrupt edge trigger no need to clear the interrupt bit
void etx0( void ) interrupt 0
{
	EA = 0; //no interrupt me

	if( mode )   // mode == 1 means current is two leds, next status one led
	{
		ledStatus = ledStatus & _crol_( ledStatus , 1 ) ;      //must use the  char rotate in a circle style
	}
	else         // mode == 0 means current is one led, next status two leds
	{
		ledStatus = ledStatus | _crol_( ledStatus , 1 ) ;      //must use the char rotate to get a circle
	}

	mode = ~mode;

	EA = 1; //global interrupt on again
}

#endif


#if USE_MODULE3

//external 1 interrupt edge trigger no need to clear the interrupt bit
void ext1( void ) interrupt 2
{
	EA = 0; //global interrupt off

	mode = ~mode;

	EA = 1; //global interrupt on
}

#endif


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

