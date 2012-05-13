#include<REG52.H>
#include<IMSutil.h>

#define RUNNING_BASKETBALL_COUNTER 0
#define COUNTER_1MS 1


#if ( ( RUNNING_BASKETBALL_COUNTER + COUNTER_1MS ) != 1 )
#error "Error Configure, Check line 4/5"
#endif

#if RUNNING_BASKETBALL_COUNTER

#define MINUTE_LED P3
#define SECOND_LED P2
#define MICRO_10MS_LED P1

#define KEY_FILTER_MS 2
#define RUNNING_INITIAL_10MS 0	
#define BASKETBALL_ROUND_TIMEUP_10MS 72000

sbit bStart = P0^0;
sbit bPauseContinue = P0^1;
sbit outPut = P0^7;
bit running;

sbit bClear = P0^2;
sbit bExit = P0^3;

//unsigned long is 32 bit...good
volatile unsigned long currentTicksIn10ms;
volatile bit basketball;

//set the timer 2 interrupt at every 10ms
void setTimer2_10ms( void );

//update the 7-SEG led using the global ticks
void update7SEG( void );

int main( void )
{
	running = 0;
	outPut = 1;
	basketball = 0;

	if( !basketball )
		currentTicksIn10ms = RUNNING_INITIAL_10MS;
	else
		currentTicksIn10ms = BASKETBALL_ROUND_TIMEUP_10MS;

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

				if( !basketball )
					currentTicksIn10ms = RUNNING_INITIAL_10MS;
				else
					currentTicksIn10ms = BASKETBALL_ROUND_TIMEUP_10MS;
				
				update7SEG();
				EA = 1;
			}
		}

		if( ! bExit )
		{
			delay_ms( KEY_FILTER_MS );
			if( ! bExit )
			{
				EA = 0;
				while( ! bExit );

				if( !basketball )
				{
					basketball = 1;
					currentTicksIn10ms = BASKETBALL_ROUND_TIMEUP_10MS;
				}
				else
				{
					basketball = 0;
					currentTicksIn10ms = RUNNING_INITIAL_10MS;
				}
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
		if( !basketball )
			currentTicksIn10ms++;
		else
			currentTicksIn10ms--;

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

#if COUNTER_1MS 
//running at counter mode can count up and count down

#define KEY_FILTER_MS 0

#define SECOND_LED P3
#define MICRO_100MS_LED P2
#define MICRO_1MS_LED P1

#define UP_INITIAL_1MS 0
#define DOWN_INITIAL_1MS ( 0x1869F )

sbit bStart = P0^0;				//start button
sbit bPauseContinue = P0^1;     //Pause Continue button
sbit outPut = P0^7;             //outPut pluse output port
bit running;					//indicate it's on running status or stopped status

sbit bClear = P0^2;             //clear button 
sbit bExit = P0^3;				//mode switch button

//unsigned long is 32 bit...good
//volatile unsigned long currentTicksIn1ms;   //will the volatile change my time?? It seems it won't
volatile bit up;

volatile unsigned char secCnt;
volatile unsigned char ms100Cnt;
volatile unsigned char ms1Cnt;

void setTimer2_1ms( void );
void update7SEG( void );

int main( void )
{

	running = 0;
	up = 0;

	if( !up )
	{
		//currentTicksIn1ms = DOWN_INITIAL_1MS;
		secCnt = 99;
		ms100Cnt = 9;
		ms1Cnt = 99;
	}
	else
	{
		//currentTicksIn1ms = UP_INITIAL_1MS;
		secCnt = 0;
		ms100Cnt = 0;
		ms1Cnt = 0;
	}

	update7SEG();
	
	setTimer2_1ms();

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

				if( !up )
				{
					secCnt = 99;
					ms100Cnt = 9;
					ms1Cnt = 99;
					//currentTicksIn1ms = DOWN_INITIAL_1MS;
				}
				else
				{
					//currentTicksIn1ms = UP_INITIAL_1MS;
					secCnt = 0;
					ms100Cnt = 0;
					ms1Cnt = 0;
				}
				
				update7SEG();
				EA = 1;
			}
		}

		if( ! bExit )
		{
			delay_ms( KEY_FILTER_MS );
			if( ! bExit )
			{
				EA = 0;
				while( ! bExit );

				if( !up )
				{
					up = 1;
					secCnt = 0;
					ms100Cnt = 0;
					ms1Cnt = 0;
				}
				else
				{
					up = 0;
					secCnt = 99;
					ms100Cnt = 9;
					ms1Cnt = 99;
				}
				update7SEG();
				EA = 1;
			}
		}


	}


	return 0;
}

//timer2 overflow interrupt handler(routine)
void timer2Interrupt( void ) interrupt 5
{
	EA = 0;
	TF2 = 0;
	
	if( running )
	{
		if( up )
		{
			//currentTicksIn1ms++;
			ms1Cnt ++;
			if( ms1Cnt >= 100 )
			{
				ms1Cnt = 0;
				ms100Cnt ++;
			}

			if( ms100Cnt >= 10 )
			{
				ms100Cnt = 0;
				secCnt ++;
			}
		}
		else
		{
			//currentTicksIn1ms--;
			if( ms1Cnt == 0 )
			{
				ms1Cnt = 99;
				if( ms100Cnt == 0 )
				{
					ms100Cnt = 9;
					secCnt --;
				}
				else
				{
					ms100Cnt--;
				}
			}
			else
			{
				ms1Cnt --;
			}
		}

		update7SEG();
	}

	outPut = ~outPut;
	EA = 1;
}

//set the timer2 interrupt every 1ms  
void setTimer2_1ms( void )
{
	T2CON = 0x00;
	TH2 = RCAP2H = 0xFC ;   //run at 12MHz at 1us percycle 65536 - 1000 
	TL2 = RCAP2L = 0x18 ;

	ET2 = 1;  //enable the timer 2 interrupt
	TR2 = 1;  //start the timer	
}

void update7SEG( void )    //C51 c do not support inline???
{
	/*
	 * This code just simply run too slow
	 * It suddenly occur to me that The c hide a lots of thing that it will run very slow...
	 * using the unsigned long in C51 is expensive and doing dividing multpling is expensive
	 * using function call and some define will run very slow too!!!

	unsigned long sec;   //if i don't use the long it will fail me, trust me. why??? It seem it use the 2Byte int to minus
	unsigned long ms1;

	sec = currentTicksIn1ms / ( 1000 );
	ms1 = currentTicksIn1ms - sec*1000 ;

	SECOND_LED = char2BCD( sec );              //this piece of code run up to 5ms????? it's unusal
	MICRO_1MS_LED = char2BCD( ms1 % 100 );
	MICRO_100MS_LED = char2BCD( ms1 / 100 );		
	*/

	//This code will not exceed 1ms ! so it can count correctly
	SECOND_LED = char2BCD( secCnt );
	MICRO_100MS_LED = char2BCD( ms100Cnt );
	MICRO_1MS_LED = char2BCD( ms1Cnt );
	
}

#endif
