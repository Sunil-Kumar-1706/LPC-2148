#include<LPC21xx.h>
#define DACR (*((volatile unsigned long *) 0xE006C000)) // DAC Register address 

void delay_micro(unsigned int s)
{
	T0PR = 15000-1; 
	T0TCR = 0x01;
	while(T0TC < s);
	T0TCR = 0x03;
	T0TCR = 0x00;	
}

void delay_milli(unsigned int s)
{
	T0PR = 15-1; 
	T0TCR = 0x01;
	while(T0TC < s);
	T0TCR = 0x03;
	T0TCR = 0x00;	
}

void analogWrite(unsigned int DAC_data)
{
	PINSEL1=0x00080000;
	DACR = DAC_data << 6;
}

int main()
{
    int i;

    while(1)
    {
        for(i = 0; i < 100; i++)  
        {
            analogWrite(i * 10);  
            delay_micro(200);       
        }
    }
}



