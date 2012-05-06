
//A delay function used to work at 11.0952MHz
void delay_uc( unsigned int ucMs )
{
#define DELAYTIMES 239
    unsigned char ucCounter;

    while( ucMs != 0 )
    {
	for( ucCounter = 0 ; ucCounter < DELAYTIMES; ucCounter++ ){}
	ucMs--;
    }
}





