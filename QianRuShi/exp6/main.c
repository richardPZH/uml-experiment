

#if 0  
//This is a little program to test to compim componet we should search the net and find out that use the compim + virtual serial com drive + com assiatant
//So that the machine in the proteus can commucate with the host computer using the com map

#include<REG52.H>
#include<stdio.h>
#include<IMSutil.h>

void initUart( void );


int main( void )
{
	initUart();
	while( 1 )
	{
		delay_uc( 1000 );
		printf("Hello IMS!\n");
	}
	
	return 0;
}

void initUart( void )
{
	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 0xFD;
	TR1 = 1;
	TI = 1;
}

#endif
