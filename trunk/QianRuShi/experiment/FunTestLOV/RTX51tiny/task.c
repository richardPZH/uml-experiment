#include<rtx51tny.h>
#include<REG52.H>

//also need to find the Conf_tny.A51 to local directory

sbit led1 = P2^0;
sbit led2 = P2^1;
sbit led3 = P2^2;
sbit led4 = P2^3;
sbit led5 = P2^4;
sbit led6 = P2^5;
sbit led7 = P2^6;
sbit led8 = P2^7;

void init( void ) _task_ 0
{
	os_create_task( 1 );
	os_create_task( 2 );
	os_create_task( 3 );
	os_create_task( 4 );
	os_create_task( 5 );
	os_create_task( 6 );
	os_create_task( 7 );

	while( 1 )
	{
		led8 = ! led8;
		os_wait( K_TMO , 255 , 0 );
	}

}

void ledTask_1( void ) _task_ 1
{
	while( 1 )
	{
		led1 = ! led1;
		os_wait( K_TMO , 8 , 0 );
	}
}

void ledTask_2( void ) _task_ 2
{
	while( 1 )
	{
		led2 = ! led2;
		os_wait( K_TMO , 10 , 0 );
	}
}

void ledTask_3( void ) _task_ 3
{
	while( 1 )
	{
		led3 = ! led3;
		os_wait( K_TMO , 15 , 0 );
	}
}

void ledTask_4( void ) _task_ 4
{
	while( 1 )
	{
		led4 = ! led4;
		os_wait( K_TMO , 25 , 0 );
	}
}


void ledTask_5( void ) _task_ 5
{
	while( 1 )
	{
		led5 = ! led5;
		os_wait( K_TMO , 50 , 0 );
	}
}


void ledTask_6( void ) _task_ 6
{
	while( 1 )
	{
		led6 = ! led6;
		os_wait( K_TMO , 100 , 0 );
	}
}


void ledTask_7( void ) _task_ 7
{
	while( 1 )
	{
		led7 = ! led7;
		os_wait( K_TMO , 200 , 0 );
	}
}

