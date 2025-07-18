#include<LPC21XX.h>
#define BUTTON 14
void delay()
{
	unsigned int i;
	for(i=0;i<200000;i++);
}
int main()
{
	IODIR0|=(1<<15);//Pin 15 as OUTPUT
	while(1)
	{
		if(((IOPIN0>>BUTTON)&1)==0)
		{
			IOCLR0|=(1<<15);//Turn ON LED
			delay();
		}
		else
		{
			IOSET0|=(1<<15);//Turn OFF LED
			delay();
		}
	}
}

