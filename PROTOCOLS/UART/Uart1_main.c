#include<lpc21xx.h>
#include"uart_driver.h"
#include"lcd_driver.h"

int main()
{
	uart_init();
	lcd_init();
	lcd_command(0X80);
	lcd_command(0x01);
	uart_str("Hello\r");
	lcd_string("Hello");
	lcd_command(0xC0);
	lcd_string("transmitted");
  while(1);
		
	}