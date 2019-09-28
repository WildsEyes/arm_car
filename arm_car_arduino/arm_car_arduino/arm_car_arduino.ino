#include "bluetooth_driver.h"
#include "arm_ctl.h"
#include "key.h"

#define moveSpeed 10
#define Ts    0.05

long position[4];
float positionf[4];

void setup() {
  // put your setup code here, to run once:

  //初始化串口链路
  init_bluetoothDriver();
  //初始化机械臂电   
//  analogWrite(3,0/(180.0/4*3)*(255-63)+63);  
//  analogWrite(5,200);  
//  analogWrite(6,50);
  
  positionf[0] = 100;
  positionf[1] = 00;
  positionf[2] = 50;
  
}

unsigned char key_value = 0;
void loop() {
  // put your main code here, to run repeatedly:
//1.获取需求的工作位置
//  getLongData_bluetoothDriver(0,&position[0]);
//  getLongData_bluetoothDriver(1,&position[1]);
//  getLongData_bluetoothDriver(2,&position[2]);
//  positionf[0] = position[0];
//  positionf[1] = position[1];
//  positionf[2] = position[2];
//2.发送给机械臂

  key_value = getKeyValue();
  switch(key_value)
  {
    case 0:
      
    break;
    case 1:
    positionf[0]+= moveSpeed * Ts;
    break;
    case 2:
    positionf[1]+= moveSpeed * Ts;
    break;
    case 3:
    positionf[2]+= moveSpeed * Ts;
    break;
    case 4:
    positionf[3]+= moveSpeed * Ts;
    break;
    case 5:
    positionf[0]-= moveSpeed * Ts;
    break;
    case 6:
    positionf[1]-= moveSpeed * Ts;
    break;
    case 7:
    positionf[2]-= moveSpeed * Ts;
    break;
    case 8:
    positionf[3]-= moveSpeed * Ts;
    break;
    }    
    if(key_value!=0)
    {
      Serial.print(key_value);
      Serial.print("   A");
      Serial.print(positionf[0]);
      Serial.print("   B");
      Serial.print(positionf[1]);
      Serial.print("   C");
      Serial.print(positionf[2]);
      Serial.print("   D");
      Serial.print(positionf[3]);
      Serial.print('\n');
    }
   limitRange2(positionf[0],20,150);
   limitRange2(positionf[1],-60,100);
   limitRange2(positionf[2],0,130);
   limitRange2(positionf[3],0,70);
  arm_ctl_loop(positionf);
  arm_clamp(positionf[3]); 
  delay(Ts*1000);
}
