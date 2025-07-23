#include<LPC21xx.h>

#define LED 1<<7

void delay(unsigned int s)
{
	T0PR=15000000-1;
	T0TCR=0x01;
	while(T0TC<s);
	T0TCR=0x03;
	T0TCR=0x00;
}

void uart_init(void)
{
	PINSEL0=0x05;
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x03;
}

void uart_tx(unsigned char ch)
{
	U0THR=ch;
	while(((U0LSR>>5)&1)==0);
}

unsigned char uart_rx(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}

void uart_str(unsigned char* s)
{
	while(*s)
		uart_tx(*s++);
}

int main()
{
	unsigned char c; 
	IODIR0|=LED;
	uart_init();
	while(1)
	{
		c=uart_rx();
		if(c=='A')
		{
			uart_str("LED ON");
			IOSET0|=LED;
			delay(2);
		} 
		else if(c=='B')
		{
			uart_str("LED OFF");
			IOCLR0|=LED;
			delay(2);
		}
	}
}

