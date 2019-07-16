
#include "angleMotor.h"

/******对外函数begin**************/
void setAngleInc(float arg)
{
  setAngle += arg;
  MotorRun();
}
float getAngleInc()
{
  return realAngle;
}

void initAngleMotor()
{

  /*编码器捕获*/
  attachInterrupt(0, encoderCallback, RISING);
  pinMode(4, INPUT);
  /*编码器捕获end*/

  /*PWM输出*/
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  /*end PWM输出*/
}
/******对外函数end**************/

void MotorRun()
{
  float pwm;
  static float time[2];
  
  time[0] = time[1];
  time[1] = millis();
  
  pwm = pid(setAngle - realAngle, (time[1] - time[0])/1000.0);
  setPWM(pwm);
}


float Kp = 0.03;
float Ki = 0.02;
float Kd = 0.001;
float Imax = 0.2;

//位置闭环PID
//抗饱和积分
//低误差停止调节
float pid(float arg, float Ts) //输入：位置差   输出：PWM
{
  static float I_ = 0;
  static float D_ = 0;
  static float arg_last = 0;
  if(fabs(arg) < 4)
  {
    return 0;
   }
  
  I_ += arg * Ts; 
  if (I_ > Imax)
    I_ = Imax;
  if (I_ < -Imax)
    I_ = -Imax;

  D_ = (arg - arg_last) / Ts;
  arg_last = arg;
  
  return arg * Kp + I_ * Ki + D_ * Kd;
}

//设置电机输出电压
void setPWM(float arg)
{
  if (arg > 1)
    arg = 1;
  else if (arg < -1)
    arg = -1;

  if (arg > 0)
  {
    OCR2A = arg * 255;
    OCR2B = 0;
  }
  else
  {
    OCR2A = 0;
    OCR2B = -arg * 255;
  }
}

//编码器采集
void encoderCallback()
{
  if (digitalRead(4))
  {
    encoderCount--;
  }
  else
  {
    encoderCount++;
  }

  realAngle = encoderCount * K_angle;

}
