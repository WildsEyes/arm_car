#ifndef ARM_CTL_H
#define ARM_CTL_H
#define limitRange2(y,x1,x2) y>x2?(y=x2):(y<x1?(y=x1):(y=y))

void arm_clamp(float arg);            //夹子控制 (角度)
void arm_ctl_loop(float position[3]); //运动位置控制 （mm）

#endif
