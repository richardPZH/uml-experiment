#include<REG52.H>
#include"IMSutil.h"

/*
 *
 * The 7SEG cc / CA  has been verified by hand
 * IMS @ SCUT 2012/05/25 I will tell you.
*/

#define LED P2
#define LED_NUM 8

sbit i_clk = P1^6;
sbit i_data = P1^7;

code unsigned char str[][LED_NUM] =
{
	{ 0x76 , 0x79 , 0x38 , 0x38 , 0x3f , 0x40 , 0x40 , 0x40 },   //HELLO---
	{ 0x3f , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d , 0x7d , 0x07 },   //01234567
	{ 0x77 , 0x7c , 0x39 , 0x5e , 0x79 , 0x71 , 0x73 , 0x71 },   
	{ 0x76 , 0x06 , 0x38 , 0x38 , 0x3f , 0x06 , 0x5b , 0x4f }, 
	{ 0x38 , 0x40 , 0x38 , 0x40 , 0x07 , 0x40 , 0x07 , 0x40 },
};

int main( void )
{
	//LED = 0x6f;
	
	unsigned char cyc;
	unsigned char mode;

	unsigned char i;
	unsigned char j;
	unsigned char k;

	cyc = sizeof( str ) / LED_NUM;

	i_clk = 0;  //initial the clk to low level;

	while( 1 )
	{
		for( i=0 ; i<cyc; i++ )
		{
			for( j=0 ; j < LED_NUM ; j++ )
			{
				mode = str[i][ LED_NUM - 1 - j ];
				for( k=0 ; k<8 ; k++ )
				{
					i_data = ( mode >> ( 7 - k )) & 0x01;
					//delay_50us();
					i_clk = 1;
					//delay_5us();
					i_clk = 0;
					//delay_50us();
				}

			}

			delay_ms( 5000 );
		}
	}



	return 0;
}


