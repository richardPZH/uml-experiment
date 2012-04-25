#include<REG52.H>
#include<absacc.h>

#define X (DBYTE[0x30])
#define Y (DBYTE[0x40])
#define Shang (DBYTE[0x50])
#define Remain (DBYTE[0x51])

int main( void )
{
	X = 0xAA;
	Y = 0x03;

	Shang = X/Y;
	Remain = X%Y;

	while( 1 )
	{
	}

	return 0;
}