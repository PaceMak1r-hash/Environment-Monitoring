#include "DHT11.h"
#include "oled.h"
#include "delay.h"
#include "stdio.h"

#define DHT11_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)
#define DHT11_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)

char DHT11_Read()
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_SET)
	{
		return 1;
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET)
	{
		return 0;
	}
	
	return 2;
}

void DHT11_Rst(void)
{
		DHT11_Clr(); //主机拉低总线，至少18ms
		delay_ms(20);
		DHT11_Set(); //主机拉高总线 20-40us
		delay_us(30);
}

char DHT11_Check()
{
	unsigned int t = 0;
	while(DHT11_Read())//等待低电平出现
	{
		t++;
		delay_us(1);
		if(t > 1000)
		{
			printf("test1:%d\r\n",DHT11_Read());
			return 1;
		}
	}
	t = 0;
	while((!(DHT11_Read())))//判断低电平合法性 低电平时间80us
	{
		t++;
		delay_us(1);
		if(t > 100)
		{
			printf("test2\r\n");
			return 2;
		}
	}
	t = 0;
	while(DHT11_Read())//判断高电平合法性 高电平时间80us
	{
		t++;
		delay_us(1);
		if(t > 100)
		{
			printf("test3\r\n");
			return 3;
		}
	}

	return 0;//如果DHT11正常 返回0 异常返回小于0
}

unsigned char DHT11_Read_Byte(void)
{
	unsigned char i;
	unsigned char t = 0;	//超时标志位
	unsigned char dat = 0;
	for(i = 0;i < 8;i++)//高位先出 低位后出
	{
		while((!(DHT11_Read())) && (t < 1000))//等待50us低电平过去 超时跳过
		{
			t++;
			delay_us(1);
		}
		t = 0;
		delay_us(40);
		dat <<= 1;
		if(DHT11_Read() == 1)
		{
			dat |= 1;
			while((DHT11_Read()) && t<1000)//等待50us高电平过去 超时跳过
			{
				t++;
				delay_us(1);
			}
		}
		t = 0;
	}
	return dat;
}

int DHT11_Read_Data(unsigned char *tempH,unsigned char *tempL,unsigned char *humiH,unsigned char *humiL)
{
	unsigned char buf[5];
	DHT11_Set();
	DHT11_Rst();
	
	if(DHT11_Check() == 0)
	{
		buf[0] = DHT11_Read_Byte();//接收湿度高八位
		buf[1] = DHT11_Read_Byte();//接收湿度第八位
		buf[2] = DHT11_Read_Byte();//接收温度高八位
		buf[3] = DHT11_Read_Byte();//接收温度第八位
		buf[4] = DHT11_Read_Byte();//接收校验位
		DHT11_Set();
		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			*humiH = buf[0];
			*humiL = buf[1];
			*tempH = buf[2];
			*tempL = buf[3];
		}
		else 
			return -1;
	}
	else 
		return -2;
	return 0;
}


