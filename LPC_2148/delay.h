void pll_init(void)
{
	PLL0CON = 0x01;//Enable
	PLL0CFG = 0x24;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while((PLL0STAT&(0x01<<10))==0);
	PLL0CON = 0x03;//Enable and Connect
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	VPBDIV = 0x01;//CCLK = PCLK = 60MHz
}

void delay_seconds(unsigned int s)
{
	T0PR = 15000000-1; 
	T0TCR = 0x01;
	while(T0TC < s);
	T0TCR = 0x03;
	T0TCR = 0x00;	
}

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