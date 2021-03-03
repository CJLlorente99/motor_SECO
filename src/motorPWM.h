#include <Arduino.h>

/*  Usable pins for interrupts in Arduino Due are all digital pins
    Usable pins for PWM in Arduino Due are 2-13 pins
*/

#define port_PWM_H_IN1 4
#define port_PWM_H_IN2 5
#define port_ENCODER_IN1 3
#define port_ENCODER_IN2 7
#define port_ENABLE 2
#define port_DEBUG_LED 13

#define port_PWM_OUT1 35 // PC3
#define port_PWM_OUT2 39 // PC7

enum direccion{
    HORARIO,
    ANTI_HORARIO
};

/*  Declaracion de funcion que maneja los inputs del puente en H dada una dirección y un duty cycle
    Parametros
        - dir -> DERECHA o IZQUIERDA dependiendo del sentido de giro deseado
        - dutycycle -> float entre 0 y 100
*/

int setPWM(direccion dir, float dutycycle);
int configurePWM(direccion dir, float dutycycle, int freq);