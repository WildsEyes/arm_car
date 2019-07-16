
#ifndef _ANGLE_MOTOR_H_
#define _ANGLE_MOTOR_H_


#define Fs 50       //控制执行频率

long long encoderCount = 0;
float realAngle;
float setAngle;
float K_angle = 360.0 / 692.0; //每转一圈大约是692个脉冲

/******对外函数begin**************/
void initAngleMotor();

void setAngleInc(float arg);

float getAngleInc();
/******对外函数end**************/

//控制运行主函数
void MotorRun();

//位置闭环PID
//抗饱和积分
//低误差停止调节
float pid(float arg, float Ts); //输入：位置差   输出：PWM

//设置电机输出电压
void setPWM(float arg);

//编码器采集
void encoderCallback();

#endif