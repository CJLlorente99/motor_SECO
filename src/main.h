#include <Arduino.h>

#include "DueTimer.h"

#define port_PWM_H_IN1 4
#define port_PWM_H_IN2 5
#define port_ENCODER_IN1 3
#define port_ENCODER_IN2 7
#define port_ENABLE 2
#define port_DEBUG_LED LED_BUILTIN

#define port_PWM_OUT1 35 // PC3
#define port_PWM_OUT2 37 // PC5

void ENCODER1_ISR(void);
void ENCODER2_ISR(void);
void serialActivate(void);

enum direccion{
    HORARIO,
    ANTI_HORARIO
};

/*  setPWM
    Declaracion de funcion que maneja los inputs del puente en H dada una dirección, un duty cycle y una frecuencia del dutycycle
    Parametros
        - dir -> HORARIO o ANTIHORARIO dependiendo del sentido de giro deseado
        - dutycycle -> float entre 0 y 100
        - freq -> int (Hz)
*/
int setPWM(direccion dir, float dutycycle, int freq);

/*  configurePWM
    Configuracion de los registros para usar PWM desde ATMEL SAM3X
    Parametros
        - dir -> HORARIO o ANTIHORARIO dependiendo del sentido de giro deseado
        - dutycycle -> float entre 0 y 100
        - freq -> int (Hz)
*/
int configurePWM(direccion dir, float dutycycle, int freq);