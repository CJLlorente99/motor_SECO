#include "main.h"
#include "CSVPos.h"

/* Encoder variables */
volatile int pulseCounter;
int timerms;

int* pulses;
float* voltages;
float lastRad;
float* previousError;
int sizeError;
volatile float actualVoltage;

/* CSV data */
float* csv_positions;
uint32_t* csv_controllerType;
float* csv_Kp;
float* csv_TauD;
float* csv_TauI;

float* aux_positions;
int* aux_controllerType;
float* aux_Kp;
float* aux_TauD;
float* aux_TauI;

uint32_t controllerType;
float finalRad;
float csvKp;
float csvTauD;
float csvTauI;
int csvIndex;
int csvSize;

/* Flag activated by timer to send serial data */
volatile int show_Serial;

/* Flag activated to activate controller */
volatile int controllerActivation;

/* */
volatile int precautiusTimer;
int starvationCounter;

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
    precautiusTimer = 1;
    starvationCounter = 0;

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
    csv_controllerType = (uint32_t*)cp["CONTROLLER"];
    csv_Kp = (float*)cp["KP"];
    csv_TauD = (float*)cp["TAUD"];
    csv_TauI = (float*)cp["TAUI"];

    aux_positions = (float*)malloc(csvSize*sizeof(float));
    aux_controllerType = (int*)malloc(csvSize*sizeof(float));
    aux_Kp = (float*)malloc(csvSize*sizeof(float));
    aux_TauD = (float*)malloc(csvSize*sizeof(float));
    aux_TauI = (float*)malloc(csvSize*sizeof(float));
    for (int i = 0; i < csvSize; i++){
        aux_positions[i] = csv_positions[i];
        aux_controllerType[i] = csv_controllerType[i];
        aux_Kp[i] = csv_Kp[i];
        aux_TauD[i] = csv_TauD[i];
        aux_TauI[i] = csv_TauI[i];
    }
    
    controllerType = csv_controllerType[csvIndex];
    finalRad = csv_positions[csvIndex];
    csvKp = csv_Kp[csvIndex];
    csvTauD = csv_TauD[csvIndex];
    csvTauI = csv_TauI[csvIndex];

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&sampleData).start(1000);
    Timer1.attachInterrupt(&activateController).start(PERIOD);
    Timer2.attachInterrupt(&serialActivate).start(10000);
    Timer3.attachInterrupt(&precautiusTimerISR);
    Timer4.attachInterrupt(&restartExecution);

    /* Configure PWM */
    rv = configurePWM(0, FREQ);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    // Serial.println("Setup completed");
    // Serial.println("Entering infinite loop");
    Serial.println("TIME,FINALRAD,CONTROLLER,KP,TAUD,TAUI,CONTROLLERVOLTAGE,PULSES");

}

void loop() {
    if(Serial && show_Serial){
        Timer0.stop();
        Timer1.stop();
        Timer2.stop();
        for(int i = 0; i < timerms; i++){
            Serial.print(i);
            Serial.print(",");
            Serial.print(finalRad,4);
            Serial.print(",");
            Serial.print(controllerType);
            Serial.print(",");
            Serial.print(csvKp,4);
            Serial.print(",");
            Serial.print(csvTauD,4);
            Serial.print(",");
            Serial.print(csvTauI,4);
            Serial.print(",");
            Serial.print(voltages[i],4);
            Serial.print(",");
            Serial.println(pulses[i]);
        }
       
        show_Serial = 0;
        controllerActivation = 0;
        Timer4.start(1000000);
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
    float actualRad = pulsesToRad(pulseCounter);
    if(((abs(finalRad*REDUCTORA - actualRad) < 2*PI/CPR) && (abs(actualVoltage) < 0.6) && precautiusTimer) || (starvationCounter > 500)){
        show_Serial = 1;
        Timer3.stop();
        precautiusTimer = 0;
        starvationCounter = 0;
    } else {
        show_Serial = 0;
        starvationCounter++;
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

    voltages = (float*)realloc(voltages, timerms * sizeof(float));
    voltages[timerms - 1] = actualVoltage;
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
    memset(voltages, 0, timerms*sizeof(int));
    timerms = 0;
    pulseCounter = 0;

    csvIndex++;
    if(csvIndex == csvSize){
        Serial.println("TEST ENDED");
        while(1){
            // idle
        }
    }

    Serial.print("NEXT EXPERIMENT");

    controllerType = aux_controllerType[csvIndex];
    finalRad = aux_positions[csvIndex];
    csvKp = aux_Kp[csvIndex];
    csvTauD = aux_TauD[csvIndex];
    csvTauI = aux_TauI[csvIndex];

    Timer0.start(1000);
    Timer1.start(PERIOD);
    Timer2.start(10000);
    Timer3.start(1000000);    
}

/* From pulses to rad */
float
pulsesToRad(int pulses){
    float res = pulses * 2 * PI /CPR;
    return res;
}

/* */
void
precautiusTimerISR(){
    precautiusTimer = 1;
}