#include <Arduino.h>

#include <DueTimer.h>
#include <CSV_Parser.h>
#include <SD.h>

#define port_PWM_H_IN1 4
#define port_PWM_H_IN2 5
#define port_ENCODER_IN1 3
#define port_ENCODER_IN2 7
#define port_ENABLE 2
#define port_DEBUG_LED LED_BUILTIN

#define port_PWM_OUT1 35 // PC3
#define port_PWM_OUT2 37 // PC5

/* ISR declaration for timers and GPIO interruptions */
void ENCODER1_ISR();
void ENCODER2_ISR();
void serialActivate();
void testISR();

/* Enumeration describing all possible rotation movements */
enum direccion{
    HORARIO = 0,
    ANTI_HORARIO = 1,
    WRONG = 2
};

/* Struct declaration and definition that contains encoder state */
struct state_s
{
    int stateEncoder1 : 1;
    int stateEncoder2 : 1;
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

/*  testInitialization
 *  Takes array of times and voltages and runs first test values
 *  Parameters
 *      - testTimes -> uint32_t*
 *      - testVoltages -> float*
*/
int testInitialization(uint32_t* testTimes, float* testVoltages);

/*  decide_direction
 *  Decides direction based on previous and current encoder state
 *  Returns
 *      - HORARIO = 0
 *      - ANTIHORARIO = 1
 *      - WRONG = 2
*/
int decide_direction();