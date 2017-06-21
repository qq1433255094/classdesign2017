#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <REGX52.H>
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
void delay_n40us(uint n);//延时
void DisplayListChar (unsigned char x,unsigned char y,uchar dat);//显示的字符串
 void DisplayListString(unsigned char x,unsigned char y,char *s);
 void DisplayListNum(unsigned char x,unsigned char y,int num);
 void DisplayClear();


#endif