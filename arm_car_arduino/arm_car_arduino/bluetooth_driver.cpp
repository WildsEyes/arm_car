
#include "bluetooth_driver.h"
#include "communicationWilds.h"
#include "Arduino.h"

/*2019/9/22
 * 这个文件用来适配不同的硬件板子
**/

void init_bluetoothDriver(void)
{
  Serial.begin(9600); //设置串口波特率9600  
}

unsigned char getAData_bluetoothDriver(unsigned char addr,float *dataPtr)
{
  Data_mem *bufferPtr;
  
  if(COM_haveNewData(&addr))
  {
    bufferPtr = COM_readReg(&addr);    
    *dataPtr = bufferPtr->fdata;
    return 1;
  }
  return 0;
}
unsigned char getLongData_bluetoothDriver(unsigned char addr,long *dataPtr) //从内存中拷贝一个指定的数据
{
  Data_mem *bufferPtr;
  
  if(COM_haveNewData(&addr))
  {
    bufferPtr = COM_readReg(&addr);    
    *dataPtr = bufferPtr->udata32;    
    return 1;
  }  
  return 0;
}


void sendAData_bluetoothDriver(unsigned char addr,float data) //发送一个浮点数
{
  Data_mem buffer;
  buffer.fdata = data;
  COM_SendData32(&buffer.udata32,&addr);
}
void sendLongData_bluetoothDriver(unsigned char addr,long data) //发送一个32位整数
{
  Data_mem buffer;
  buffer.udata32 = data;
  COM_SendData32(&buffer.udata32,&addr);
}

//物理层

void listen_bluetoothDriver(void) //轮询查询数据
{

  static int out_count = 0;
   out_count++;
  if (Serial.available()>=COM_RX_LENGTH||out_count>1) 
  {
    out_count = 0;
    Serial.readBytes(COM_rx_buf,COM_RX_LENGTH);   
    COM_anlysis();
  }
    
}


void COM_SendCallback(unsigned char *pData,unsigned char length) //发送数据 物理层
{
  while(length--)
    {
      unsigned char buf;
      buf = *pData++;
      Serial.write(buf);
    }
}
