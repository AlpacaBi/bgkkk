#include<reg52.h>
#include<main.h>
#include<mfrc522.h>
unsigned char UID[5],Temp[4];

void init(){		
	beep = 0;
    PcdReset();
    PcdAntennaOff(); 
    PcdAntennaOn();  
	M500PcdConfigISOType( 'A' );
	lock_off();
	dispwel();
}

void find_the_card(void){
  while(1){
    if(PcdRequest(0x52,Temp)==MI_OK){  //寻卡
      if(PcdAnticoll(UID)==MI_OK){	  //防冲突，UID数组数据为卡序列号
           find_card_success();	 //
		   jiance();       
      }
    }else beep = 1;//寻卡失败
  } 
}





//主函数
void main( ){   
    init( );			//系统初始化
    find_the_card();		//循环寻卡状态
}