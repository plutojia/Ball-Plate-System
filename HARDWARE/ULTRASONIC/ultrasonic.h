#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H	

#include "sys.h"

#define TRIG_Send  PBout(3) 
#define ECHO_Reci  PBin(5)

void Hcsr04Init(void);
void hcsr04_NVIC(void);
u32 GetEchoTimer(void);
float Hcsr04GetLength(void );
#endif
