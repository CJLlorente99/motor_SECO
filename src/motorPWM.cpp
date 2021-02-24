#include "motorPWM.h"

int setPWM(direccion dir, float dutycycle){
    int dutycycle_int = dutycycle * 100;
    long pwm_level = map(dutycycle_int, 0, 10000, 0 , 255);
    
    if(dir == DERECHA){
        analogWrite(port_PWM_IN1, (uint32_t) pwm_level);
    } else if(dir == IZQUIERDA){
        analogWrite(port_PWM_IN2, (uint32_t) pwm_level);
    }

    return 0;
}