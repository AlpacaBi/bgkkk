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
    if(PcdRequest(0x52,Temp)==MI_OK){  //Ѱ��
      if(PcdAnticoll(UID)==MI_OK){	  //����ͻ��UID��������Ϊ�����к�
           find_card_success();	 //
		   jiance();       
      }
    }else beep = 1;//Ѱ��ʧ��
  } 
}





//������
void main( ){   
    init( );			//ϵͳ��ʼ��
    find_the_card();		//ѭ��Ѱ��״̬
}