#include <Arduino.h>
#include <stdio.h>

#include <DueTimer.h>
#include "SPI.h"
#include "CSV_Parser.h"
#include "SD.h"

#define port_PWM_H_IN1 4
#define port_PWM_H_IN2 5
#define port_ENCODER_IN1 3
#define port_ENCODER_IN2 7
#define port_ENABLE 2
#define port_DEBUG_LED LED_BUILTIN

#define port_PWM_OUT1 35 // PC3
#define port_PWM_OUT2 37 // PC5

#define CPR 48
#define FREQ 20000

/* Controller constants */
#define PERIOD 10000
#define REDUCTORA 75


/* ISR declaration for timers and GPIO interruptions */
void ENCODER1_ISR();
void ENCODER2_ISR();
void serialActivate();
void sampleData();
void restartExecution();
void activateController();

/* From pulses to rad */
float pulsesToRad(int pulses);

/* Enumeration describing all possible rotation movements */
enum direccion{
    HORARIO = 0,
    ANTI_HORARIO = 1,
    WRONG = 2
};

typedef struct state_s state_t;

/*  setPWM
 *  Declaracion de funcion que maneja los inputs del puente en H dada una dirección, un duty cycle y una frecuencia del dutycycle
 *  Parametros
 *      - voltage -> float (V) Entre -12 y 12. Signo indica sentido de dirección.
 *      - freq -> int (Hz)
*/
int setPWM(float volt, int freq);

/*  configurePWM
 *  Configuracion de los registros para usar PWM desde ATMEL SAM3X
 *  Parametros
 *      - voltage -> float (V) Entre -12 y 12. Signo indica sentido de dirección.
 *      - freq -> int (Hz)
*/
int configurePWM(float volt, int freq);


/*  readPos
 *  Translates pulse counts to actual position in radians
 *  Parameters
 *      - pulses -> int
 *  Returns
 *      - radians -> float
*/
float readPos(int pulses);

/*  proportionalController
 *  Calls controller to set voltage
 *  Parameters
 *      - finalRad -> float 
 *      - actualRad -> float
 *      - Kp -> float
*/
float proportionalController(float finalRad, float actualRad, float Kp);

/*  proportionalDerivativeController
 *  Calls controller to set voltage
 *  Parameters
 *      - finalRad -> float 
 *      - actualRad -> float
 *      - lastRad -> float valor en radianes en la última llamada
 *      - Kp -> float
 *      - tauD -> float
 *      - period -> int
*/
float proportionalDerivativeController(float finalRad, float actualRad, float lastRad, float Kp, float tauD, float period);

/*  proportionalIntegralController
 *  Calls controller to set voltage
 *  Parameters
 *      - finalRad -> float 
 *      - actualRad -> float
 *      - lastErrorArray[] -> float[] valor de los errores (usado para la integral)
 *      - sizeError -> int tamaño de lastErrorArray
 *      - Kp -> float
 *      - tauI -> float
 *      - period -> int
*/
float proportionalIntegralController(float finalRad, float actualRad, float lastErrorArray[], int sizeError, float Kp, float tauI, float period);

/*  proportionalIntegralController
 *  Calls controller to set voltage
 *  Parameters
 *      - finalRad -> float 
 *      - actualRad -> float
 *      - lastRad -> float valor en radianes en la última llamada
 *      - lastErrorArray[] -> float[] valor de los errores (usado para la integral)
 *      - sizeError -> int tamaño de lastErrorArray
 *      - Kp -> float
 *      - tauI -> float
 *      - tauD -> float
 *      - period -> int
*/
float proportionalIntegralDerivativeController(float finalRad, float actualRad, float lastRad, float lastErrorArray[], int sizeError, float Kp, float tauI, float tauD, float period);