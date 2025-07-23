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

void delay(unsigned int s)
{
		T0PR=15000000-1;
		T0TCR=0x01;
		while(T0TC<s);
		T0TCR=0x03;
		T0TCR=0x00;
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

unsigned int ADC_Check()
{
	ADCR = 0x00000000;
	ADCR|=(0x01<<2);
	ADCR|=(13<<8);
	ADCR|=(0x01<<16);
	ADCR|=(0x01<<21);
	
	adc_val = ADC_Val();

	delay_ms(100);
	return adc_val;		
}

void uart_init()
{
	PINSEL0 = 0X05;
	U0LCR = 0x83;
	U0DLL = 97;
	U0DLM = 0;
	U0LCR = 0x03;
}

void uart_tx(unsigned char ch)
{
	U0THR = ch;
	while((U0LSR>>5&1)==0);
}

unsigned char uart_rx()
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
	unsigned int x;
	PINSEL1|=0x4000000;
	lcd_init();
	uart_init();
	while(1)
	{
		x = ADC_Check();
		lcd_command(0x80);
		lcd_command(0x01);
		lcd_string("TEMP:");
		lcd_int(x);
		delay_ms(100);

		if(x>27)
		{
			IOSET0|=(1<<7);
			uart_str("Temperature Exceeded Threshold Value\r\n");
			delay_ms(100);
		}
		else
		{
			IOCLR0|=(1<<7);
			uart_str("Temperature Within Threshold Value\r\n");
			delay_ms(100);

		}
		
		if(((IOPIN0>>14)&1)==0)
		{
			lcd_command(0xc0);
			lcd_string("Motion Detected");
			uart_str("Motion Detected\r\n");
			delay_ms(100);
		}
		else
		{
			lcd_command(0xc0);
			lcd_string("No Motion Detected");
			uart_str("No Motion Detected\r\n");
			delay_ms(100);

		}
		delay(2);
	}
}