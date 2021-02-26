#include "motorPWM.h"
#include "DueTimer.h"

void main_loop(void);
void speedUpLeft(void);
void speedUpRight(void);

void ENCODER1_ISR(void);
void ENCODER2_ISR(void);

void serialActivate(void);