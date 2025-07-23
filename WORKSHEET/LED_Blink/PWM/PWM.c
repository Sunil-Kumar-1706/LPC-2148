#include<lpc21xx.h>

unsigned int long x;

int main()
{
	x=300;
	PINSEL1=0X400;// P0.21 as PWM 
	PWMMR0=0Xf;   
	PWMMR5=0X0;		
	PWMMCR=0X3;		
	PWMPCR=0X2000; 
	PWMTCR=0X9;   
	PWMLER=0X20;  
	while(1)
	{
	while(PWMIR!=0X1);  // Wait until interrupt raise
	PWMIR=0X01;  
	PWMMR5=x;  
	if(x>=PWMMR0)
		x=0;
	x=x+300;  // Increasing duty cycle
	PWMLER=0X20;
	}
}

