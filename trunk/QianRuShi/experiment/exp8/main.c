#include<REG52.H>
#include"IMSutil.h"

#define KEY_PORT P1
#define DataPort P0
#define LedConPort P2

unsigned char comNegative[]={ 0x3f , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d , 0x7d , 0x07 , 0x7f , 0x6f , 0x77 , 0x7c , 0x39 , 0x5e , 0x79 , 0x71 , 0x73 , 0x3e , 0xff , 0x00 };

unsigned char code ch[8] = {0x76 , 0x79 , 0x38 , 0x38 , 0x3f , 0x40 , 0x6f , 0x4f };

enum BUTTON_TYPE { B0 , B1 , B2 , B3 , B4 , B5 , B6 , B7 , B8 , B9 , CANCEL , CONFIRM , SET , RES0 , RES1 , RES2 , NON };

//This function acts like a factory, it map the raw input value into the enum type and return it
enum BUTTON_TYPE convertButton( unsigned char raw );

//This function read the keyboad and return the key typed, NON when no key is pressed
enum BUTTON_TYPE getKeyPressed( void );

int main( void )
{
	enum BUTTON_TYPE button;
	unsigned char cnt;

	while( 1 )
	{
		button = getKeyPressed();
		if( button != NON )
		{
			LedConPort= 0x00;
			DataPort = comNegative[button];
		}

		for( cnt=0 ; cnt<8; cnt++ )
		{
			LedConPort = cnt;
			DataPort = ch[cnt];
			delay_ms( 5 );
		}
	}

	return 0;
}


enum BUTTON_TYPE convertButton( unsigned char raw )
{
	//use the simple if else stuff( switch case are essentially the same)
	//can i figure out a classifier to compute and get raw to correct type more efficient?
	switch( raw )
	{
		case 0x11: return B7;break;
		case 0x21: return B8;break;
		case 0x41: return B9;break;
		case 0x81: return RES0;break;
		case 0x12: return B4;break;
		case 0x22: return B5;break;
		case 0x42: return B6;break;
		case 0x82: return CANCEL;break;
		case 0x14: return B1;break;
		case 0x24: return B2;break;
		case 0x44: return B3;break;
		case 0x84: return SET;break;
		case 0x18: return B0;break;
		case 0x28: return RES1;break;
		case 0x48: return RES2;break;
		case 0x88: return CONFIRM;break;
		default: return NON;
	}

	return RES0;
}

//This is a bad implementation 
//They have a bette approach 
//First read the key into firstKey    ( if first read faild, give up immediately)
//Second reread the key into secondKey 
//if they are the same so it is pressed 
//if not return NON
enum BUTTON_TYPE getKeyPressed( void )
{
#ifndef KEY_PORT
#error "No KEY_PORT defined function getKeyPressed() can't work!!"
#endif

	unsigned char x_temp,y_temp;

	KEY_PORT = 0x0F;

	x_temp = KEY_PORT;

	KEY_PORT = 0xF0;

	y_temp = KEY_PORT;

	x_temp = x_temp | y_temp;
	
	x_temp = ~x_temp;

	//wait for user to release the key
	do
	{
		KEY_PORT = 0xF0;         //can/should I use delay here?? If I have an operating system , it will be very easy, set a wait and let other run. when timeout check the value if fail..continue wait...
	}
	while( KEY_PORT != 0xF0 );
	
	
	return ( convertButton(x_temp) );
}




