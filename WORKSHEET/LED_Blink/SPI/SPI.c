#include<LPC21xx.h>

#define SCK 1<<8 
#define MISO 1<<10 
#define MOSI 1<<12 
#define CS 1<<7

#define MODE_3 0X18 
#define MSTR 1<<5 

#define PCLK 15000000 
#define SPI_RATE 100000 
#define SPEED (PCLK/SPI_RATE)

#define lcd_d 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

//DATA TYPEDEF 
typedef unsigned char u8; 
typedef signed int s32; 
typedef unsigned int u32; 
typedef float f32;

void SPI_INIT(void) 
{ 
PINSEL0|=SCK|MISO|MOSI; 
S0SPCCR=SPEED; 
S0SPCR=(MSTR|MODE_3);//BITS 0 0 1 1 1 0 0 0 
IODIR0|=CS;//SET 7TH PIN OUTPUT FOR CS 
IOSET0=CS;//INITIAL HIGH 
} 

u8 SPI_TRANSMIT(u8 data) 
{ 
u8 stat; 
stat=S0SPSR; 
stat++;//TO REMOVE NEVER USED ERROR 
S0SPDR=data; 
while(((S0SPSR>>7)&1)==0); 
return S0SPDR; 
}
 
float ADC_SPI_READ(u32 channelno) 
{ 
u8 HBYTE,LBYTE; 
f32 ADC=0;
IOCLR0=CS; 
SPI_TRANSMIT(0X60); 
HBYTE=SPI_TRANSMIT(channelno<<6); 
LBYTE=SPI_TRANSMIT(0x00); 
IOSET0=CS; 
ADC=(((HBYTE&0X0F)<<12)|LBYTE); 
return ((ADC*3.3)/4096);  
} 

void delay_ms(u32 ms)  
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}

void lcd_command(u8 cmd)
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

void lcd_data(u8 d)
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

void lcd_init(void)
{
				IODIR1|=lcd_d|rs|rw|e;
				lcd_command(0x01);
				lcd_command(0x02);
				lcd_command(0x0c);
				lcd_command(0x28);
				lcd_command(0x80);
}

void lcd_int(s32 a)
{
	u8 b[10];
	s32 i=0;
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

void lcd_string(u8 *s)
{
	while(*s)
		lcd_data(*s++);
}

void lcd_float(f32 f)
{
	s32 temp;
	temp=f;
	lcd_int(temp);
	lcd_data('.');
	temp=(f-temp)*1000;
	lcd_int(temp);
}

int main() 
{ 
	f32 f; 
	SPI_INIT(); 
	lcd_init(); 
	while(1) 
	{ 
		f=ADC_SPI_READ(0); 
		lcd_command(0x80);
		lcd_string("TEMP:");
		lcd_float(f);
	} 
} 
