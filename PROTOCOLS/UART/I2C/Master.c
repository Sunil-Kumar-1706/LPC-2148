#include<LPC21XX.h>
#include"I2C_Header.h"
#include"I2C_driver.h"
#include<string.h>
#include"lcd.h"


int main()
{
			lcd_init();
			i2c_init();
			lcd_string("I2C");
			i2c_Byte_write(0X50,0X0000,'A');
			delay_ms(500);
			
		 	while(1); 
}
		 