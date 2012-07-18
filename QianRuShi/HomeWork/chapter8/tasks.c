#include <rtx51tny.h>
#include <reg52.h>
#include <INTRINS.H>

unsigned char ucMotorDrvPuls; //电机运转时初始值
unsigned char FORREV=1; //1表示上行，0表示下行
unsigned char STOPCUR=0;//1电梯停留在当前层，0不停留
unsigned char CURFLR=1; //当前所在楼层
unsigned char count=0;  //累计到COUNT即表示运行过一层

#define UCTIMES 8 //设置电机转速
#define OUTPUT P2 //电机驱动信号口
#define COUNT 8   //电机每循环8次表示电梯经过一层楼

sbit UP1=P3^4;
sbit DOWN2=P3^3;
sbit UP2=P3^2;
sbit DOWN3=P3^1;
sbit UP3=P3^0;
sbit DOWN4=P1^7;
sbit FLOOR1=P1^0;
sbit FLOOR2=P1^1;
sbit FLOOR3=P1^2;
sbit FLOOR4=P1^3;
sbit START=P1^4;
sbit STOP=P1^5;
sbit ALARM=P1^6;

//报警位
sbit alarmBit=P0^4;
sbit upLight=P0^5;
sbit downLight=P0^6;

void time(unsigned int ucMs);//延时单位：ms
void outPress();//按下电梯外按钮
unsigned char inPress();//按下电梯内楼层按钮
unsigned char elevator();//到达某一层返回1，否则返回0
void storeUP(unsigned char);//存储当前所有上行请求
void storeDOWN(unsigned char);//存储当前所有下行请求

//延时函数
void time(unsigned int ucMs);

//初始化所有灯
void initLights(void);

//设置当前楼层
void setFloor(int floor);

//设置电梯向上运行灯
void setUpLight();

//设置电梯向下运行灯
void setDownLight();

//设置电梯停止运行灯
void setStopLight();

//设置电梯向上运行灯
void setAlarmLight();

//设置电梯报警灯和喇叭
void setAlarmLight();

//关闭电梯报警灯和喇叭
void offAlarmLight();


//报警开关
int alarmSwitch=1;

unsigned char UP_req[5]={0,0,0,0,0}; //上行请求
unsigned char DOWN_req[5]={0,0,0,0,0}; //下行请求

//电机定位
void position(void)
{
  OUTPUT=0x01|(P2&0xf0);time(200);
  OUTPUT=0x02|(P2&0xf0);time(200);
  OUTPUT=0x04|(P2&0xf0);time(200);
  OUTPUT=0x08|(P2&0xf0);time(200);
  ucMotorDrvPuls=0x11;
  OUTPUT=0x01|(P2&0xf0);	
  
}

/****************************************/
/*                 init task            */
/****************************************/
void elavator(void) _task_ 0
{ 
  time(100);  //Get others ready
  initLights();
  position();//电机定位
  ucMotorDrvPuls=0x11;
  OUTPUT=0x00|(P2&0xf0);//电机停止 
  setFloor(CURFLR);
  setUpLight();
  time(100);
  
  os_create_task( 1 );
  os_create_task( 2 );
  os_create_task( 3 );

  while(1)//主循环
  {
	  do{
		  os_send_signal( 1 );
	
		  os_wait1( K_SIG ); //wait for fisrt scan to finish

		  os_send_signal( 2 );
	
		  os_wait1( K_SIG ); //wait for second scan to finish

	  }while( STOPCUR == 1 );//电梯在当前层，电梯不动，可以继续接受请求
	    

	  if( inPress() )//按下电梯内楼层按钮
	  {
		do{
			  os_send_signal( 1 );
	
			  os_wait1( K_SIG ); //wait for fisrt scan to finish

			  os_send_signal( 2 );
	
			  os_wait1( K_SIG ); //wait for second scan to finish

		  }while( START );//等待启动按键按下，电梯不动，可以继续接受请求
	  }
	
	  os_send_signal( 3 );

	  os_wait1( K_SIG );

	  OUTPUT=0x00|(P2&0xf0);//电机停止，有请求时按下启动按钮启动
	  

  }//end while-主循环   
}

//outpress task
void task_outPress(void) _task_ 1
{
	while( 1 )
	{
		os_wait1( K_SIG );

		outPress();

		os_send_signal( 0 );

	}
}

//inPress task
void task_inPress( void ) _task_ 2
{
	while( 1 )
	{
		os_wait1( K_SIG );

		inPress();

		os_send_signal( 0 );

	}
}

//motor task
void task_motor( void ) _task_ 3
{
	os_wait1( K_SIG );
	while( 1 )
	{
	    if(UP_req[1]==0&&UP_req[2]==0&&UP_req[3]==0 && DOWN_req[2]==0&&DOWN_req[3]==0&&DOWN_req[4]==0) 
		{
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//没有请求，跳出电机运转循环，电梯不动
		}
		  
	    if(FORREV)//上行 
	    { 
		  setUpLight();//上行灯亮

		  if(STOPCUR==1){
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//没有请求，跳出电机运转循环，电梯不动
		  }//

		  if(elevator())//往上到达某一层
	      {
			if(CURFLR==4) {
				setDownLight();
				os_send_signal( 0 );
				os_wait1( K_SIG );
				continue;//没有请求，跳出电机运转循环，电梯不动
			}

	      }
	      OUTPUT=(ucMotorDrvPuls&0x0f)|(P2&0xf0);
		  ucMotorDrvPuls=_crol_(ucMotorDrvPuls,1);
	    }
	    if(!FORREV)//下行
	    {
		  setDownLight();//下行灯亮

		  if(STOPCUR==1){
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//没有请求，跳出电机运转循环，电梯不动
		  }

		  if(elevator())//往下到达某一层
	      {
			if(CURFLR==1) {
			setUpLight();//到达一楼
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//没有请求，跳出电机运转循环，电梯不动
	      }
		  }
	      OUTPUT=(ucMotorDrvPuls&0x0f)|(P2&0xf0);
		  ucMotorDrvPuls=_cror_(ucMotorDrvPuls,1);
	    }	
	
	    outPress();//按下电梯外按钮
	    //if(inPress())//按下电梯内楼层按钮
		//{
		 // while(START)//等待启动按键按下
		  //{
	      //  outPress();
	        inPress();//电梯运行时，内部按钮按下，无需按启动按钮
	     // }
		//}

		os_wait( K_TMO , ( 380 - UCTIMES*16 ) / 10, 0 );

	}

}


/****************************************/
/*               按钮事件               */
/****************************************/
//按下电梯外按钮 
void outPress()
{
   //FORREV=1;
   if(!UP1)//1楼上
   {
     storeUP(1);
	 if(CURFLR>1&&STOPCUR==1)//电梯不在一楼,且当前没其他请求，电梯马上自动启动
	 {
	   FORREV=0;//向下运行
	   STOPCUR=0;
	 }
	 if(CURFLR==1)
	 {
	   STOPCUR=1;//电梯停留在当前层
	 }
   }
   if(!UP2)//2楼上
   {
     storeUP(2);
	 if(CURFLR>2&&STOPCUR==1)//电梯在二楼以上
	 {
	   FORREV=0;
	   STOPCUR=0;
	 }
	 if(CURFLR<2&&STOPCUR==1)
	 {
	   FORREV=1;
	   STOPCUR=0;
	 }
	 if(CURFLR==2)
	 {
	   STOPCUR=1;
	 }
   }
   if(!UP3)//3楼上
   {
     storeUP(3);
	 if(CURFLR>3&&STOPCUR==1)//电梯在三楼以上
	 {
	   FORREV=0;
	   STOPCUR=0;
	 }
	 if(CURFLR<3&&STOPCUR==1)
	 {
	   FORREV=1;
	   STOPCUR=0;
	 }
	 if(CURFLR==3)
	 {
	   STOPCUR=1;
	 }
   }
   if(!DOWN2)//2楼下
   {
     storeDOWN(2);
	 if(CURFLR>2&&STOPCUR==1)//电梯在二楼以上
	 {
	   FORREV=0;
	   STOPCUR=0;
	 }
	 if(CURFLR<2&&STOPCUR==1)
	 {
	   FORREV=1;
	   STOPCUR=0;
	 }
	 if(CURFLR==2)
	 {
	   STOPCUR=1;
	 }
   }
   if(!DOWN3)//3楼下
   {
     storeDOWN(3);
	 if(CURFLR>3&&STOPCUR==1)//电梯在三楼以上
	 {
	   FORREV=0;
	   STOPCUR=0;
	 }
	 if(CURFLR<3&&STOPCUR==1)
	 {
	   FORREV=1;
	   STOPCUR=0;
	 }
	 if(CURFLR==3)
	 {
	   STOPCUR=1;
	 }
   }
   if(!DOWN4)//4楼下
   {
     storeDOWN(4);
	 if(CURFLR<4&&STOPCUR==1)
	 {
	   FORREV=1;
	   STOPCUR=0;
	 }
	 if(CURFLR==4)
	 {
	   STOPCUR=1;
	 }
   }
}

//按下电梯内楼层按钮
unsigned char inPress()
{
  
  int i;
  int flag=0;

  if(!FLOOR1)
  {	
    if(1<CURFLR)
	{
	  STOPCUR=0;
	  UP_req[1]=1;
	}
	if(1==CURFLR)
	{
	  STOPCUR=1;
	}
	return 1;
  }
  if(!FLOOR2)
  {
    if(2>CURFLR)//请求层大于当前层
	{
	  UP_req[2]=1;
	  STOPCUR=0;
	}
	if(2<CURFLR)
	{
	  DOWN_req[2]=1;
	  STOPCUR=0;
	}
	if(2==CURFLR)
	{
	  STOPCUR=1;
	}
	return 1;
	
  }
  if(!FLOOR3)
  {
    if(3>CURFLR)//请求层大于当前层
	{
	  UP_req[3]=1;
	  STOPCUR=0;
	}
	if(3<CURFLR)
	{
	  DOWN_req[3]=1;
	  STOPCUR=0;
	}
	if(3==CURFLR)
	{
	  STOPCUR=1;
	}
	return 1;
  }

  if(!FLOOR4)
  {
    if(4>CURFLR)
	{
      DOWN_req[4]=1;
	  STOPCUR=0;
	}
	if(4==CURFLR)
	{
	  STOPCUR=1;
	}
	return 1;
  }
  if(!START)
  {
    STOPCUR=0;
	return 1;
  }
  if(!STOP)//紧急停止
  {
    while(START)//不响应其他按键
	{
	  //亮灯
	  setStopLight();
	}
	return 1;
  }
  if(!ALARM)
  {
	  setAlarmLight();
	return 1;
  }

   if(FORREV==1)
   {
      //请求上行而进去电梯内选择的是下层
	  for(i=CURFLR+1;i<=4;i++)
	  {
	    if(UP_req[i]==1||DOWN_req[i]==1){flag=1;}
	  }
	  if(flag==0)//上层没请求
	  {
	    FORREV=0;
		
	  }
	}
	if(FORREV==0)
	{
	   //请求下行而进去电梯内选择的是上层
	  for(i=CURFLR-1;i>=1;i--)
	  {
	    if(UP_req[i]==1||DOWN_req[i]==1){flag=1;}
	  }
	  if(flag==0)//上层没请求
	  {
	    FORREV=1;
		
	  }
	}
  return 0;
}

/*******************************************************************/
/*                       到达某一层返回1，否则返回0,			   */
/*    亮灯、显示数字、请求清零、电机停止、确定接下去电机方向       */
/*******************************************************************/
unsigned char elevator()
{
  count++;
  if(count==COUNT)
  {
	//正常情况
    if(FORREV==1)//判断上行是否到达请求楼层,上行请求优先处理
	{
	  CURFLR++;
	  setUpLight();//上行灯亮 

	  

	  if(CURFLR==2)//到达二楼
	  {
	    count=0;	
	    setFloor(2);//显示数字
	    if(UP_req[2]==1)//二楼有上行请求，优先处理
	    {		
		  setUpLight();
	      UP_req[2]=0;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
		  return 1;
	    }
		if(DOWN_req[2]==1&&UP_req[3]==0&&DOWN_req[3]==0
		&&DOWN_req[4]==0)//二楼有下行请求，上面两层没有请求，不再往上
		{		   
		   setDownLight();
		   DOWN_req[2]=0;
		   STOPCUR=1;
		   OUTPUT=0x00|(P2&0xf0);//电机停止
		   FORREV=0;
		   return 1;
		} 
	  }
	  if(CURFLR==3)//到达三楼
	  {	
	    setFloor(3);//显示数字
		count=0;
	    if(UP_req[3]==1)//三楼有上行请求,优先处理
	    {
		  setUpLight();
	      UP_req[3]=0;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
		  return 1;
	    }
		if(DOWN_req[3]==1&&DOWN_req[4]==0)//三楼有下行请求，四楼无请求，不再往上
		{
		  setDownLight();
		  FORREV=0;
		  DOWN_req[3]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  return 1;
		} 
	  }
	  if(CURFLR==4)//到达四楼
	  {	
	    setFloor(4);//显示数字
		setDownLight();
		count=0;
	    if(DOWN_req[4]==1)//四楼有请求,四楼的请求只用向下的情况
	    {
	      DOWN_req[4]=0;
		  FORREV=0;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
	    }
	  }
	}
	else//判断下行是否到达请求层，下行请求优先处理
	{
	  CURFLR--;
	  setDownLight();//下行等亮

	 

	  if(CURFLR==1)//到达一楼
	  {	
	    setFloor(1);//显示数字
		count=0;
	    if(UP_req[1]==1)//一楼有请求,一楼的请求只有向上的情况
	    {
		  setUpLight();
	      UP_req[1]=0;
		  FORREV=1;
	      OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
	    }
	  }
	  if(CURFLR==2)//到达二楼
	  {	
	    setFloor(2);//显示数字
		count=0;
	    if(DOWN_req[2]==1)//二楼有下行请求，优先处理
	    {
		  setDownLight();
	      DOWN_req[2]=0;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
		  return 1;
	    }
		if(UP_req[2]==1&&UP_req[1]==0)//一楼无请求，不再往下
		{
		  setUpLight();
		  FORREV=1;
		  UP_req[2]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		} 
	  }
	  if(CURFLR==3)//到达三楼
	  {	
	    setFloor(3);//显示数字
		count=0;
	    if(DOWN_req[3]==1)//三楼有下行请求，优先处理
	    {
		  setDownLight(); 
	      DOWN_req[3]=0;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		  STOPCUR=1;
		  return 1;
	    }
		if(UP_req[1]==0&&DOWN_req[2]==0&&UP_req[2]==0&&UP_req[3]==1)
		{
		  setUpLight();
		  FORREV=1;
		  UP_req[3]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//电机停止
		} 
	  }
	}//end if-FORREV

    return 1;
  }
  else
  {
    return 0;
  }//end if-count
}

/****************************************/
/*               保存请求               */
/****************************************/
void storeUP(unsigned char x)
{
  UP_req[x]=1;
}
void storeDOWN(unsigned char x)
{
  DOWN_req[x]=1;
}

/****************************************/
/*               功能函数               */
/****************************************/
//初始化所有灯
void initLights()
{
	P0=0x11;
}

//设置楼层显示
void setFloor(int floor)
{
	switch (floor)
	{
		case 1:
		{	
			P0&=0xf0;//清零
			P0|=0x01;break;
		}
		case 2:
		{
			P0&=0xf0;//清零
			P0|=0x02;break;
		}
		case 3:
		{
			P0&=0xf0;//清零
			P0|=0x03;break;
		}
		case 4:
		{
			P0&=0xf0;//清零
			P0|=0x04;break;
		}
		default:
		{
			P0=0x06;break;
		}
	}
}

//设置电梯向上运行灯
void setUpLight()
{
	upLight=1;
	downLight=0;
}

//设置电梯向下运行灯
void setDownLight()
{
	upLight=0;
	downLight=1;
}

//设置电梯停止运行灯
void setStopLight()
{
	upLight=0;
	downLight=0;
}

//设置电梯报警灯和喇叭
void setAlarmLight()
{
	int num=0;//循环次数
	//设置警告灯亮
	while(START)
	{
		num++;
		if(!alarmBit)
		{
			alarmBit=1;
		}
		else//熄灭警告灯
		{
			alarmBit=0;
		}
		time(400);
	}
	offAlarmLight();	
}

//停止报警灯和喇叭
void offAlarmLight()
{
	alarmBit=1;
}

/******************************************/
/*              延时函数                  */
/******************************************/
void delay_5us(void)
{
  _nop_();
  _nop_();
}
void delay_50us(void)
{
  unsigned char i;
  for(i=0;i<4;i++)
  {
    delay_5us();
  }
}
void delay_100us(void)
{
  delay_50us();
  delay_50us();
}
void time(unsigned ucMs)
{
  unsigned char j;
  while(ucMs>0)
  {
    for(j=0;j<10;j++)
	delay_100us();
	ucMs--;
  }
}


