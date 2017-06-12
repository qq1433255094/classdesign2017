#include <REGX52.H>

#include "lib51.h"
#include "intrins.h"



sbit DS18=P3^3;
sbit buzzer=P3^2;




int count=0;
extern unsigned short SEG_Num;
extern char SEG_Dot;

char key,key2;
void TIM1_CallBack(void)
{
	count++;
	key=(P2&0x01);
	if((key2-key)>0)
		buzzer=~buzzer;
	if(count==100)
	{count=0;
		SEG_Num++;
	}
	key2=(P2&0x01);
}


void Delay_ms(unsigned int n)		//@12.000MHz
{
	unsigned char i, j;
	while(n--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Delay10us(int n)		//@11.0592MHz
{
	unsigned char i;

	i = 2*n;
	while (--i);
}

void Wr_Bit(bit a)
{
	_nop_();
	_nop_();
	DS18=0;
	_nop_();
	_nop_();
	DS18=a;
	Delay10us(60);
	DS18=1;
}

void Wr_Byte(unsigned char byte)
{
	unsigned char i=0;
	
	while(i++<8)
	{
		Wr_Bit(byte);
		byte>>=1;
	}
}

bit Re_Bit()
{
	bit a;
	_nop_();
	_nop_();
	DS18=0;
	_nop_();_nop_();	
	DS18=1;
	_nop_();_nop_();	
	_nop_();_nop_();	
	a=DS18;
	Delay10us(60);
	DS18=1;
	return a;
}

unsigned char Re_Byte()
{
	unsigned char a=0,i=0,bits=0;
	while(i++<8)
	{
		bits=Re_Bit();
		a|=bits<<8;
		a>>=1;
	}
	return a;
}

unsigned short Re_Word()
{
	unsigned short a=0,i=0,bits=0;
	while(i++<16)
	{
		if(i==8)
		{_nop_();_nop_();}
		bits=Re_Bit();
		a|=bits<<16;
		a>>=1;
	}
	return a;
}





void main()
{
	int i=0,j=0;

	
	DS18=1;
	TIM0_init(1);
	TIM1_init(10);
	UART_init();

	while(1)
	{
		i=Get_SysTime()%1000;
		SEG_Display(i%4);
		
		if(i==0)
		{	
			j++;if(j==8) j=0;
			SendString("text \r\n");
			P1=~(0x01<<j);	
		}
	}
}



















