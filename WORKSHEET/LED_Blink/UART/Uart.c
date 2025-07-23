#include<LPC21XX.h>
#define LED1 1<<7
#define LED2 1<<8

void uart_init(void)
{
	PINSEL0 = 0x05;
	U0LCR = 0x83;
	U0DLL = 97;
	U0DLM = 0;
	U0LCR = 0x03;
}

void uart_tx(unsigned char ch)
{
	U0THR = ch;
	while(((U0LSR>>5)&1)==0);
}

unsigned char uart_rx(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}

int main()
{
	unsigned char ch;
	uart_init();
	IODIR0|=LED1|LED2;
	while(1)
	{
		ch = uart_rx();
		if(ch == 'A')
		{
			IOSET0|=LED1;
		}
		else if(ch == 'B')
		{
			IOCLR0|=LED1;
		}
		else if(ch == 'C')
		{
			IOSET0|=LED2;
		}
		else if(ch == 'D')
		{
			IOCLR0|=LED2;
		}
		else
		{
			IOCLR0|=LED1|LED2;
		}
	}
}

