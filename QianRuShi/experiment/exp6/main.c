#include<REG52.H>
#include<IMSutil.h>
#include<stdio.h>

//Command Mode
//CMD LEN DATA

#define READ_PORT_STATUS 0x0A
#define REC_OK_EXC_OK 0x00
#define REC_OK_EXC_FI 0x01
#define SET_PORT_STATUS 0x06
#define NUM_PORT P1

#define INPUT_PORT P0
#define OUT_PORT P2

typedef struct 
{
	unsigned char cmd;
	unsigned char len;
	unsigned char cData[2];
}Potocol;


//initial uart run at 19200 baud @11.0592Mhz
void ini_Uart( void );

//send a byte
void sendByte( unsigned char b );

int main( void )
{
	ini_Uart();

	EA = 0;
	ES = 0;
	//say hello to host and my num
	//printf("Hello, I am %u, Ready!\n", num );

	ES = 1;
	TI = 0;
	RI = 0;
	EA = 1;

	while( 1 );

	return 0;
}

void usarInterrupt( void ) interrupt 4
{
	Potocol obj;
	unsigned char cnt;

	EA = 0;

	//only receive interrupt will be here
	obj.cmd = SBUF;
	RI = 0;
	TI = 0;

	while( ! RI );

	obj.len = SBUF;
	RI = 0;
	TI = 0;

	for( cnt=0 ; cnt<obj.len ; cnt++ )
	{	
		while( !RI );
		obj.cData[cnt]=SBUF;
		RI = 0;
		TI = 0;
	}

	//total command receive complete
	if( obj.cmd == READ_PORT_STATUS )
	{
		if( obj.cData[0] == NUM_PORT ) //my num
		{
			sendByte( REC_OK_EXC_OK );
			sendByte( 2 );
			sendByte( INPUT_PORT );
			sendByte( OUT_PORT );
		}
	}
	else if( obj.cmd == SET_PORT_STATUS )
	{
		if( obj.cData[0] == NUM_PORT )
		{
			OUT_PORT = obj.cData[1];
			sendByte( REC_OK_EXC_OK );
			sendByte( 0 );
		}
	}
	else
	{
		//unknown command
		if( obj.cData[0] == NUM_PORT )
		{
			sendByte( REC_OK_EXC_FI );
			sendByte( 0 );
		}
	}
	RI = 0;
	TI = 0;
	
	EA = 1;
}

//send a byte
void sendByte( unsigned char b )
{
	TI = 0;
	SBUF = b;
	while( !TI );
	TI = 0;
}

//initial uart run at 19200 baud @11.0592Mhz //use the time1 to generage baud
void ini_Uart( void )
{
	TMOD = 0x20;
	PCON = 0x80;	
	TH1 = TL1 = 0xFD;
	TR1 = 1;

	SCON = 0x50;
	TI = 1;
}



#if 0

#include<REG52.H>
#include<IMSutil.h>
#include<string.h>

int main( void )
{
	char *s;
	s = "Hello IMS!";
	
	//use the timer 1 to go
	//run at 19200 baud
	TMOD = 0x20;
	PCON = 0x80;	
	TH1 = TL1 = 0xFD;
	TR1 = 1;

	SM0 =0;
	SM1 =1;

	while( 1 )
	{
		unsigned char c;
		unsigned char l;
		l = strlen(s); 
		for( c=0 ; c<l ; c++ )
		{
			SBUF = s[c];
			while( !TI );
			TI = 0;
		}

		delay_uc( 520 );

	}

	return 0;
}

#endif



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
