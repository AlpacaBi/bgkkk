//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC522
sbit     MF522_NSS  =    P1^0;
sbit     MF522_SCK  =    P1^1;
sbit     MF522_SI   =    P1^2;
sbit     MF522_SO   =    P1^3;
sbit     MF522_RST  =    P1^4;                   //RC500片选

sbit     beep         =    P2^3;                   //蜂鸣器

//////////////////////////////////
//电机
/////////////////////////////////////////////////////////////////////

sbit     dianji  =    P1^7;



/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();                                                                   
#define OSC_FREQ          22118400L
#define  RCAP2_50us      65536L - OSC_FREQ/40417L
#define  RCAP2_1ms       65536L - OSC_FREQ/2000L
#define  RCAP2_10ms      65536L - OSC_FREQ/1200L
#define  TIME0_500us     65536L - OSC_FREQ/8000L
#define  TIME0_10ms      65536L - OSC_FREQ/200
#define CALL_isr_UART()         TI = 1
#define TRUE 1
#define FALSE 0

extern unsigned char UID[5];
void find_card_success();
void lock_on();
void lock_off();


void dispwel();
void disbgk();
void disyjj();
void discxq();

void display_cardnum() ;
void diserr() ;

void jiance();

#define uchar unsigned char
#define uint  unsigned int
