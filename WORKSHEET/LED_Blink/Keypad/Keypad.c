#include<LPC21xx.h>
#include<string.h>
#define c0 (IOPIN0&(1<<5))
#define c1 (IOPIN0&(1<<6))
#define c2 (IOPIN0&(1<<7))
#define c3 (IOPIN0&(1<<8))
#define r0 1<<0
#define r1 1<<1
#define r2 1<<2
#define r3 1<<3

#define LED 1<<9

#define lcd_d 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

unsigned char key[4][4]={{'1','2','3','4'},{'5','6','7','8'},{'9','10','11','12'},{'13','14','15','16'}};


void delay(unsigned int s)
{
	T0PR=15000000-1;
	T0TCR=0x01;
	while(T0TC<s);
	T0TCR=0x03;
	T0TCR=0x00;
}

void delay_ms(unsigned int ms)  
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
}

unsigned char key_scan(void)
{
	unsigned char row_val,col_val;
	IODIR0|=r0|r1|r2|r3;
	
	while(1)
	{
		IOCLR0|=r0|r1|r2|r3;
		IOSET0|=c0|c1|c2|c3;

		while((c0&&c1&&c2&&c3)==1);

		IOCLR0|=r0;
		IOSET0|=r1|r2|r3;
		if(((c0&&c1&&c2&&c3)==0))
		{
			row_val=0;
			break;
		}

		IOCLR0|=r1;
		IOSET0|=r0|r2|r3;
		if(((c0&&c1&&c2&&c3)==0))
		{
			row_val=1;
			break;
		}
			
		IOCLR0|=r2;
		IOSET0|=r1|r0|r3;
		if(((c0&&c1&&c2&&c3)==0))
		{
			row_val=2;
			break;
		}
			
		IOCLR0|=r3;
		IOSET0|=r1|r2|r0;
		if(((c0&&c1&&c2&&c3)==0))
		{
			row_val=3;
			break;
		}
	}
if(((c0&&c1&&c2&&c3)==0))
{
	if(c0==0)
		col_val=0;
	else if(c1==0)
		col_val=1;
	else if(c2==0)
		col_val=2;
	else if(c3==0)
		col_val=3;
}
		
	while(((c0&&c1&&c2&&c3)==0));
	delay_ms(500);
	return key[row_val][col_val];	
	
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

void lcd_string(char *s)
{
	while(*s)
		lcd_data(*s++);
}

int main()
{
	char s[5];
	int i;
	lcd_init();
	lcd_command(0x80);
	lcd_string("KEYPAD");
	IODIR0|=LED;
	lcd_command(0X01);
	lcd_command(0xc0);
	
	for(i=0;i<5;i++)
	{
		s[i]=key_scan();
		lcd_data('*');
	}
	lcd_string(s);
	if(strcmp(s,"12345")==0)
	{
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("PASSWORD CORRECT");
		delay_ms(500);
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("LED ON");
		
		IOSET0|=LED;
		
	}
	else
	{
		lcd_command(0X01);
		lcd_command(0x80);
		lcd_string("INVALID");
		IOCLR0|=LED;

	}
}


 