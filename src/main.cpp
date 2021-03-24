#include "main.h"
#include "CSVPos.h"

/* Encoder variables */
volatile int pulseCounter;
int timerms;

int* pulses;
float lastRad;
float* previousError;
int sizeError;
float actualVoltage;

/* CSV data */
float* csv_positions;
int* csv_controllerType;
float* csv_Kp;
float* csv_TauD;
float* csv_TauI;

int controllerType;
float finalRad;
float csvKp;
float csvTauD;
float csvTauI;
int csvIndex;
int csvSize;

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

void setup() {
    delay(5000);
    int rv;

    /* Init counters */   
    pulseCounter = 0;
    timerms = 0;
    pulses = NULL;

    show_Serial = 0;
    controllerActivation = 0;
    lastRad = 0;
    sizeError = 0;
    previousError = NULL;
    actualVoltage = 0;

    /* CSV init */
    csvIndex = 0;

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

    /* CSV parser for positions */
    CSV_Parser cp(csv_str, "Lffff");
    csvSize = cp.getRowsCount();

    csv_positions = (float*)cp["POSITION"];
    csv_controllerType = (int*)cp["CONTROLLER"];
    csv_Kp = (float*)cp["KP"];
    csv_TauD = (float*)cp["TAUD"];
    csv_TauI = (float*)cp["TAUI"];

    controllerType = csv_controllerType[csvIndex];
    finalRad = csv_positions[csvIndex];
    csvKp = csv_Kp[csvIndex];
    csvTauD = csv_TauD[csvIndex];
    csvTauI = csv_TauI[csvIndex];

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&sampleData).start(1000);
    Timer1.attachInterrupt(&activateController).start(PERIOD);
    Timer2.attachInterrupt(&serialActivate).start(10000);

    /* Configure PWM */
    rv = configurePWM(0, FREQ);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");
    Serial.println("Entering infinite loop");
    Serial.println("TIME,FINALRAD,CONTROLLER,KP,TAUD,TAUI,PULSES");

}

void loop() {
    if(Serial && show_Serial){
        Timer0.stop();
        Timer1.stop();
        Timer2.stop();
        for(int i = 0; i < timerms; i++){
            Serial.print(i);
            Serial.print(", ");
            Serial.print(finalRad);
            Serial.print(", ");
            Serial.print(controllerType);
            Serial.print(", ");
            Serial.print(csvKp);
            Serial.print(", ");
            Serial.print(csvTauD);
            Serial.print(", ");
            Serial.print(csvTauI);
            Serial.print(", ");
            Serial.println(pulses[i]);
        }
       
        show_Serial = 0;
        controllerActivation = 0;
        Timer4.attachInterrupt(&restartExecution).start(2000000);
    }

    if(controllerActivation){
        float actualRad = pulsesToRad(pulseCounter);

        switch (controllerType)
        {
        case PROPORTIONAL:
            actualVoltage = proportionalController(finalRad, actualRad, csvKp);
            break;

        case DERIVATIVE:
            actualVoltage = proportionalDerivativeController(finalRad, actualRad, lastRad, csvKp, csvTauD, PERIOD);
            lastRad = actualRad;
            break;

        case INTEGRAL:
            actualVoltage = proportionalIntegralController(finalRad, actualRad, previousError, sizeError, csvKp, csvTauI, PERIOD);
            previousError = (float*)realloc(previousError, ++sizeError * sizeof(float));
            previousError[sizeError - 1] = finalRad*REDUCTORA - actualRad;
            break;

        case INTEGRALDERIVATIVE:
            actualVoltage = proportionalIntegralDerivativeController(finalRad, actualRad, lastRad, previousError, sizeError, csvKp, csvTauI, csvTauD, PERIOD);
            previousError = (float*)realloc(previousError, ++sizeError * sizeof(float));
            previousError[sizeError - 1] = finalRad*REDUCTORA - actualRad;
            lastRad = actualRad;
            break;
        
        default:
            break;
        }

        controllerActivation = 0;
    }
}

/* Timer routine fo showing serial info periodically */
void
serialActivate(){
    // Iniciar timer3 que activa una variable
    float actualRad = pulsesToRad(pulseCounter);
    if((abs(finalRad*REDUCTORA - actualRad) < 2*PI/CPR) && (abs(actualVoltage) < 0.6)){
        Serial.println("Hola");
        show_Serial = 1;
        // Apagar timer3 para que no se active una variable
    } else {
        show_Serial = 0;
    }
}

/* Timer routine to activate controller */
void
activateController(){
    controllerActivation = 1;
}

/* ISR for storing data that will be shown afterward */
void
sampleData(){
    pulses = (int*)realloc(pulses, ++timerms * sizeof(int));
    pulses[timerms - 1] = pulseCounter;
}

/* ISR for encoder interruptions */
void
ENCODER1_ISR(){
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter--;
    } else{
        pulseCounter++;
    }
}

void
ENCODER2_ISR(){
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter++;
    } else{
        pulseCounter--;
    }
}

/* Function used to give time for serial printing */
void
restartExecution(){
    Timer4.stop();
    memset(pulses, 0, timerms*sizeof(int));
    timerms = 0;
    pulseCounter = 0;

    csvIndex++;
    if(csvIndex == csvSize){
        Serial.println("TEST ENDED");
        while(1){
            // idle
        }
    }

    controllerType = csv_controllerType[csvIndex];
    finalRad = csv_positions[csvIndex];
    csvKp = csv_Kp[csvIndex];
    csvTauD = csv_TauD[csvIndex];
    csvTauI = csv_TauI[csvIndex];    

    Timer1.start(PERIOD);
    Timer2.start(10000);
    Timer0.start(1000);
}

/* From pulses to rad */
float
pulsesToRad(int pulses){
    float res = pulses * 2 * PI /CPR;
    return res;
}