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