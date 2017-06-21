#include <reg52.h> 
#include <intrins.h> 
#define uchar unsigned char  
#define uint unsigned int 
#define LCD_DB P0 
sbit LCD_RS = P2^0;    
sbit LCD_RW = P2^1;
sbit LCD_E = P2^2;  
void LCD_init(void);//初始化
void LCD_write_command(uchar command);//写指令函数
void LCD_write_data(uchar dat);//写数据指令

void DisplayListChar (unsigned char x,unsigned char y,uchar dat)//显示的字符串
 { unsigned address;
 if(y==1) 
 address=0x80+x;  
 else 
 address= 0xC0+x;
 LCD_write_command(address);
 LCD_write_data(dat)	 ;
 }


 
 //DisplayListChar(0,0,"speed:--",8);//液晶1602第一行显示
//DisplayListChar(0,1,"stop",4);//液晶1602第二行显示 
void delay_n40us(uint n);//延时

void LCD_init(void)//初始化程序
{ 
LCD_write_command(0x38);
LCD_write_command(0x0c);
LCD_write_command(0x06);
 LCD_write_command(0x01);
 delay_n40us(100);
}
void LCD_write_command(uchar command)//指令程序
{ LCD_DB=command;
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

  void main(void)
  { LCD_init();
  // DisplayListChar(0,0,"speed:--",8);//液晶1602第一行显示
  while(1)
  {
	 DisplayListChar(0,1,'s');//液晶1602第二行显示
	 delay_n40us(1000);
  }
  
   }