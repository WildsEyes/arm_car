
/*
*帧格式：0xAA 长度(整个帧得长度) 功能 数据 校验和{长度+功能+第一个数据} (最长44个字节)
*/

#include "communicationWilds.h"


unsigned char COM_rx_p[COM_RX_LENGTH];
unsigned char rx_buf_loop[RX_BUF_LENGTH+2];
unsigned char tx_buf[8];
Data_mem COM_reg_Data[COM_REG_NUM];//地址映射
unsigned char reg_have_write[COM_REG_NUM];	//每次收到数据写1,

Data_mem comRegBuf;

static unsigned int pointA = 0;
static unsigned int pointB = 0;

unsigned char COM_haveNewData(unsigned char *addr)
{
	if(*addr<COM_REG_NUM)
	{
		return reg_have_write[*addr];
	}
	return 0;
}

Data_mem *COM_readReg(unsigned char *addr)
{
	if(*addr<COM_REG_NUM)
	{
			reg_have_write[*addr] = 0;    //已经读取了，写0
			return &COM_reg_Data[*addr];
	}
	return &COM_reg_Data[0];
}


void writeRegWilds(void)
{
		for(int j = 0; j<4 ;j++)
		{
			if((pointA + 3 + j)<RX_BUF_LENGTH)
			{
				comRegBuf.cdata[j] = rx_buf_loop[pointA + 3 + j];
			}
			else
			{
				comRegBuf.cdata[j] = rx_buf_loop[pointA + 3 + j - RX_BUF_LENGTH];
			}
		}
		if(rx_buf_loop[pointA + 2]<COM_REG_NUM)
		{
			COM_reg_Data[rx_buf_loop[pointA + 2]].udata32 = comRegBuf.udata32;
			reg_have_write[rx_buf_loop[pointA + 2]] = 1;    //写1，表示有有效的数据
		}		
}


 
void COM_anlysis(void)
{	

	for(int i = 0;i<COM_RX_LENGTH;i++)
	{
		if((i+pointB)<RX_BUF_LENGTH)
		{
			rx_buf_loop[i+pointB] = COM_rx_p[i];
		}
		else
		{
			rx_buf_loop[i+pointB-RX_BUF_LENGTH] = COM_rx_p[i];
		}
		
	}	
	//更新结尾帧	
	
	if(pointB<=pointA)
	{
		pointB += COM_RX_LENGTH;
		if(pointB>=pointA)
		{
			pointA = pointB+1;
		}
	}
	if(pointB>pointA)
	{
		pointB += COM_RX_LENGTH;
		if((pointB-pointA)>=RX_BUF_LENGTH)
		{
			pointA = pointB+1;
		}
		
	}
	
	if(pointA>=RX_BUF_LENGTH)
	{
		pointA -= RX_BUF_LENGTH;
	}
	
	if(pointB>=RX_BUF_LENGTH)
	{
		pointB -= RX_BUF_LENGTH;
	}
	
	
	//解包
	//查帧头
	unsigned int point_buf;	
	unsigned int loop_conter = 0;

	for(;;pointA++)
	{
		loop_conter++;
		if(loop_conter>RX_BUF_LENGTH)
		{
			return;
		}
		if(pointA>=RX_BUF_LENGTH)
		{
			pointA -= RX_BUF_LENGTH;
		}
		if(pointA==pointB)
		{
			return;
		}
		point_buf = pointA;
		if(rx_buf_loop[point_buf]==FH)
		{
			//   找到头 ->验证尾
			if(rx_buf_loop[point_buf+1]>44)
			{
				continue;//错误
			}
			
			unsigned int buf_end = rx_buf_loop[point_buf+1]-1+point_buf;
			if(buf_end>=RX_BUF_LENGTH)
			{
				buf_end -= RX_BUF_LENGTH;
			}
			
			if((rx_buf_loop[point_buf+1] + rx_buf_loop[point_buf+2] + rx_buf_loop[point_buf+3]) == rx_buf_loop[buf_end])
			{
				writeRegWilds();
				pointA = buf_end+1;
				if(pointA>=RX_BUF_LENGTH)
				{
					pointA -= RX_BUF_LENGTH;
				}		
				if(pointA==pointB)
				{
					return;
				}
			}
				continue;//会继续查找，知道帧不完整
		}
	}
}

void COM_SendData32(unsigned long *data,unsigned char *addr)
{		
	comRegBuf.udata32 = *data;
	tx_buf[0] = 0xAA;
	tx_buf[1] = 0x08;
	tx_buf[2] = *addr;
	for(int i=0;i<4;i++)
	{
		tx_buf[3+i] = comRegBuf.cdata[i]; //低字节被先发出去
	}
	tx_buf[7] = tx_buf[1] + tx_buf[2] + tx_buf[3];
	COM_SendCallback(tx_buf,tx_buf[1]);
}
//__weak void COM_SendCallback(unsigned char *pData,unsigned char length)
//{
	
//}

