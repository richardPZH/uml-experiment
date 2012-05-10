#include<REG52.H>
#include<IMSutil.h>

#define RUNNING_COUNTER 0
#define BASKETBALL_COUNTER 0

#define RUNNING_INITIAL_10MS 0	
#define BASKETBALL_ROUND_TIMEUP_10MS 12*60*100 //12 minutes

#define MINUTE_LED P3
#define SECOND_LED P2
#define MICRO_10MS_LED P1

sbit bStart = P0^0;
sbit bPauseContinue = P0^1;
bit running;

sbit bClear = P0^2;
sbit bExit = P0^3;

#if RUNNING_COUNTER 
//unsigned long is 32 bit...good
volatile unsigned long currentTicksIn10ms;

int main( void )
{


















	return 0;
}

#endif 
