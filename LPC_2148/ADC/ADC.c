#include<LPC21XX.h>

#define lcd_d 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

unsigned int adc_val = 0;
float volt = 0.0;

void delay_ms(unsigned int ms)  
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}

void lcd_command(unsigned char cmd)
{
				IOCLR1=lcd_d;
				IOSET1=(cmd&0Xf0)<<16;
				IOCLR1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
				
				IOCLR1=lcd_d;
				IOSET1=(cmd&0X0f)<<20;
				IOCLR1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
}

void lcd_data(unsigned char d)
{				
				IOCLR1=lcd_d;
				IOSET1=(d&0Xf0)<<16;
				IOSET1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
				
				IOCLR1=lcd_d;
				IOSET1=(d&0X0f)<<20;
				IOSET1=rs;
				IOCLR1=rw;
				IOSET1=e;
				delay_ms(2);
				IOCLR1=e;
}

void lcd_init()
{
				IODIR1|=lcd_d|rs|rw|e;
				lcd_command(0x01);
				lcd_command(0x02);
				lcd_command(0x0c);
				lcd_command(0x28);
				lcd_command(0x80);
}

void lcd_int(int a)
{
	unsigned char b[10];
	int i=0;
	if(a==0)
		lcd_data(0+48);
	else
		{
			if(a<0)
			{
				lcd_data('-');
				a=-a;
			}
			while(a>0)
			{
				b[i++]=a%10;
				a/=10;
			}
	for(i--;i>=0;i--)
		lcd_data(b[i]+48);
		}
}

void lcd_string(char *s)
{
	while(*s)
		lcd_data(*s++);
}

void lcd_float(float f)
{
	int temp;
	temp=f;
	lcd_int(temp);
	lcd_data('.');
	temp=(f-temp)*1000;
	lcd_int(temp);
}

unsigned int ADC_Val()
{
	unsigned int data;
	
	ADCR|=(0x01<<24);
	while(((ADDR>>31)&1)==0);
	data = ADDR;
	data = data>>6 & 0x3ff;

	volt = ((3.3*data)/1023)*100;

	return volt;
}

void ADC_Check()
{
	ADCR = 0x00000000;
	ADCR|=(0x01<<2);
	ADCR|=(13<<8);
	ADCR|=(0x01<<16);
	ADCR|=(0x01<<21);
	
	lcd_command(0x80);
	lcd_string("ADC:");

	adc_val = ADC_Val();

	lcd_command(0x86);
	lcd_int(adc_val);		
}

int main()
{
	PINSEL1|=0x4000000;
	lcd_init();
	while(1)
	{
		ADC_Check();
	}
}