#ifndef __LIB51_H__
#define __LIB51_H__


#define __WEAK                    __attribute__((weak))
#define SysClock 12000000L
#define PI 3.1415926F

#ifdef USE_SEG
	SEG_DATA[]={
		0x3f,	//0
		0x06,	//1
		0x5b,	//2
		0x4f,	//3
		0x66,	//4
		0x6d,	//5
		0x7d, //6
		0x07,	//7
		0x7f, //8
		0x6f 	//9
	};
#endif

void TIM0_init(unsigned int ms);



#endif