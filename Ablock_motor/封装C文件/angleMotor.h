/*
*wqw 2019/6/1
*/


#ifndef _ANGLE_MOTOR_H_
#define _ANGLE_MOTOR_H_

#define Fs 50       //控制执行频率(建议执行频率)
#define M_1 0		//电机1地址
#define M_2 1		//电机2地址
#define POWER_M 0.2  //PWM输出最大功率

void initAngleMotor();	//初始化
void setAngleInc(float arg,unsigned char motor_addr); //设置电机的转动角度
void MotorRun()；		//运行主函数
float getAngleInc(unsigned char motor_addr) //获得电机当前增量角度

#endif