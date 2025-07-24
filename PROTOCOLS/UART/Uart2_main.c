#include<lpc21xx.h>
#include"uart_driver.h"
#include"lcd_driver.h"

unsigned char s[100]; 
	 
int main()
{
	uart_init();
	lcd_init();
	while(1)
	{
		lcd_command(0x80);
		uart_str_rec(s,100);
		lcd_string(s);
		lcd_command(0xC0);
		lcd_string("received");
		uart_str(s); 
	}
}
	
