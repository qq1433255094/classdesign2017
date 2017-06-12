#ifndef __LIB51_H__
#define __LIB51_H__

#include "stdio.h"
#define SysClock 12000000L
#define FOSC SysClock
#define PI 3.1415926F

//使用:1 不使用:0
#define USE_SEG		0	//使用数码管 

#define USE_TIM0 	1	//使用定时器0

#define USE_SYST0 	1	//使用定时器0产生全局时钟

#define USE_TIM1	0	//使用定时器1

#define USE_UART	1	//使用串口

//#define USE_PRINTF	1



#if USE_SEG  
static SEG_DATA[11]={ //共阴数码管
		0x3f,	//0
		0x06,	//1
		0x5b,	//2
		0x4f,	//3
		0x66,	//4
		0x6d,	//5
		0x7d, 	//6
		0x07,	//7
		0x7f, 	//8
		0x6f, 	//9
		0x00
	};
void SEG_Display(unsigned char n);
#endif

#if USE_TIM0
void TIM0_init(unsigned int ms);
#endif

#if USE_SYST0
unsigned long int Get_SysTime(void);
void Delay(unsigned int n);
#endif

#if USE_TIM1
void TIM1_init(unsigned int ms);
#endif

#if USE_UART
void UART_init(void);
void SendData(unsigned char dat);
void SendString(char *s);
void SendDatas(char *s,char len);
#endif	

#endif