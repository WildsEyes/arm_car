

#include "communicationWilds.h"

#include "Arduino.h"

unsigned char COM_rx_buf[COM_RX_LENGTH];
unsigned char COM_rx_buf2[COM_RX_LENGTH]; 	//双缓冲
unsigned char COM_rx_buf_flag=0;				//现在是哪个buf有效<0,1>

unsigned char COM_rx_buf_ana[COM_RX_LENGTH*2];


Data_mem comRegBuf;							//缓冲
Data_mem COM_reg_Data[COM_REG_NUM];			//地址映射
unsigned char reg_have_write[COM_REG_NUM];	//每次收到数据写1,可以被查询到,读取时自动写0



//发送
unsigned char tx_buf[FH_LENGTH];



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


static  int comPointA = 0;//指向帧首，数据刚好开始的那一个
static  int comPointB = 0;//指向帧尾，刚好没有数据的那一个


void COM_anlysis(void)
{
	//1.先将rx_buf/rx_buf2的数据缓冲到rx_buf_ana中
		//更新帧头帧尾
	unsigned char * cPtr;
	if(COM_rx_buf_flag==0)
	{
		cPtr = COM_rx_buf;
	}
	else
	{
		cPtr = COM_rx_buf2;
	}

	if(comPointB>COM_RX_LENGTH) //不能比它大，都在会溢出
	{
		comPointB = 0;//抛弃数据
	}
	if(comPointA>comPointB)		
	{
		comPointA = comPointB;
	}
		
	
	for(int i=0;i<COM_RX_LENGTH;i++)
	{
		COM_rx_buf_ana[comPointB++] = cPtr[i];
		
		//conf
		cPtr[i] = 0;							//清除buf
	}
	// end 1.先将rx_buf/rx_buf2的数据缓冲到rx_buf_ana中
	

	//2.从头到尾开始解析rx_buf_ana
		//解析的数据放到寄存器中
			//帧头帧尾校验正确，否则抛弃
		//更新帧头帧尾

	unsigned char state = 0;
	while(1) //不存在一帧及以上的数据
	{
		while(COM_rx_buf_ana[comPointA] != FH) //等于帧头
		{
			comPointA++;
			if(comPointA==comPointB)	//保证帧头检查不会溢出
			{
				state = 1;  //不再有数据
				break;		
			}
		}//定位到帧头

		if((comPointB-comPointA)<FH_LENGTH) //保证buf地址合法性
		{
			state = 1;	//不再有数据
			break;		
		}         
		if(state == 0)
		{
			unsigned char verify;
			unsigned char addr;						//功能 地址
			addr = COM_rx_buf_ana[++comPointA];
			verify  = addr;
			for(int i=0;i<sizeof(Data_mem);i++)
			{
				comRegBuf.cdata[i] = COM_rx_buf_ana[++comPointA]; //拷贝出数据 //指向最后一个数据
				verify+=comRegBuf.cdata[i];
			}			
			if(verify==COM_rx_buf_ana[++comPointA]) //指向校验
			{
				if(addr<COM_REG_NUM)	//地址合法
				{
					for(int i=0;i<sizeof(Data_mem);i++)
					{					
						COM_reg_Data[addr].cdata[i]=comRegBuf.cdata[i]; //写入内存
					}
					 reg_have_write[addr] = DATA_NEW;	//更新标识          
				}								
			}
			comPointA++;
		}		
	}
	//3.不能解析的包含帧头数据移到rx_buf_ana的首部
	if(comPointA==comPointB)
	{
		comPointA = comPointB = 0;
	}
	for(int i = 0;i<(comPointB-comPointA);i++)
	{
		COM_rx_buf_ana[i] = COM_rx_buf_ana[comPointA+i];
	}
}
 
void COM_SendData32(unsigned long *data,unsigned char *addr)
{		
	comRegBuf.udata32 = *data;
	tx_buf[0] = 0xAA;
	tx_buf[1] = *addr;
	for(int i=0;i<sizeof(Data_mem);i++)
	{
		tx_buf[2+i] = comRegBuf.cdata[i]; //低字节被先发出去
	}
	tx_buf[6] = tx_buf[1] + tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5];
	COM_SendCallback(tx_buf,FH_LENGTH);
}
//__weak void COM_SendCallback(unsigned char *pData,unsigned char length)
//{
	
//}

