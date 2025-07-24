void delay_ms(unsigned int n)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<n);
	T0TCR=0X03;
	T0TCR=0X00;
}

void i2c_Byte_write(u8 slaveaddr,u8 buffaddr,u8 data)
{
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_write(data);
i2c_stop();
delay_ms(10);
}

u8 i2c_Byte_read(u8 slaveaddr,u8 buffaddr)
{	
u8 data;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_restart();
i2c_write((slaveaddr<<1)|1);
data=i2c_nack();
i2c_stop();
return data;
}
 void i2c_pagewrite(u8 slaveaddr,u8 buffaddr,u8 *p,u8 nbyte)
{
u8 i;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
for(i=0;i<nbyte;i++){
i2c_write(p[i]);
 		 }
i2c_stop();
delay_ms(10);
 }
void i2c_seqread(u8 slaveaddr,u8 buffaddr,u8 *p,u8 nbyte)
{
u8 i;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_restart();
i2c_write((slaveaddr<<1)|1);
for(i=0;i<nbyte-1;i++)
{p[i]=i2c_mack();
}
p[i]=i2c_nack();
i2c_stop();

}
