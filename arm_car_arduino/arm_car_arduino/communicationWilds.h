#ifndef _COMMUN_Wilds_H
#define _COMMUN_Wilds_H

#ifdef __cplusplus
extern "C"{
#endif

typedef union
{
	float fdata;
	unsigned char cdata[4];
	unsigned long udata32;
}Data_mem;

#define COM_RX_LENGTH 			20
#define COM_REG_NUM 			8

/*
*帧格式：0xAA 长度(整个帧得长度) 功能 数据 校验和{长度+功能+第一个数据} (最长44个字节)
*		2019/9/22 现在定长为一个float数据长度      	 : 0xAA 功能 数据 校验 1+1+4+1 = 7
*/
#define FH 						0xAA									//帧头
#define FH_LENGTH				7
//记得统计内存开销

#define DATA_NEW 1
#define DATA_OLD 1


extern unsigned char COM_rx_buf[COM_RX_LENGTH];	
extern unsigned char COM_rx_buf2[COM_RX_LENGTH];

//接收部分
unsigned char COM_haveNewData(unsigned char *addr);						//查询指定地址是否更新了数据
Data_mem *COM_readReg(unsigned char *addr);								//从内存中指定位置读取一个数据
void COM_anlysis(void);
  //需要增加中断和超时



//发送部分
void COM_SendData32(unsigned long *data,unsigned char *addr);			
void COM_SendCallback(unsigned char *pData,unsigned char length);		//根据硬件来重写这个函数
	
#ifdef __cplusplus
}
#endif

#endif

