


    /*******************************************************/
    /*12864显示屏的操作模块，主要控制显示屏显示的内容	   */
    /*******************************************************/



#include <reg51.h>
#include <intrins.h>
#include <main.h>
	
#define uchar unsigned char
#define uint  unsigned int



/*TS12864A-3 端口定义*/
#define LCD_data  P0             //数据口
sbit LCD_RS  =  P3^5;            //寄存器选择输入 
sbit LCD_RW  =  P3^6;            //液晶读/写控制
sbit LCD_EN  =  P3^4;            //液晶使能控制
sbit LCD_PSB =  P3^7;            //串/并方式控制
sbit wela    =  P2^6;
sbit dula    =  P2^7;
 //******************************************************
uchar code dis1[] = {"韶关学院"};					//欢迎界面	
uchar code dis2[] = {">>丁香c312<"};
uchar code dis3[] = {"RFID智能门禁系统 "};
uchar code dis4[] = {"请刷卡！！ "};
 //******************************************************

uchar code dis5[] = {"欢迎回来！！"};
uchar code dis7[] = {"您的卡号为"};

 //******************************************************
uchar code dis8[] = {"抱歉，此卡未注册"};			//错误卡界面
uchar code dis9[] = {"请记住您的卡号："};
uchar code dis10[] = {"并联系管理员"};
  //******************************************************


#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
uchar IRDIS[2];
uchar IRCOM[4];
void delay0(uchar x);  //x*0.14MS

void  dataconv();
void lcd_pos(uchar X,uchar Y);  //确定显示位置
/*******************************************************************/
/*                                                                 */
/*  延时函数                                                       */
/*                                                                 */
/*******************************************************************/
void delay(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}		
/*******************************************************************/
/*                                                                 */
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }
/*******************************************************************/
/*                                                                 */
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

    LCD_PSB = 1;         //并口方式
    
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}

/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     //显示地址
}


void LCD_Data(unsigned char LCDbyte)	 //把4位卡号变为8位显示
{
	 unsigned char count;
	 
	 count=(LCDbyte&0xf0)>>4;
	 if (count>0x9) lcd_wdat(count%9+0x40);
	 else lcd_wdat(count+0x30);
	 
	 count=LCDbyte&0x0f;
	 if (count>9) lcd_wdat(count%9+0x40);
	 else lcd_wdat(count+0x30);
}



void display_cardnum()		   //显示卡号
{
	uchar num;
    //write_com(0x01);      //清除LCD的显示内容
    delay(5);
    lcd_pos(3,0);
    
	for(num=0;num<4;num++)
	{

		
		LCD_Data(UID[num]);//写数字
	}
}	

void display_cardnumerr()		   //显示错误卡号
{
	uchar num;
    //write_com(0x01);      //清除LCD的显示内容
    delay(5);
    lcd_pos(2,0);
    
	for(num=0;num<4;num++)
	{
      LCD_Data(UID[num]);//写数字
	}
}	

/******************************用户显示****************************************************************/

void dispwel()
{
    uchar i;
    delay(10);                 //延时
    wela=0;
	dula=0; 
    lcd_init();                //初始化LCD             
  
    lcd_pos(0,0); i = 0;while(dis1[i] != '\0'){lcd_wdat(dis1[i]);i++;}
    lcd_pos(1,0); i = 0;while(dis2[i] != '\0'){lcd_wdat(dis2[i]);i++;}
 	lcd_pos(2,0); i = 0;while(dis3[i] != '\0'){lcd_wdat(dis3[i]);i++;}
	lcd_pos(3,0); i = 0;while(dis4[i] != '\0'){lcd_wdat(dis4[i]);i++;}
}

//*************************************************************** 毕国康
uchar code dis_bgk[] = {"毕国康"};
void disbgk()
{
    uchar i;
    delay(10);                 //延时
    wela=0;
	dula=0; 
    lcd_init();                //初始化LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_bgk[i] != '\0')  {lcd_wdat(dis_bgk[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}

//*************************************************************** 杨炯建
uchar code dis_yjj[] = {"杨炯建"};
void disyjj()
{
    uchar i;
    delay(10);                 //延时
    wela=0;
	dula=0; 
    lcd_init();                //初始化LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_yjj[i] != '\0')  {lcd_wdat(dis_yjj[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}
//*************************************************************** 陈贤权
uchar code dis_cxq[] = {"陈贤权"};
void discxq()
{
    uchar i;
    delay(10);                 //延时
    wela=0;
	dula=0; 
    lcd_init();                //初始化LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_cxq[i] != '\0')  {lcd_wdat(dis_cxq[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}


/******************************用户显示end****************************************************************/






void diserr()
{
    uchar i;
    delay(10);                 //延时
    wela=0;
	dula=0; 
    lcd_init();                //初始化LCD             
  
    lcd_pos(0,0); i = 0; while(dis8[i] != '\0'){ lcd_wdat(dis8[i]); i++;}
    lcd_pos(1,0); i = 0; while(dis9[i] != '\0'){ lcd_wdat(dis9[i]); i++;}
 	lcd_pos(2,0); i = 0; display_cardnumerr();
	lcd_pos(3,0); i = 0; while(dis10[i] != '\0'){lcd_wdat(dis10[i]);i++;}
 }   