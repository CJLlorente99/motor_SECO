#include <Arduino.h>

/*  Usable pins for interrupts in Arduino Due are all digital pins
    Usable pins for PWM in Arduino Due are 2-13 pins
*/

#define V_MAX 3.3
#define port_PWM_IN1 0
#define port_PWM_IN2 1
#define port_ENCODER_IN1 2
#define port_ENCODER_IN2 3

enum direccion{
    DERECHA,
    IZQUIERDA
};

/*  Declaracion de funcion que maneja los inputs del puente en H dada una dirección y un duty cycle
    Parametros
        - dir -> DERECHA o IZQUIERDA dependiendo del sentido de giro deseado
        - dutycycle -> float entre 0 y 100
*/

int setPWM(direccion dir, float dutycycle);