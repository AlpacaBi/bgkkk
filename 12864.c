


    /*******************************************************/
    /*12864��ʾ���Ĳ���ģ�飬��Ҫ������ʾ����ʾ������	   */
    /*******************************************************/



#include <reg51.h>
#include <intrins.h>
#include <main.h>
	
#define uchar unsigned char
#define uint  unsigned int



/*TS12864A-3 �˿ڶ���*/
#define LCD_data  P0             //���ݿ�
sbit LCD_RS  =  P3^5;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P3^6;            //Һ����/д����
sbit LCD_EN  =  P3^4;            //Һ��ʹ�ܿ���
sbit LCD_PSB =  P3^7;            //��/����ʽ����
sbit wela    =  P2^6;
sbit dula    =  P2^7;
 //******************************************************
uchar code dis1[] = {"�ع�ѧԺ"};					//��ӭ����	
uchar code dis2[] = {">>����c312<"};
uchar code dis3[] = {"RFID�����Ž�ϵͳ "};
uchar code dis4[] = {"��ˢ������ "};
 //******************************************************

uchar code dis5[] = {"��ӭ��������"};
uchar code dis7[] = {"���Ŀ���Ϊ"};

 //******************************************************
uchar code dis8[] = {"��Ǹ���˿�δע��"};			//���󿨽���
uchar code dis9[] = {"���ס���Ŀ��ţ�"};
uchar code dis10[] = {"����ϵ����Ա"};
  //******************************************************


#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
uchar IRDIS[2];
uchar IRCOM[4];
void delay0(uchar x);  //x*0.14MS

void  dataconv();
void lcd_pos(uchar X,uchar Y);  //ȷ����ʾλ��
/*******************************************************************/
/*                                                                 */
/*  ��ʱ����                                                       */
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
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
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
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
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
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
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
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

    LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ�����
    delay(5);
    lcd_wcmd(0x30);      //����ָ�����
    delay(5);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(5);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(5);
}

/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
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
   lcd_wcmd(pos);     //��ʾ��ַ
}


void LCD_Data(unsigned char LCDbyte)	 //��4λ���ű�Ϊ8λ��ʾ
{
	 unsigned char count;
	 
	 count=(LCDbyte&0xf0)>>4;
	 if (count>0x9) lcd_wdat(count%9+0x40);
	 else lcd_wdat(count+0x30);
	 
	 count=LCDbyte&0x0f;
	 if (count>9) lcd_wdat(count%9+0x40);
	 else lcd_wdat(count+0x30);
}



void display_cardnum()		   //��ʾ����
{
	uchar num;
    //write_com(0x01);      //���LCD����ʾ����
    delay(5);
    lcd_pos(3,0);
    
	for(num=0;num<4;num++)
	{

		
		LCD_Data(UID[num]);//д����
	}
}	

void display_cardnumerr()		   //��ʾ���󿨺�
{
	uchar num;
    //write_com(0x01);      //���LCD����ʾ����
    delay(5);
    lcd_pos(2,0);
    
	for(num=0;num<4;num++)
	{
      LCD_Data(UID[num]);//д����
	}
}	

/******************************�û���ʾ****************************************************************/

void dispwel()
{
    uchar i;
    delay(10);                 //��ʱ
    wela=0;
	dula=0; 
    lcd_init();                //��ʼ��LCD             
  
    lcd_pos(0,0); i = 0;while(dis1[i] != '\0'){lcd_wdat(dis1[i]);i++;}
    lcd_pos(1,0); i = 0;while(dis2[i] != '\0'){lcd_wdat(dis2[i]);i++;}
 	lcd_pos(2,0); i = 0;while(dis3[i] != '\0'){lcd_wdat(dis3[i]);i++;}
	lcd_pos(3,0); i = 0;while(dis4[i] != '\0'){lcd_wdat(dis4[i]);i++;}
}

//*************************************************************** �Ϲ���
uchar code dis_bgk[] = {"�Ϲ���"};
void disbgk()
{
    uchar i;
    delay(10);                 //��ʱ
    wela=0;
	dula=0; 
    lcd_init();                //��ʼ��LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_bgk[i] != '\0')  {lcd_wdat(dis_bgk[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}

//*************************************************************** ���
uchar code dis_yjj[] = {"���"};
void disyjj()
{
    uchar i;
    delay(10);                 //��ʱ
    wela=0;
	dula=0; 
    lcd_init();                //��ʼ��LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_yjj[i] != '\0')  {lcd_wdat(dis_yjj[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}
//*************************************************************** ����Ȩ
uchar code dis_cxq[] = {"����Ȩ"};
void discxq()
{
    uchar i;
    delay(10);                 //��ʱ
    wela=0;
	dula=0; 
    lcd_init();                //��ʼ��LCD             
    lcd_pos(0,0); i = 0; while(dis5[i] != '\0')     { lcd_wdat(dis5[i]);i++;}
    lcd_pos(1,0); i = 0; while(dis_cxq[i] != '\0')  {lcd_wdat(dis_cxq[i]);i++;}
 	lcd_pos(2,0); i = 0; while(dis7[i] != '\0')     {lcd_wdat(dis7[i]);i++; }
	lcd_pos(3,0); i = 0; display_cardnum();    
}


/******************************�û���ʾend****************************************************************/






void diserr()
{
    uchar i;
    delay(10);                 //��ʱ
    wela=0;
	dula=0; 
    lcd_init();                //��ʼ��LCD             
  
    lcd_pos(0,0); i = 0; while(dis8[i] != '\0'){ lcd_wdat(dis8[i]); i++;}
    lcd_pos(1,0); i = 0; while(dis9[i] != '\0'){ lcd_wdat(dis9[i]); i++;}
 	lcd_pos(2,0); i = 0; display_cardnumerr();
	lcd_pos(3,0); i = 0; while(dis10[i] != '\0'){lcd_wdat(dis10[i]);i++;}
 }   