/*
 * SeisoMachine.c
 *
 * Created: 2021/07/10 2:30:22
 * Author : SIMPSONMAN
 */ 

#define	F_CPU	8000000UL		// CKSEL[1:0]=10, CKDIV8=0
#define _BV(x) 1<<x
#define loop_until_bit_is_set(sfr,bit) do { } while (!(sfr & _BV(bit)))

	
//#define PlaySound(x) dfCommand[5]=(char)(x>>8);dfCommand[6]=(char)(x & 0xFF);SendCommand(&dfCommand[0],(unsigned char)0x08);
	
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char dfCommand[8]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x01,0xEF};//ルートディレクトリの1番目の曲再生	
char specifyCommand[8]={0x7E,0xFF,0x06,0x0F,0x00,0x01,0x01,0xEF};//1フォルダ01ファイル
char firstFlag=1;
char volumeFlag=0;
unsigned volume=17;
uint16_t btnStatus=0x0000;
uint16_t btnStatusBefore=0x0000;
uint16_t btnStatusMax=0x0000;

void SendCommand(char* str,unsigned char num){
	for (char i=0;i<num;i++)
	{
		while(!(UCSR0A & 0b00100000));
		UDR0=*str;
		str++;
	}
}

void PlaySound(int16_t x)
{
	/*
	if (firstFlag)//音量設定
	{
		if ((~PIND) & (1<<7))
		{
			x+=10;
		}
		else if ((~PINB) & (1<<0))
		{
			x+=20;
		}
		dfCommand[3]=0x06;//音量読み上げ音量
		dfCommand[6]=20;
		SendCommand(&dfCommand[0],(unsigned char)0x08);
		dfCommand[3]=0x03;
		dfCommand[6]=0x01;
		
		specifyCommand[6]=x;
		SendCommand(&specifyCommand[0],(unsigned char)0x08);//音量読み上げ
		_delay_ms(1500);
		
		dfCommand[3]=0x06;
		dfCommand[6]=(char)x;//volume;
		SendCommand(&dfCommand[0],(unsigned char)0x08);
		dfCommand[3]=0x03;
		dfCommand[6]=0x01;
		volume=(char)x;
		
		firstFlag=0;
		return;
	}
	*/
	
	if ((~PIND) & (1<<7))
	{
		x+=10;
	}
	else if ((~PINB) & (1<<0))
	{
		x+=20;	
	}
	dfCommand[5]=(char)(x>>8);
	dfCommand[6]=(char)(x & 0xFF);	
	SendCommand(&dfCommand[0],(unsigned char)0x08);	
}


int main(void)
{
    /* Replace with your application code */
	
	//シリアル
	UBRR0H=0;
	UBRR0L=51;//9615bps
	//UDR0	データレジスタ
	UCSR0A=0b00000000;	//5bit 送信可能フラグ
	UCSR0B=0b00011000;	//送受信機能しよう
	UCSR0C=0b00000110;	//データ8ビット　stop 1bit パリティなし 非同期
	

		
	DDRC&=0xF0;//GPIO準備
	PORTC|=0x0F;
	DDRB&= 0b00111100;
	PORTB|=0b11000011;
	DDRD&= 0b01100011;
	PORTD|=0b10011100;
	
	_delay_ms(500);//待機	
	
	dfCommand[3]=0x06;
	dfCommand[6]=volume;
	SendCommand(&dfCommand[0],(unsigned char)0x08);//音量25
	dfCommand[3]=0x03;
	dfCommand[6]=0x01;
	
	
    while (1) 
    {
		
		if ((~PINC) & (1<<3))//PC3
		{
			PlaySound(1);
			btnStatus|=1<<0;
		}
		else btnStatus&=~(1<<0);
		
		if ((~PINC) & (1<<2))
		{
			PlaySound(2);
			btnStatus|=1<<1;
		}
		else btnStatus&=~(1<<1);
		
		if ((~PINC) & (1<<1))
		{
			PlaySound(3);
			btnStatus|=1<<2;
		}
		else btnStatus&=~(1<<2);
		
		if ((~PINC) & (1<<0))
		{
			PlaySound(4);
			btnStatus|=1<<3;
		}
		else btnStatus&=~(1<<3);
		
		if ((~PINB) & (1<<1))//PB1
		{
			PlaySound(5);
			btnStatus|=1<<4;
		}
		else btnStatus&=~(1<<4);		
		
		if ((~PIND) & (1<<2))//PD2
		{
			PlaySound(6);
			btnStatus|=1<<5;
		}
		else btnStatus&=~(1<<5);		
		
		if ((~PIND) & (1<<3))
		{
			PlaySound(7);
			btnStatus|=1<<6;
		}
		else btnStatus&=~(1<<6);
	
		if ((~PIND) & (1<<4))
		{
			PlaySound(8);
			btnStatus|=1<<7;
		}
		else btnStatus&=~(1<<7);	
		
		if ((~PINB) & (1<<6))//PB6
		{
			PlaySound(9);
			btnStatus|=1<<8;
		}
		else btnStatus&=~(1<<8);
		
		if ((~PINB) & (1<<7))
		{
			PlaySound(10);
			btnStatus|=1<<9;
		}
		else btnStatus&=~(1<<9);
		//2bit選曲
		if (((int16_t)btnStatusBefore-(int16_t)btnStatus)>0)
		{
			if (btnStatusMax>)
			{
			}
		}
		
		
		
		btnStatusBefore=btnStatus;
		//音量上げ
		if (volumeFlag==1 && ((PIND) & (1<<7)) && ((PINB) & (1<<0)))
		{
			volume++;
			if(volume>30) volume=30;
			dfCommand[3]=0x06;
			dfCommand[6]=volume;
			SendCommand(&dfCommand[0],(unsigned char)0x08);
			dfCommand[3]=0x03;
			dfCommand[6]=0x01;
			
			volumeFlag=0;
		}		
		//音量下げ
		if (volumeFlag==2 && ((PIND) & (1<<7)) && ((PINB) & (1<<0)))
		{
			volume--;
			if(volume<1) volume=1;
			dfCommand[3]=0x06;
			dfCommand[6]=volume;
			SendCommand(&dfCommand[0],(unsigned char)0x08);
			dfCommand[3]=0x03;
			dfCommand[6]=0x01;
					
			volumeFlag=0;
		}
		//音量ボタンフラグ
		if (((~PIND) & (1<<7)) && volumeFlag==0)//音量ダウン
		{
			volumeFlag=2;
		}
		else if (((~PINB) & (1<<0)) && volumeFlag==0)//音量アップ
		{
			volumeFlag=1;
		}

		
    }
}

