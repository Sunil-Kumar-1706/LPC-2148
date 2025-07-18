#include<LPC21XX.h>
#define LCD_D 0xff
#define RS 1<<8
#define E 1<<9

void delay(unsigned int seconds)
{
	T0PR = 15000-1;
	T0TCR = 0x01;
	while(T0TC<seconds);
	T0TCR = 0x03;
	T0TCR = 0x00;	
}

void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0 = LCD_D;// Clear the data Pins
	IOSET0 = cmd; // Assign command Byte to Data Pins
	IOCLR0 = RS; // Select command Register
	IOSET0 = E;
	delay(2);
	IOCLR0 = E;
}

void LCD_DATA(unsigned char d)
{
	IOCLR0 = LCD_D;// Clear the data Pins
	IOSET0 = d; // Assign Data Byte to Data Pins
	IOSET0 = RS; 
	IOSET0 = E;
	delay(2);
	IOCLR0 = E;
}

void LCD_INIT()
{
	IODIR0 = LCD_D|RS|E; //P0.0 to P0.9 as 	OUTPUT Pins
	LCD_COMMAND(0x01); // To clear the display
	LCD_COMMAND(0x02); //Return Cursor to home position
	LCD_COMMAND(0x0c); // Display ON Cursor OFF
	LCD_COMMAND(0x38); // 8 Bit Interface Mode with Both Lines
	LCD_COMMAND(0x80); // Select Starting address of DRAM 
}

void LCD_STR(unsigned char* s)
{
	while(*s)
		LCD_DATA(*s++);
}
	
int main()
{
	LCD_INIT();
	while(1)
	{
		LCD_COMMAND(0x80);
		LCD_STR("Hello");
	}
}

