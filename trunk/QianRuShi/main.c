#include<REG52.H>
#include<intrins.h>

#define LED_PORT1 P1
#define LED_PORT2 P0
#define LED_PORT3 P3
#define LED_PORT4 P2

#define GET_PORT( n , led ) ( (led >> ( 8 * n - 8 )) & 0xFF )

//delay function no timer in use delay for micro seconds
void time( unsigned int ucMs );

//update the ledstatus to the port. Call it Map? Well..
void updateLedStatus( unsigned long ledStatus );

int main( void )
{
	
	unsigned long ledStatus;
	unsigned char cycle;

	LED_PORT1 = 0xFF;
	LED_PORT2 = 0xFF;
	LED_PORT3 = 0xFF;
	LED_PORT4 = 0XFF;

	while( 1 )
	{
	    time( 200 ); //holds for 200 micro seconds

	    //Start (1) VD1-VD32 on one led one time
	    ledStatus = 0x01;
	    updateLedStatus( ledStatus );
	    time( 200 );

	    for( cycle=1 ; cycle < 32 ; cycle++ )
	    {
		ledStatus = _lrol_( ledStatus , 1 ); //long left shift
	        updateLedStatus( ledStatus );	     //update to port
	        time( 200 );			     //delay 200ms

	    }

	    //start (2) VD32 on and hold and VD31 ...VD0
	    ledStatus = 0x00; 
	    updateLedStatus( ledStatus );
	    time( 200 );

	    for( cycle=1 ; cycle <= 32 ; cycle++ )
	    {
		ledStatus = _lror_( ledStatus , 1 );
			ledStatus |= 0x80000000;  //unsigned long for 32bits unsafe
			updateLedStatus( ledStatus );
			time( 200 );
	    }

	    //start (3) VD1 ~ VD32 off
		//ledStatus = 0xFFFFFFFE;
	    for( cycle=1 ; cycle <= 32 ; cycle++ )
	    {
		ledStatus = _lrol_( ledStatus , 1 ); //long left shift
		ledStatus &= ~ 0x01;
			updateLedStatus( ledStatus );
			time( 200 );
	    }



	}

	return 0;
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

void updateLedStatus( unsigned long ledStatus )
{
    LED_PORT1 = ~( GET_PORT( 1 , ledStatus ) );
    LED_PORT2 = ~( GET_PORT( 2 , ledStatus ) );
    LED_PORT3 = ~( GET_PORT( 3 , ledStatus ) );
    LED_PORT4 = ~( GET_PORT( 4 , ledStatus ) );

}
