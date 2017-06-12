#include "lib51.h"
#include <REGX52.H>
#include "math.h"
#include <stdio.h>
/*
**name:Delayms()|Delayms(10);延时10ms
**para:n 延时的时间，单位ms
**return:non
**note:软件延时，精度较低
*/
void Delayms(unsigned int n)		//@12.000MHz
{
	unsigned char i, j;

	
	i = 2;
	j = 239;
	while(n--){
		do
		{
			while (--j);
		} while (--i);
	}
}



#if USE_SEG
//使用数码管
char SEG_Cnt=0;
unsigned short SEG_Num=1234;
char SEG_Dot=-1;
/*
**name:SEG_Display()|SEG_Num=1234; SEG_Display(0);显示SEG_Num这个变量值的最后一位，显示哪一位由参数n确定
**para:SEG_Num 要显示的数，为全局变量 |n 显示数的位置，0为个位，1为十位，2为百位，3为千位
**return:数码管显示
**note:为了方便动态显示特意写的函数，每次中断只要显示一位数，减少中断中的延时
*/
void SEG_Display(unsigned char n)
{
	unsigned short temp=SEG_Num; //取得要显示的数
	unsigned char send=n; //取得要显示的位置
	
	while(send--) temp/=10; //将数值右移，把要显示的位调整到个位
	temp=(temp%10); //求余，留下个位，即取出要显示的数值
	
	P3|=0xf0; //关闭所有的显示，高4位为显示控制，低电平选中相应的位置
	
	P0=SEG_DATA[temp]; //显示相应的数字
	if(SEG_Dot==n) P0|=0x80; //显示小数点
	
	P3&=(0x0f|0xff7f>>n);	//显示在相应的位置上
	
}
#endif
////////////////////////////////////////////
#if USE_TIM0
unsigned int TIM0_Time;
void TIM0_init(unsigned int ms)
{
	TIM0_Time=(unsigned short)(65536-SysClock*ms/12000); //设置为(ms)ms产生一次中断
	TMOD |= 0x01;                    //set timer0 as mode1 (16-bit)
	TL0 = TIM0_Time;                     //initial timer0 low byte
	TH0 = TIM0_Time >> 8;                //initial timer0 high byte
	TR0 = 1;                        //timer0 start running
	ET0 = 1;                        //enable timer0 interrupt
	EA = 1;                         //open global interrupt switch
}

//void TIM0_CallBack(void) {}


#if USE_SYST0 //使用系统时钟模式
unsigned long int SYS_Tim=0;

/*
**name:Get_SysTime()|SYS_TIME=Get_SysTime();获取当前系统时间，单位为毫秒,从初始化TIM0开始为0ms
**para:non
**return:当前系统时间，最长为4294967295ms，约为49.7天
**note:需要打开定时器0中断，此库中即调用 TIM0_init()
*/
unsigned long int Get_SysTime(void)
{
	return SYS_Tim;
}

/*
**name:Delay()|Delay(1); 延时1ms
**para:n 延时时间，单位ms
**return:non
**note:需要打开定时器0中断，此库中即调用 TIM0_init()
*/
void Delay(unsigned int n)
{
	unsigned long int time=Get_SysTime();
	while((Get_SysTime()-time)<n)
	{
		//可以添加一些在延时时执行的程序，执行时间不要大于1ms
	}
}

/*
**name: tm0_isr()|定时器0中断服务函数
**para:non
**return:non
**note:默认设置为自动重载，每1ms中断一次，并将SYS_Tim加1，提供一个系统时钟
*/
void tm0_isr() interrupt 1 using 1
{
    TL0 = TIM0_Time;                     //reload timer0 low byte
    TH0 = TIM0_Time >> 8;                //reload timer0 high byte
	SYS_Tim++;
//	SEG_Cnt++;
//	if(SEG_Cnt==4) SEG_Cnt=0;
//	SEG_Display(SEG_Cnt);
}
#else  //使用外部定时调用模式，TIM0_CallBack()为外部声明的回调函数，外部未声明将报错
extern void TIM0_CallBack(void);
/*
**name: tm0_isr()|定时器0中断服务函数
**para:non
**return:non
**note:默认设置为自动重载，按设定时间TIM0_Time中断，定时执行TIM0_CallBack()
*/
void tm0_isr() interrupt 1 using 1
{
    TL0 = TIM0_Time;                     //reload timer0 low byte
    TH0 = TIM0_Time >> 8;                //reload timer0 high byte
	TIM0_CallBack();
}

#endif



#endif
////////////////////////////////////////////
#if USE_TIM1
unsigned int TIM1_Time;
void TIM1_init(unsigned int ms )
{
	TIM1_Time=(unsigned short)(65536-SysClock*ms/12000); //设置为(ms)ms产生一次中断
	TMOD |= 0x10;                    //set timer0 as mode1 (16-bit)
	TL1 = TIM1_Time;                     //initial timer0 low byte
	TH1 = TIM1_Time >> 8;                //initial timer0 high byte
	TR1 = 1;                        //timer0 start running
	ET1 = 1;                        //enable timer0 interrupt
	EA = 1;                         //open global interrupt switch
}

//使用外部定时调用模式，TIM1_CallBack()为外部声明的回调函数，外部未声明将报错
extern void TIM1_CallBack(void);
/*
**name: tm1_isr()|定时器1中断服务函数
**para:non
**return:non
**note:默认设置为自动重载，按设定时间TIM1_Time中断，定时执行TIM1_CallBack()
*/
void tm1_isr() interrupt 3 using 1
{
    TL1 = TIM1_Time;                     //reload timer1 low byte
    TH1 = TIM1_Time >> 8;                //reload timer1 high byte
    TIM1_CallBack();

}
#endif
////////////////////////////////////////////
#if USE_UART
#ifndef RAUD
	#define BAUD 9600       //UART baudrate
#endif

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT NONE_PARITY   //Testing even parity

unsigned char RX_Buff;
bit busy;

void UART_init(void)
{
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
#endif
//#if USE_PRINTF
//	 TI = 1;
//#endif	
    TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //Set auto-reload vaule
    TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
    T2CON = 0x34;           //Timer2 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch

}


void Uart_Isr() interrupt 4 using 1
{
	
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
        RX_Buff = SBUF;    
		
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

void SendData(unsigned char dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {
		#if (PARITYBIT == ODD_PARITY)
				TB8 = 0;            //Set parity bit to 0
		#elif (PARITYBIT == EVEN_PARITY)
				TB8 = 1;            //Set parity bit to 1
		#endif
    }
    else
    {
		#if (PARITYBIT == ODD_PARITY)
				TB8 = 1;            //Set parity bit to 1
		#elif (PARITYBIT == EVEN_PARITY)
				TB8 = 0;            //Set parity bit to 0
		#endif
    }
    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}

void SendDatas(char *s,char len)
{
	while(len--)
	{
		SendData(*s++);
	}
}


//int fputc(int ch, FILE *f)
//{
//	SendData(ch);
//    return ch;
//}

#endif





