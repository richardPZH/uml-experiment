#include <rtx51tny.h>
#include <reg52.h>
#include <INTRINS.H>

unsigned char ucMotorDrvPuls; //�����תʱ��ʼֵ
unsigned char FORREV=1; //1��ʾ���У�0��ʾ����
unsigned char STOPCUR=0;//1����ͣ���ڵ�ǰ�㣬0��ͣ��
unsigned char CURFLR=1; //��ǰ����¥��
unsigned char count=0;  //�ۼƵ�COUNT����ʾ���й�һ��

#define UCTIMES 8 //���õ��ת��
#define OUTPUT P2 //��������źſ�
#define COUNT 8   //���ÿѭ��8�α�ʾ���ݾ���һ��¥

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

//����λ
sbit alarmBit=P0^4;
sbit upLight=P0^5;
sbit downLight=P0^6;

void time(unsigned int ucMs);//��ʱ��λ��ms
void outPress();//���µ����ⰴť
unsigned char inPress();//���µ�����¥�㰴ť
unsigned char elevator();//����ĳһ�㷵��1�����򷵻�0
void storeUP(unsigned char);//�洢��ǰ������������
void storeDOWN(unsigned char);//�洢��ǰ������������

//��ʱ����
void time(unsigned int ucMs);

//��ʼ�����е�
void initLights(void);

//���õ�ǰ¥��
void setFloor(int floor);

//���õ����������е�
void setUpLight();

//���õ����������е�
void setDownLight();

//���õ���ֹͣ���е�
void setStopLight();

//���õ����������е�
void setAlarmLight();

//���õ��ݱ����ƺ�����
void setAlarmLight();

//�رյ��ݱ����ƺ�����
void offAlarmLight();


//��������
int alarmSwitch=1;

unsigned char UP_req[5]={0,0,0,0,0}; //��������
unsigned char DOWN_req[5]={0,0,0,0,0}; //��������

//�����λ
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
  position();//�����λ
  ucMotorDrvPuls=0x11;
  OUTPUT=0x00|(P2&0xf0);//���ֹͣ 
  setFloor(CURFLR);
  setUpLight();
  time(100);
  
  os_create_task( 1 );
  os_create_task( 2 );
  os_create_task( 3 );

  while(1)//��ѭ��
  {
	  do{
		  os_send_signal( 1 );
	
		  os_wait1( K_SIG ); //wait for fisrt scan to finish

		  os_send_signal( 2 );
	
		  os_wait1( K_SIG ); //wait for second scan to finish

	  }while( STOPCUR == 1 );//�����ڵ�ǰ�㣬���ݲ��������Լ�����������
	    

	  if( inPress() )//���µ�����¥�㰴ť
	  {
		do{
			  os_send_signal( 1 );
	
			  os_wait1( K_SIG ); //wait for fisrt scan to finish

			  os_send_signal( 2 );
	
			  os_wait1( K_SIG ); //wait for second scan to finish

		  }while( START );//�ȴ������������£����ݲ��������Լ�����������
	  }
	
	  os_send_signal( 3 );

	  os_wait1( K_SIG );

	  OUTPUT=0x00|(P2&0xf0);//���ֹͣ��������ʱ����������ť����
	  

  }//end while-��ѭ��   
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
			continue;//û���������������תѭ�������ݲ���
		}
		  
	    if(FORREV)//���� 
	    { 
		  setUpLight();//���е���

		  if(STOPCUR==1){
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//û���������������תѭ�������ݲ���
		  }//

		  if(elevator())//���ϵ���ĳһ��
	      {
			if(CURFLR==4) {
				setDownLight();
				os_send_signal( 0 );
				os_wait1( K_SIG );
				continue;//û���������������תѭ�������ݲ���
			}

	      }
	      OUTPUT=(ucMotorDrvPuls&0x0f)|(P2&0xf0);
		  ucMotorDrvPuls=_crol_(ucMotorDrvPuls,1);
	    }
	    if(!FORREV)//����
	    {
		  setDownLight();//���е���

		  if(STOPCUR==1){
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//û���������������תѭ�������ݲ���
		  }

		  if(elevator())//���µ���ĳһ��
	      {
			if(CURFLR==1) {
			setUpLight();//����һ¥
			os_send_signal( 0 );
			os_wait1( K_SIG );
			continue;//û���������������תѭ�������ݲ���
	      }
		  }
	      OUTPUT=(ucMotorDrvPuls&0x0f)|(P2&0xf0);
		  ucMotorDrvPuls=_cror_(ucMotorDrvPuls,1);
	    }	
	
	    outPress();//���µ����ⰴť
	    //if(inPress())//���µ�����¥�㰴ť
		//{
		 // while(START)//�ȴ�������������
		  //{
	      //  outPress();
	        inPress();//��������ʱ���ڲ���ť���£����谴������ť
	     // }
		//}

		os_wait( K_TMO , ( 380 - UCTIMES*16 ) / 10, 0 );

	}

}


/****************************************/
/*               ��ť�¼�               */
/****************************************/
//���µ����ⰴť 
void outPress()
{
   //FORREV=1;
   if(!UP1)//1¥��
   {
     storeUP(1);
	 if(CURFLR>1&&STOPCUR==1)//���ݲ���һ¥,�ҵ�ǰû�������󣬵��������Զ�����
	 {
	   FORREV=0;//��������
	   STOPCUR=0;
	 }
	 if(CURFLR==1)
	 {
	   STOPCUR=1;//����ͣ���ڵ�ǰ��
	 }
   }
   if(!UP2)//2¥��
   {
     storeUP(2);
	 if(CURFLR>2&&STOPCUR==1)//�����ڶ�¥����
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
   if(!UP3)//3¥��
   {
     storeUP(3);
	 if(CURFLR>3&&STOPCUR==1)//��������¥����
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
   if(!DOWN2)//2¥��
   {
     storeDOWN(2);
	 if(CURFLR>2&&STOPCUR==1)//�����ڶ�¥����
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
   if(!DOWN3)//3¥��
   {
     storeDOWN(3);
	 if(CURFLR>3&&STOPCUR==1)//��������¥����
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
   if(!DOWN4)//4¥��
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

//���µ�����¥�㰴ť
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
    if(2>CURFLR)//�������ڵ�ǰ��
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
    if(3>CURFLR)//�������ڵ�ǰ��
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
  if(!STOP)//����ֹͣ
  {
    while(START)//����Ӧ��������
	{
	  //����
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
      //�������ж���ȥ������ѡ������²�
	  for(i=CURFLR+1;i<=4;i++)
	  {
	    if(UP_req[i]==1||DOWN_req[i]==1){flag=1;}
	  }
	  if(flag==0)//�ϲ�û����
	  {
	    FORREV=0;
		
	  }
	}
	if(FORREV==0)
	{
	   //�������ж���ȥ������ѡ������ϲ�
	  for(i=CURFLR-1;i>=1;i--)
	  {
	    if(UP_req[i]==1||DOWN_req[i]==1){flag=1;}
	  }
	  if(flag==0)//�ϲ�û����
	  {
	    FORREV=1;
		
	  }
	}
  return 0;
}

/*******************************************************************/
/*                       ����ĳһ�㷵��1�����򷵻�0,			   */
/*    ���ơ���ʾ���֡��������㡢���ֹͣ��ȷ������ȥ�������       */
/*******************************************************************/
unsigned char elevator()
{
  count++;
  if(count==COUNT)
  {
	//�������
    if(FORREV==1)//�ж������Ƿ񵽴�����¥��,�����������ȴ���
	{
	  CURFLR++;
	  setUpLight();//���е��� 

	  

	  if(CURFLR==2)//�����¥
	  {
	    count=0;	
	    setFloor(2);//��ʾ����
	    if(UP_req[2]==1)//��¥�������������ȴ���
	    {		
		  setUpLight();
	      UP_req[2]=0;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
		  return 1;
	    }
		if(DOWN_req[2]==1&&UP_req[3]==0&&DOWN_req[3]==0
		&&DOWN_req[4]==0)//��¥������������������û�����󣬲�������
		{		   
		   setDownLight();
		   DOWN_req[2]=0;
		   STOPCUR=1;
		   OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		   FORREV=0;
		   return 1;
		} 
	  }
	  if(CURFLR==3)//������¥
	  {	
	    setFloor(3);//��ʾ����
		count=0;
	    if(UP_req[3]==1)//��¥����������,���ȴ���
	    {
		  setUpLight();
	      UP_req[3]=0;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
		  return 1;
	    }
		if(DOWN_req[3]==1&&DOWN_req[4]==0)//��¥������������¥�����󣬲�������
		{
		  setDownLight();
		  FORREV=0;
		  DOWN_req[3]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  return 1;
		} 
	  }
	  if(CURFLR==4)//������¥
	  {	
	    setFloor(4);//��ʾ����
		setDownLight();
		count=0;
	    if(DOWN_req[4]==1)//��¥������,��¥������ֻ�����µ����
	    {
	      DOWN_req[4]=0;
		  FORREV=0;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
	    }
	  }
	}
	else//�ж������Ƿ񵽴�����㣬�����������ȴ���
	{
	  CURFLR--;
	  setDownLight();//���е���

	 

	  if(CURFLR==1)//����һ¥
	  {	
	    setFloor(1);//��ʾ����
		count=0;
	    if(UP_req[1]==1)//һ¥������,һ¥������ֻ�����ϵ����
	    {
		  setUpLight();
	      UP_req[1]=0;
		  FORREV=1;
	      OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
	    }
	  }
	  if(CURFLR==2)//�����¥
	  {	
	    setFloor(2);//��ʾ����
		count=0;
	    if(DOWN_req[2]==1)//��¥�������������ȴ���
	    {
		  setDownLight();
	      DOWN_req[2]=0;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
		  return 1;
	    }
		if(UP_req[2]==1&&UP_req[1]==0)//һ¥�����󣬲�������
		{
		  setUpLight();
		  FORREV=1;
		  UP_req[2]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		} 
	  }
	  if(CURFLR==3)//������¥
	  {	
	    setFloor(3);//��ʾ����
		count=0;
	    if(DOWN_req[3]==1)//��¥�������������ȴ���
	    {
		  setDownLight(); 
	      DOWN_req[3]=0;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
		  STOPCUR=1;
		  return 1;
	    }
		if(UP_req[1]==0&&DOWN_req[2]==0&&UP_req[2]==0&&UP_req[3]==1)
		{
		  setUpLight();
		  FORREV=1;
		  UP_req[3]=0;
		  STOPCUR=1;
		  OUTPUT=0x00|(P2&0xf0);//���ֹͣ
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
/*               ��������               */
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
/*               ���ܺ���               */
/****************************************/
//��ʼ�����е�
void initLights()
{
	P0=0x11;
}

//����¥����ʾ
void setFloor(int floor)
{
	switch (floor)
	{
		case 1:
		{	
			P0&=0xf0;//����
			P0|=0x01;break;
		}
		case 2:
		{
			P0&=0xf0;//����
			P0|=0x02;break;
		}
		case 3:
		{
			P0&=0xf0;//����
			P0|=0x03;break;
		}
		case 4:
		{
			P0&=0xf0;//����
			P0|=0x04;break;
		}
		default:
		{
			P0=0x06;break;
		}
	}
}

//���õ����������е�
void setUpLight()
{
	upLight=1;
	downLight=0;
}

//���õ����������е�
void setDownLight()
{
	upLight=0;
	downLight=1;
}

//���õ���ֹͣ���е�
void setStopLight()
{
	upLight=0;
	downLight=0;
}

//���õ��ݱ����ƺ�����
void setAlarmLight()
{
	int num=0;//ѭ������
	//���þ������
	while(START)
	{
		num++;
		if(!alarmBit)
		{
			alarmBit=1;
		}
		else//Ϩ�𾯸��
		{
			alarmBit=0;
		}
		time(400);
	}
	offAlarmLight();	
}

//ֹͣ�����ƺ�����
void offAlarmLight()
{
	alarmBit=1;
}

/******************************************/
/*              ��ʱ����                  */
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


