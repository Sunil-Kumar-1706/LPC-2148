#include<LPC21xx.h>
#define LED1 1<<7
#define LED2 1<<8
#define LED3 1<<9


void delay_seconds(unsigned int s)
{
	T0PR=15000000-1;
	T0TCR=0x01;
	while(T0TC<s);
	T0TCR=0x03;
	T0TCR=0x00;
}

int main()
{
	IODIR0|=LED1|LED2|LED3;
	while(1)
	{
		IOSET0|=LED1;
		IOCLR0|=LED2|LED3;
		delay_seconds(2);
		
		IOSET0|=LED2;
		IOCLR0|=LED1|LED3;
		delay_seconds(2);
		
		IOSET0|=LED3;
		IOCLR0|=LED2|LED1;
		delay_seconds(2);
	}
}
