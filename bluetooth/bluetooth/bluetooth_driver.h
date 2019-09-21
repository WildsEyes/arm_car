#ifndef __BLUETOOTH_D_H
#define __BLUETOOTH_D_H

#ifdef __cplusplus
extern "C"{
#endif

void init_bluetoothDriver(void);
unsigned char getAData_bluetoothDriver(unsigned char addr,float *dataPtr);
unsigned char getLongData_bluetoothDriver(unsigned char addr,long *dataPtr);
void sendAData_bluetoothDriver(unsigned char addr,float data);
void listen_bluetoothDriver(void);
#ifdef __cplusplus
}
#endif

#endif
