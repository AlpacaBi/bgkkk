#include <reg52.h>
#include <main.h>
#include <mfrc522.h>

void find_card_success()		  //寻卡成功响应函数
{   
	beep=0;  delay_10ms(5); 
	beep=1;  delay_10ms(5);
}

void open_the_lock()			  //开锁，保持开锁状态一会儿，然后闭锁
{
    lock_on();  delay_10ms(30);
	lock_off(); delay_10ms(30);
	dispwel();
}


void jiance()
{
	     if(UID[0]==0x0b&&UID[1]==0x34&&UID[2]==0x52&&UID[3]==0x35) {disbgk(); open_the_lock(); }  //毕国康
	else if(UID[0]==0x01&&UID[1]==0x02&&UID[2]==0x03&&UID[3]==0x04) {disbgk(); open_the_lock(); }  //毕国康的手机
	else if(UID[0]==0xfb&&UID[1]==0x00&&UID[2]==0x1e&&UID[3]==0x25) {disyjj(); open_the_lock(); }  //杨炯建
	else if(UID[0]==0x95&&UID[1]==0x59&&UID[2]==0xa0&&UID[3]==0x69) {discxq(); open_the_lock(); }  //陈贤权

	
                  
	else {diserr();  delay_10ms(60);  dispwel();}   //错误卡号界面
}
					