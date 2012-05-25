#include "IMSutil.h"
#include <intrins.h>

//A delay function used to work at 11.0952MHz
void delay_ms( unsigned int ucMs )
{
#define DELAYTIMES 239
    unsigned char ucCounter;

    while( ucMs != 0 )
    {
		for( ucCounter = 0 ; ucCounter < DELAYTIMES; ucCounter++ ){}
		ucMs--;
    }
}

//These functions are designed at 11.0592MHz or( 4 _nop_() when 22.1184 )
//the delay has to use the cpu time and it's inaccurace. So don't use it when time is requite strictly
//because the function call and c will generate assemble code much unexcepted
//
void delay_5us( void )
{
	_nop_();
	_nop_();
	//_nop_();
	//_nop();
}

void delay_50us( void )
{
	unsigned char i;
	for( i=0; i<4 ; i++ )
	{
		delay_5us();
	}
}

void delay_100us( void )
{
	delay_50us();
	delay_50us();
}



