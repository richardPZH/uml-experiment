#include<REG52.H>
#include<IMSutil.h>

#define RUNNING_COUNTER 0
#define BASKETBALL_COUNTER 1
#define KEY_FILTER_MS 2

#define RUNNING_INITIAL_10MS 0	
#define BASKETBALL_ROUND_TIMEUP_10MS 72000

#define MINUTE_LED P3
#define SECOND_LED P2
#define MICRO_10MS_LED P1


sbit bStart = P0^0;
sbit bPauseContinue = P0^1;
sbit outPut = P0^7;
bit running;

sbit bClear = P0^2;
sbit bExit = P0^3;

#if RUNNING_COUNTER || BASKETBALL_COUNTER 
//unsigned long is 32 bit...good
volatile unsigned long currentTicksIn10ms;

//set the timer 2 interrupt at every 10ms
void setTimer2_10ms( void );

//update the 7-SEG led using the global ticks
void update7SEG( void );

int main( void )
{
	running = 0;
	outPut = 1;

#if RUNNING_COUNTER
	currentTicksIn10ms = RUNNING_INITIAL_10MS;
#elif BASKETBALL_COUNTER
	currentTicksIn10ms = BASKETBALL_ROUND_TIMEUP_10MS;
#else
#error "Not currect option"
#endif

	update7SEG();

	setTimer2_10ms();

	EA = 1;
	while( 1 )
	{
		if( ! bStart )
		{
			delay_ms( KEY_FILTER_MS );
			if( ! bStart ) //the button is on !!
			{
				while( ! bStart );
				running = 1;
			}
		}

		if( ! bPauseContinue )
		{
			delay_ms( KEY_FILTER_MS );
			if( ! bPauseContinue ) //the button is on !!
			{
				while( ! bPauseContinue );
				running = ~running;
			}
		}

		if( ! bClear )
		{
			delay_ms( KEY_FILTER_MS );
			if( ! bClear ) //the button is on !!
			{
				EA = 0;
				while( ! bClear );

#if RUNNING_COUNTER
				currentTicksIn10ms = RUNNING_INITIAL_10MS;
#elif BASKETBALL_COUNTER
				currentTicksIn10ms = BASKETBALL_ROUND_TIMEUP_10MS;
#else
#error "Not currect Option"
#endif

				update7SEG();
				EA = 1;
			}
		}
	}
	
	return 0;
}

//timer 2 interrupt routine
void timer2Interrupt( void ) interrupt 5
{
	EA = 0;
	TF2 = 0;

	if( running )
	{

#if RUNNING_COUNTER
				currentTicksIn10ms++;
#elif BASKETBALL_COUNTER
				currentTicksIn10ms--;
#else 
#error "Not currect Option"
#endif

		update7SEG();
	}
	
	outPut = ~outPut;
	EA = 1;
}

//set the timer 
void setTimer2_10ms( void )
{
	T2CON = 0x00;
	TH2 = RCAP2H = 0xD8 ;   //run at 12MHz at 1us percycle 65536 - 10000 
	TL2 = RCAP2L = 0xF0 ;

	ET2 = 1;  //enable the timer 2 interrupt
	TR2 = 1;  //start the timer	
}

void update7SEG( void )
{
	unsigned long min;   //if i don't use the long it will fail me, trust me. why??? It seem it use the 2Byte int to minus
	unsigned long sec;
	unsigned long ms10;

	min = currentTicksIn10ms / ( 1 * 60 * 100 );
	sec =( currentTicksIn10ms - ( 1 * 60 * 100 * min ) ) / ( 100 );
	ms10 = currentTicksIn10ms - ( 1 * 60 * 100 * min ) - 100 * sec;

	MINUTE_LED = char2BCD( min );
	SECOND_LED = char2BCD( sec );
	MICRO_10MS_LED = char2BCD( ms10 ); 

}

#endif 
