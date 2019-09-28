#include "key.h"
#include "Arduino.h"

#define C1 7    //列地址  引脚序号
#define C2 6
#define C3 5
#define C4 4
#define R1 12  //行地址
#define R2 13

void enableC(unsigned char arg);

unsigned char getKeyValue()
{
  unsigned char R_num = 0;
  unsigned char C_num = 0;
  //查询行
pinMode(R1,INPUT_PULLUP);
pinMode(R2,INPUT_PULLUP);

pinMode(C1,OUTPUT);
pinMode(C2,OUTPUT);
pinMode(C3,OUTPUT);
pinMode(C4,OUTPUT);


digitalWrite(C1,LOW);
digitalWrite(C2,LOW);
digitalWrite(C3,LOW);
digitalWrite(C4,LOW);


R_num = digitalRead(R2);
R_num = R_num << 1; 
R_num += digitalRead(R1);


switch(R_num)
{
  case 0:
        R_num = 1;
  break;
  case 1:
        R_num = 2;
  break;
  case 2:
        R_num = 1;
  break;
  case 3:
    return 0; //没有按键按下
 }
  
for(int i = 0;i<4;i++)
{
  enableC(i);
      
  if( (digitalRead(R2)+digitalRead(R1)) != 2)
  {
    C_num = i+1;
    break;
  }
}

return (R_num - 1)*4 + C_num;

  //查询列
}

void enableC(unsigned char arg)
{
  pinMode(C1,INPUT);
  pinMode(C2,INPUT);
  pinMode(C3,INPUT);
  pinMode(C4,INPUT);

  switch(arg)
  {
      case 0:
            pinMode(C1,OUTPUT);
      break;
      case 1:
            pinMode(C2,OUTPUT);
      break;
      case 2:
            pinMode(C3,OUTPUT);
      break;
      case 3:
            pinMode(C4,OUTPUT);
      break;
  }  
}

