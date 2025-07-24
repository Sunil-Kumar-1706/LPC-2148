#include<lpc21xx.h>

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
void uart_str_rec(unsigned char* s, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        s[i] = uart_rx();
        if (s[i] == '\r')
        {
            s[i] = '\0';
            break;
        }
    }
}