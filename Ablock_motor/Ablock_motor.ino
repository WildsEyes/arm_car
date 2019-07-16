
void setup() {
  // put your setup code here, to run once:
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  initAngleMotor();   //-1初始化
}


void loop() {
  // put your main code here, to run repeatedly:

  float angle;

  angle = getAngleSerial(); //串口获取一个控制量，为了测试

  //-1
  setAngleInc(angle,0);   //设置M1的期望角度（增量）
  setAngleInc(angle,1);   //设置M2的期望角度（增量）
  
  //-2
  MotorRun();             //电机运行主函数

  //-3 获取电机当前位置
  Serial.print(getAngleInc(0));  
  Serial.print(' ');
  Serial.print(getAngleInc(1));  

  Serial.print('\n');
  
  delay(20);//50hz 运行周期
}

//串口函数
float getAngleSerial()
{
  float outAngle;
  String comdata = "";
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
  }
  if (comdata.length() != 0)
  {
    if (comdata[0] == 'a')
    {
      outAngle += 60;
    }
    if (comdata[0] == 'b')
    {
      outAngle -= 60;
    }
  }
  return outAngle;
}

/**************************************************下面是封装的代码*****************************************************************************/

#define Fs 50       //控制执行频率
#define M_1 0
#define M_2 1
#define POWER_M 0.2  //PWM输出最大功率


long long encoderCount[2] = {0,0};
float realAngle[2];
float setAngle[2];
float K_angle = 360.0 / 692.0; //每转一圈大约是692个脉冲


/******对外函数begin**************/
void setAngleInc(float arg,unsigned char motor_addr) //设置电机的转动角度，主函数
{
  if(motor_addr<2) //检测地址合法性
  {
  setAngle[motor_addr] += arg;
  }  
}
float getAngleInc(unsigned char motor_addr)
{
  if(motor_addr>1) //检测地址合法性
  {
  return 0;
  }
  return realAngle[motor_addr];
}

void initAngleMotor()
{

//M1
  /*编码器捕获*/  
  attachInterrupt(1, encoderCallback_M1, RISING);
  pinMode(12, INPUT);
  /*编码器捕获end*/

  /*PWM输出*/
  analogWrite(9, 0);
  analogWrite(10, 0);
  /*end PWM输出*/
  
//M2
  /*编码器捕获*/  
  attachInterrupt(0, encoderCallback_M2, RISING);
  pinMode(4, INPUT);
  /*编码器捕获end*/

  /*PWM输出*/
  analogWrite(6, 0);
  analogWrite(5, 0);
  /*end PWM输出*/
}
/******对外函数end**************/

void MotorRun()
{
  float pwm;
  static float time[2];
  float Ts;

  time[0] = time[1];
  time[1] = millis();
  Ts =  (time[1] - time[0]) / 1000.0;
  pwm = pid_M1(setAngle[0] - realAngle[0],Ts);
  setPWM_M1(pwm);
  pwm = pid_M2(setAngle[1] - realAngle[1],Ts);
  setPWM_M2(pwm);
}


float Kp = 0.025;
float Ki = 0.1;
float Kd = 0.0016;
float Imax = 0.2;

//位置闭环PID
//抗饱和积分
//低误差停止调节
float pid_M1(float arg, float Ts) //输入：位置差   输出：PWM
{
  static float I_ = 0;
  static float D_ = 0;
  static float arg_last = 0;
  if (fabs(arg) < 4)
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

float pid_M2(float arg, float Ts) //输入：位置差   输出：PWM
{
  static float I_ = 0;
  static float D_ = 0;
  static float arg_last = 0;
  if (fabs(arg) < 4)
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
void setPWM_M1(float arg)
{
  if (arg > POWER_M)
    arg = POWER_M;
  else if (arg < -POWER_M)
    arg = -POWER_M;

  if (arg > 0)
  {
    analogWrite(9, 0);
    analogWrite(10, arg * 255);
  }
  else
  {
    analogWrite(9, -arg * 255);
    analogWrite(10, 0);
  }
}
void setPWM_M2(float arg)
{
  if (arg > POWER_M)
    arg = POWER_M;
  else if (arg < -POWER_M)
    arg = -POWER_M;

  if (arg > 0)
  {
    analogWrite(6, 0);
    analogWrite(5, arg * 255);
  }
  else
  {
    analogWrite(6, -arg * 255);
    analogWrite(5, 0);
  }
}

//编码器采集
void encoderCallback_M1()
{
  if (digitalRead(12))
  {
    encoderCount[0]--;
  }
  else
  {
    encoderCount[0]++;
  }

  realAngle[0] = encoderCount[0] * K_angle;

}
void encoderCallback_M2()
{
  if (digitalRead(4))
  {
    encoderCount[1]--;
  }
  else
  {
    encoderCount[1]++;
  }

  realAngle[1] = encoderCount[1] * K_angle;

}


