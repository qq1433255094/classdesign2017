#include <stdio.h>
#include <REGX52.H>
#include "lib51.h"
#include "LCD1602.h"

char motor_dir=1;
char motor_step=0;
char motor_enable=0;
char motor_table[6]={5,4,6,2,3,1};
unsigned short motor_speed=0;

void motor_run();
char readkey();
unsigned char modify_para(unsigned short *para);

unsigned int tick0=0;
unsigned int tick1=0;

void TIM0_CallBack(void)
{
	tick0++;

	if(tick0%10==0)
	{

	}

}

void TIM1_CallBack(void)
{
	tick1++;
	if(motor_speed>200)
		motor_speed==200;
	if(tick1>=(210-motor_speed))
	{
		tick1=0;
		if(motor_enable==1 && motor_speed>0)
			motor_run();
	}
}




void main()
{
	short speed_val=0;

	UART_init();
	TIM0_init(1);
	TIM1_init(5);
	LCD_init();

	P3 = 0x0f;

	DisplayListString(0,1,"speed:");
	
	while (1)
	{
		motor_dir = speed_val>0?0:1;
		motor_speed = speed_val>0?speed_val:-speed_val;
		
		DisplayListNum(6,1,speed_val);
		if(motor_enable==1)
		{
			DisplayListString(0,0,"        ");
			DisplayListString(0,0,"runing!");
		}
		else
		{
			DisplayListString(0,0,"        ");
			DisplayListString(0,0,"stop!");
		}

		if(readkey()==11)
		{
			DisplayClear();
			DisplayListString(0,1,"set:");
			DisplayListNum(6,1,0);

			modify_para(&speed_val);
			Delay(1000);
			DisplayClear();

			DisplayListString(0,1,"speed:");
			DisplayListNum(6,1,speed_val);
		}
		else if(readkey()==12)
		{
			motor_enable = 1;
		}
		else if(readkey()==13)
		{
			motor_enable = 0;
		}

		Delay(50);
	}
}


char readkey()
{
	char keyval=-1;
	char i,j;
	char table[4]={0x10,0x20,0x40,0x80};
	char val[16]={	7,8,9,12,  4,5,6,13,  1,2,3,14,  10,0,11,15};

	P3 = 0x0f;

	if(P3 != 0x0f)
	{
		for(i=0;i<4;i++)
		{
			P3= ~table[i];
			for(j=0;j<4;j++)
			{
				if((P3 & ( 0x01<<j )) == 0)
					return val[(i+(j<<2))];
			}
		}
	}
	return keyval;
}

void motor_run()
{
	if (motor_dir==1)
	{
		motor_step++;
		if(motor_step==6)
			motor_step=0;
	}
	else
	{
		motor_step--;
		if(motor_step==-1)
			motor_step=5;
	}

	P1 &= 0xf8;
	P1 |= motor_table[motor_step];
}

unsigned char modify_para(unsigned short *para)
{
	char val=-1,exit_enable=0,len=0,modified=0;
	char dir=1;
	unsigned short tick=0,temp=0;

	while(!exit_enable)
	{
		val=readkey();

		while(val !=-1 ){
			val=readkey();
			Delay(5);
		}
		tick =0;
		while(val ==-1 ){
			val=readkey();
			Delay(5);
			tick++;
			if(tick>1000)
			{
				DisplayClear();
				DisplayListString(0,0,"timeout!");
				return 0;
			}
		}

		if(val<10 && len < 3){
			temp *=10;
			temp +=val;
			if(temp>200)
				temp=200;
			len ++;
		}
		if(val==14){
			dir=-1;
		}
		if(val==15){
			dir=1;
		}

		DisplayListNum(6,1,temp*dir);
		

		if(val==10){
			exit_enable =1;
			modified=0;
		}
		if(val==11){
			*para = temp*dir;
			exit_enable =1;
			modified=1;
		}

		while(val !=-1 ){
			val=readkey();
			Delay(5);
		}
		Delay(5);
		
	}
	DisplayClear();
	if(modified==1){
		DisplayListString(0,0,"set complete!");
	}
	else{
		DisplayListString(0,0,"set cancel!");
	}
	return modified;
}
