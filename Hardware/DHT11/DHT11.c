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
		DHT11_Clr(); //�����������ߣ�����18ms
		delay_ms(20);
		DHT11_Set(); //������������ 20-40us
		delay_us(30);
}

char DHT11_Check()
{
	unsigned int t = 0;
	while(DHT11_Read())//�ȴ��͵�ƽ����
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
	while((!(DHT11_Read())))//�жϵ͵�ƽ�Ϸ��� �͵�ƽʱ��80us
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
	while(DHT11_Read())//�жϸߵ�ƽ�Ϸ��� �ߵ�ƽʱ��80us
	{
		t++;
		delay_us(1);
		if(t > 100)
		{
			printf("test3\r\n");
			return 3;
		}
	}

	return 0;//���DHT11���� ����0 �쳣����С��0
}

unsigned char DHT11_Read_Byte(void)
{
	unsigned char i;
	unsigned char t = 0;	//��ʱ��־λ
	unsigned char dat = 0;
	for(i = 0;i < 8;i++)//��λ�ȳ� ��λ���
	{
		while((!(DHT11_Read())) && (t < 1000))//�ȴ�50us�͵�ƽ��ȥ ��ʱ����
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
			while((DHT11_Read()) && t<1000)//�ȴ�50us�ߵ�ƽ��ȥ ��ʱ����
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
		buf[0] = DHT11_Read_Byte();//����ʪ�ȸ߰�λ
		buf[1] = DHT11_Read_Byte();//����ʪ�ȵڰ�λ
		buf[2] = DHT11_Read_Byte();//�����¶ȸ߰�λ
		buf[3] = DHT11_Read_Byte();//�����¶ȵڰ�λ
		buf[4] = DHT11_Read_Byte();//����У��λ
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


