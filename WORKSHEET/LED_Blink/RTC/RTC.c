#include <LPC21xx.h>
#include <stdio.h>
#include <stdint.h>

#define lcd_d 0x0f<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_month;
    uint8_t day_of_week;
    uint16_t day_of_year;
    uint8_t month;
    uint16_t year;
} RTC_Time;


void RTC_Set_Time(RTC_Time set_time) {
    SEC   = set_time.seconds;
    MIN   = set_time.minutes;
    HOUR  = set_time.hours;
    DOM   = set_time.day_of_month;
    DOW   = set_time.day_of_week;
    DOY   = set_time.day_of_year;
    MONTH = set_time.month;
    YEAR  = set_time.year;
}

RTC_Time RTC_Get_Time(void) {
    RTC_Time time;
    time.seconds      = SEC;
    time.minutes      = MIN;
    time.hours        = HOUR;
    time.day_of_month = DOM;
    time.day_of_week  = DOW;
    time.day_of_year  = DOY;
    time.month        = MONTH;
    time.year         = YEAR;
    return time;
}

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

int main(void) 
{
    char time_str[17];
    RTC_Time set_time, current_time;

    // Initialize LCD
    lcd_init();

    // Set time: (seconds, minutes, hours, day, weekday, doy, month, year)
    set_time.seconds = 0;
    set_time.minutes = 30;
    set_time.hours   = 12;
    set_time.day_of_month = 23;
    set_time.day_of_week  = 2;
    set_time.day_of_year  = 204;
    set_time.month        = 7;
    set_time.year         = 2025;

    // RTC setup
    CCR = 0x02;  // Reset clock
    PREINT = 0x01C8;  // For 15MHz PCLK
    PREFRAC = 0x61C0;
    RTC_Set_Time(set_time);
    CCR = 0x01;  // Enable RTC

    while (1) {
        current_time = RTC_Get_Time();

        sprintf(time_str, "Time: %02d:%02d:%02d", 
                current_time.hours, 
                current_time.minutes, 
                current_time.seconds);

        lcd_command(0x80);       // First line
        lcd_string("Current Time:");
        lcd_command(0xC0);       // Second line
        lcd_string(time_str);

        delay_ms(1000);
    }
}
