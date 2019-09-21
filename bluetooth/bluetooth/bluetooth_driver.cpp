
#include "bluetooth_driver.h"
#include "communicationWilds.h"
#include "Arduino.h"
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
    *dataPtr = bufferPtr->data;
  }
  return 0;
}
unsigned char getLongData_bluetoothDriver(unsigned char addr,long *dataPtr)
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


void sendAData_bluetoothDriver(unsigned char addr,float data)
{
  Data_mem buffer;
  buffer.udata32 = data;
  COM_SendData32(&buffer.udata32,&addr);
}

void listen_bluetoothDriver(void)
{
  if (Serial.available()>=COM_RX_LENGTH) 
  {
    Serial.readBytes(COM_rx_p,COM_RX_LENGTH);   
    COM_anlysis();
  }
    
}


void COM_SendCallback(unsigned char *pData,unsigned char length)
{
  while(length--)
    {
      unsigned char buf;
      buf = *pData++;
      Serial.write(buf);
    }
}
