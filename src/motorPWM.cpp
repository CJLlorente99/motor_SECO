#include "motorPWM.h"

int setPWM(direccion dir, float dutycycle){
    int dutycycle_int = (int)(dutycycle * 100);
    long pwm_level = (long) map(dutycycle_int, 0, 10000, 0 , 255);
    
    if(dir == HORARIO){
        analogWrite(port_PWM_OUT1, (uint32_t) pwm_level);
        analogWrite(port_PWM_OUT2, (uint32_t) 0);
    } else if(dir == ANTI_HORARIO){
        analogWrite(port_PWM_OUT2, (uint32_t) pwm_level);
        analogWrite(port_PWM_OUT1, (uint32_t) 0);
    }

    return 0;
}