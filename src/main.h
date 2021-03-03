#include <Arduino.h>

#include "motorPWM.h"
#include "DueTimer.h"

#define port_PWM_H_IN1 4
#define port_PWM_H_IN2 5
#define port_ENCODER_IN1 3
#define port_ENCODER_IN2 7
#define port_ENABLE 2
#define port_DEBUG_LED LED_BUILTIN

#define port_PWM_OUT1 35 // PC3
#define port_PWM_OUT2 39 // PC7

void ENCODER1_ISR(void);
void ENCODER2_ISR(void);

void serialActivate(void);