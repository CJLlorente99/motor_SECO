#include "main.h"
#include <stdio.h>

/* Encoder variables */
volatile int pulseCounter;
int timerms;

int pulses[1251];
float lastRad;
float* previousError;
int sizeError;

/* Flag activated by timer to send serial data */
int show_Serial;

/* Flag activated to activate controller */
int controllerActivation;

enum controllerTypes {
    PROPORTIONAL,
    DERIVATIVE,
    INTEGRAL,
    INTEGRALDERIVATIVE
};

controllerTypes controllerType;

void setup() {
    delay(5000);
    int rv;

    /* Init counters */   
    pulseCounter = 0;
    timerms = 0;

    memset(pulses, 0, 1251*sizeof(int));

    show_Serial = 0;
    controllerActivation = 0;
    controllerType = CONTROLLERTOUSE;
    lastRad = 0;
    sizeError = 0;

    /* Set pinMode */
    // pinMode(port_PWM_H_IN1, INPUT);
    // pinMode(port_PWM_H_IN2, OUTPUT);
    pinMode(port_PWM_OUT1, OUTPUT);
    pinMode(port_PWM_OUT2, OUTPUT);
    pinMode(port_ENCODER_IN1, INPUT);
    pinMode(port_ENCODER_IN2, INPUT);
    pinMode(port_ENABLE, OUTPUT);
    pinMode(port_DEBUG_LED, OUTPUT);

    /* Encoder interrupts */
    attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN1), &ENCODER1_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN2), &ENCODER2_ISR, CHANGE);

    /* Initialize serial communication */
    /* Baudrate  */
    Serial.begin(115200);

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&sampleData).start(1000);
    Timer1.attachInterrupt(&activateController).start(PERIOD);
    Timer2.attachInterrupt(&serialActivate).start(1200000);

    /* Configure PWM */
    rv = configurePWM(0, FREQ);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");
    Serial.println("Entering infinite loop");

}

void loop() {
    if(Serial && show_Serial){
        Timer0.stop();
        Timer1.stop();
        Timer2.stop();
        for(int i = 0; i < 1201; i++){
            Serial.print(i);
            Serial.print(", ");
            Serial.println(pulses[i]);
        }
       
        Timer4.attachInterrupt(&restartExecution).start(500000);

        show_Serial = 0;
    }

    if(controllerActivation){
        float actualRad = pulsesToRad(pulseCounter);

        switch (controllerType)
        {
        case PROPORTIONAL:
            proportionalController(FINALRAD, actualRad, KP);
            break;

        case DERIVATIVE:
            proportionalDerivativeController(FINALRAD, actualRad, lastRad, KP, TAUD, PERIOD);
            lastRad = actualRad;
            break;

        case INTEGRAL:
            proportionalIntegralController(FINALRAD, actualRad, previousError, sizeError, KP, TAUI, PERIOD);
            previousError = (float*)realloc(previousError, ++sizeError * sizeof(float));
            previousError[sizeError - 1] = FINALRAD - actualRad;
            break;

        case INTEGRALDERIVATIVE:
            proportionalIntegralDerivativeController(FINALRAD, actualRad, lastRad, previousError, sizeError, KP, TAUI, TAUD, PERIOD);
            previousError = (float*)realloc(previousError, ++sizeError * sizeof(float));
            previousError[sizeError - 1] = FINALRAD - actualRad;
            lastRad = actualRad;
            break;
        
        default:
            break;
        }
    }
}

/* Timer routine fo showing serial info periodically */
void
serialActivate(){
    show_Serial = 1;
}

/* Timer routine to activate controller */
void
activateController(){
    controllerActivation = 1;
}

/* */
void
sampleData(){
    pulses[++timerms] = pulseCounter;
}

/* ISR for encoder interruptions */
void
ENCODER1_ISR(){
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter++;
    } else{
        pulseCounter--;
    }
}

void
ENCODER2_ISR(){
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter--;
    } else{
        pulseCounter++;
    }
}

void
restartExecution(){
    memset(pulses, 0, 1251 * sizeof(int));

    timerms = 0;
    pulseCounter = 0;

    Timer0.start(1000);
    Timer1.start(10000);
    Timer2.start(1200000);
    Timer4.stop();
}

/* From pulses to rad */
float
pulsesToRad(int pulses){
    float res = pulses/CPR * 2 * PI;
    return res;
}