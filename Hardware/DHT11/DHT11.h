#ifndef __DTH11_H__
#define __DTH11_H__

void DHT11_Rst(void);
char DHT11_Check(void);
unsigned char DHT11_Read_Byte(void);
int DHT11_Read_Data(unsigned char *tempH,unsigned char *tempL,unsigned char *humiH,unsigned char *humiL);

#endif
