#include<LPC21XX.h>
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
		IOSET0|=(1<<15);//Turn ON LED
		delay();
		IOCLR0|=(1<<15);//Turn OFF LED
		delay();
	}
}

