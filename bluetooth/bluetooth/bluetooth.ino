
#include "bluetooth_driver.h"
void setup() {
  // put your setup code here, to run once:
init_bluetoothDriver();
sendAData_bluetoothDriver(1,2);
 pinMode(8,OUTPUT);
 float a = 10;
 Serial.println(a);
}
static long *a;
void loop() {
  // put your main code here, to run repeatedly:


unsigned long b;
  
listen_bluetoothDriver();
sendAData_bluetoothDriver(2,1);
getLongData_bluetoothDriver(2,a);

 b = *a;
 Serial.write(b);

 if(b==1)
 {
  digitalWrite(8, HIGH);
  }
  else
  {
    digitalWrite(8, LOW);
    }
// 
//digitalWrite(8, HIGH);
delay(1000);
//digitalWrite(8, LOW);
//delay(500);
}
