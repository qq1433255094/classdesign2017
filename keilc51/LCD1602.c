#include "LCD1602.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

void DisplayListChar (unsigned char x,unsigned char y,uchar dat)//显示的字符串
 { 
    unsigned address;
    if(y==1) 
        address=0x80+x;  
    else 
        address= 0xC0+x;
        LCD_write_command(address);
        LCD_write_data(dat);
 }

void LCD_init(void)//初始化程序
{ 
    LCD_write_command(0x38);
    LCD_write_command(0x0c);
    LCD_write_command(0x06);
    LCD_write_command(0x01);
    delay_n40us(100);
}

void LCD_write_command(uchar command)//指令程序
{ 
    LCD_DB=command;
    LCD_RS=0;
    LCD_RW=0; 
    LCD_E=1;
    LCD_E=0;
    delay_n40us(1);
 }

void LCD_write_data(uchar dat)//数据指令
 {
    LCD_DB=dat; 
    LCD_RS=1;
    LCD_RW=0;
    LCD_E=1;
    LCD_E=0; 
    delay_n40us(1); 
}

void delay_n40us(uint n)//延时
 {
    uint i;    
    uchar j;
    for(i=n;i>0;i--)      
        for(j=0;j<2;j++);
 }
 
 void DisplayListString(unsigned char x,unsigned char y,char *s)
 {
	 while(*s != 0 && x < 16)
	 {
		 DisplayListChar(x, y,*s);
         x++;
         s++;
	 }
     DisplayListChar(x, y,0);
 }

void DisplayListNum(unsigned char x,unsigned char y,int num)
 {
      char str[16];
      //itoa(num,str,10);
      sprintf(str,"%d",num);
      DisplayListString(x,y,str);
 }

void DisplayClear()
 {
      char i,j;
      for(j=0;j<2;j++)
        for(i=0;i<16;i++)
         DisplayListChar(i, j,0);
 }
 