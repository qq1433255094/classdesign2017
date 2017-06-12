#include <stdio.h>
#include <REGX52.H>
#include "lib51.h"

void main()
{
	char i=0;
	char dir=1;
	short val = 0x0e38;
	
	UART_init();
	TIM0_init(1);
	while(1)
	{
		
		P2 = 0xff;
		dir =P2&0x01;
		
		if(dir >= 1)
		{
			i++;
			if(i==6)
				i=0;
		}
		else
		{
			i--;
			if(i==-1)
				i=5;
		}
		
		
			P1 = val>>(i+1);
			Delay(500);

	}
	
}