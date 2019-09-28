
#include "arm_ctl.h"
#include "math.h"
#include "Arduino.h"
/*命名和固有参数
 * 单位 mm s °
**********************************************/

#define L 78.6 

#define A_DEF   120   //A点起始角度
#define B_DEF   90   //B点起始角度
#define C_DEF   -90   //A点起始角度

#define servoA(x) analogWrite(3,coverAngle(x))
#define servoB(x) analogWrite(9,coverAngle(x))
#define servoC(x) analogWrite(10,coverAngle(x))
#define servoD(x) analogWrite(11,coverAngle(x))

float A;          //舵机A   基座数 第一个连杆角度
float B;          //舵机B   第二个连杆角度
float C;          //舵机C   转向
float D;          //舵机D   夹具

#define coverAngle(x) (x/180.0*2+0.5)/2*255


/* position[0] 半径
 * position[1] 高度
 * position[2] 旋转
*/
void arm_ctl_loop(float position[3])
{
    float helfDistance;
    float bufa;
    float bufa2;
    
    helfDistance =sqrt(position[0]*position[0] + position[1]*position[1])/2.0f;
    bufa = asin(-position[1]/(helfDistance * 2))/3.14*180; //考虑第二个角度的符号要反向
    bufa2 = 90 - asin(helfDistance/L)/3.14*180;
    B = 180 - (bufa2 + bufa) - B_DEF;
    A = -(bufa2 - bufa - A_DEF);
    
    //C = atan(position[1]/position[0])/3.14*180 - C_DEF;    
    C = position[2];
    limitRange2(A,0,135);
    limitRange2(B,0,135);
    limitRange2(C,0,135);
    servoA(A);
    servoB(B);
    servoC(C); 
  
}

void arm_clamp(float arg)
{
  D = arg;
  limitRange2(D,0,90);
   servoD(D); 
  }

