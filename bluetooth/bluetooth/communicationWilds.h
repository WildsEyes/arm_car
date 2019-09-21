#ifndef _COMMUN_Wilds_H
#define _COMMUN_Wilds_H

#ifdef __cplusplus
extern "C"{
#endif

typedef union
{
	float data;
	unsigned char cdata[4];
	unsigned long udata32;
}Data_mem;

#define COM_RX_LENGTH 8
#define FH 						0xAA

#define COM_REG_NUM 20

#define RX_BUF_LENGTH COM_RX_LENGTH*2


extern unsigned char COM_rx_p[COM_RX_LENGTH];	
//extern Data_mem COM_reg_Data[COM_REG_NUM];//地址映射
	
void COM_anlysis(void);
unsigned char COM_haveNewData(unsigned char *addr);						//查询指定地址是否更新了数据
Data_mem *COM_readReg(unsigned char *addr);										//从内存中指定位置读取一个数据
void COM_SendData32(unsigned long *data,unsigned char *addr);


void writeRegWilds(void);

void COM_SendCallback(unsigned char *pData,unsigned char length);
	
#ifdef __cplusplus
}
#endif

#endif

