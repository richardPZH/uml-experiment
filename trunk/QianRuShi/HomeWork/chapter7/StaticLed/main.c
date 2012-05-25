#include<REG52.H>

/*
 *
 * The 7SEG cc / CA  has been verified by hand
 * IMS @ SCUT 2012/05/25 I will tell you.
*/

#define LED P2

int main( void )
{
	LED = 0x6f;

	while( 1 )
	{
	}



	return 0;
}


