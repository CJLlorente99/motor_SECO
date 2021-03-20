#include "main.h"
#include "testCSV.h"

/* Controller parameters */
float finalRad;
float Kp;

/* Test variables */
int testCounter;
uint32_t* testTimes;
float* testVoltages;
int testSize;

/* Encoder variables */
volatile int pulseCounter;
int timerms;
static volatile state_t currentState;
static volatile state_t previousState;

int pulses[1251];
int infoState[4][1251];

/* Flag activated by timer to send serial data */
int show_Serial;

/* Flag activated to call the controller */
int controllerCalling;

void setup() {
    delay(5000);
    int rv;

    /* Set final radians */
    finalRad = PI;
    Kp = 0.5;

    /* Init counters */   
    pulseCounter = 0;
    testCounter = 0;
    timerms = 0;

    currentState.stateEncoder1 = 0;
    currentState.stateEncoder2 = 0;
    previousState.stateEncoder1 = 0;
    previousState.stateEncoder2 = 0;

    memset(pulses, 0, 1251*sizeof(int));

    show_Serial = 0;
    controllerCalling = 0;

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

    /* Analyze intial state */
    currentState.stateEncoder1 = digitalRead(port_ENCODER_IN1);
    currentState.stateEncoder2 = digitalRead(port_ENCODER_IN2);

    /* Initialize serial communication */
    /* Baudrate  */
    Serial.begin(115200);

    /* Init CSV parser */
    CSV_Parser cp(csv_str, /* format */ "Lf"); // TIME, VOLTAGE // L = uint_32, f = float
    testTimes = (uint32_t*)cp["TIME"]; 
    testVoltages = (float*)cp["VOLTAGE"];
    testSize = cp.getRowsCount();

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&sampleData).start(1000);
    Timer1.attachInterrupt(&stopISR).start(600000);
    Timer2.attachInterrupt(&serialActivate).start(1200000);

    /* Configure PWM */
    rv = configurePWM(0, 20000);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");
    Serial.println("Entering infinite loop");
    Serial.println("VOLTAGE,INDEX,PULSES");

    /* Controller initialization */
    Timer3.attachInterrupt(&callController).start(5000);

    /* Test initialization */
    setPWM(testVoltages[testCounter++], 20000);
}

void loop() {
    if(Serial && show_Serial){
        Timer0.stop();
        Timer2.stop();
        for(int i = 0; i < 1201; i++){
            Serial.print(testVoltages[testCounter - 1]);
            Serial.print(", ");
            Serial.print(i);
            Serial.print(", ");
            Serial.println(pulses[i]);
        }
        if(testCounter >= testSize){
            Timer1.stop();
            Serial.println("TEST ENDED");
            digitalWrite(port_DEBUG_LED, HIGH);
            while(1);
        }
        Timer4.attachInterrupt(&restartExecution).start(500000);

        show_Serial = 0;
    }

    if(controllerCalling){
        proportionalController(finalRad, readPos(pulseCounter), Kp);
        controllerCalling = 0;
    }
}

/* Timer routine fo showing serial info periodically */
void
serialActivate(){
    show_Serial = 1;
}

void
callController(){
    controllerCalling = 1;
}

/* */
void
sampleData(){
    pulses[++timerms] = pulseCounter;
}

/* ISR for encoder interruptions */
void
ENCODER1_ISR(){
    // previousState.stateEncoder1 = !digitalRead(port_ENCODER_IN1);
    // currentState.stateEncoder1 = digitalRead(port_ENCODER_IN1);
    // previousState.stateEncoder2 = !digitalRead(port_ENCODER_IN2);
    // currentState.stateEncoder2 = digitalRead(port_ENCODER_IN2);
    // pulseCounter++;
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter++;
    } else{
        pulseCounter--;
    }
}

void
ENCODER2_ISR(){
    // previousState.stateEncoder1 = !digitalRead(port_ENCODER_IN1);
    // currentState.stateEncoder1 = digitalRead(port_ENCODER_IN1);
    // previousState.stateEncoder2 = !digitalRead(port_ENCODER_IN2);
    // currentState.stateEncoder2 = digitalRead(port_ENCODER_IN2);
    // pulseCounter++;
    if(digitalRead(port_ENCODER_IN1) == digitalRead(port_ENCODER_IN2)){
        pulseCounter--;
    } else{
        pulseCounter++;
    }
}

/* Decide direction base on states and previous states */
int
decide_direction(int currentEncoder1, int currentEncoder2, int previousEncoder1, int previousEncoder2){
    if(currentEncoder1 == 0 && currentEncoder2 == 0){
        if(previousEncoder1 == 0 && previousEncoder2 == 1){
            return HORARIO;
        } else if(previousEncoder1 == 1 && previousEncoder2 == 0){
            return ANTI_HORARIO;
        }
    } else if(currentEncoder1 == 0 && currentEncoder2 == 1){
        if(previousState.stateEncoder1 == 1 && previousState.stateEncoder2 == 1){
            return HORARIO;
        } else if(previousEncoder1 == 0 && previousEncoder2 == 0){
            return ANTI_HORARIO;
        }
    } else if(currentEncoder1 == 1 && currentEncoder2 == 0){
        if(previousState.stateEncoder1 == 0 && previousState.stateEncoder2 == 0){
            return HORARIO;
        } else if(previousEncoder1 == 1 && previousEncoder2 == 1){
            return ANTI_HORARIO;
        }
    } else if(currentEncoder1 == 1 && currentEncoder2 == 0){
        if(previousState.stateEncoder1 == 1 && previousState.stateEncoder2 == 0){
            return HORARIO;
        } else if(previousEncoder1 == 0 && previousEncoder2 == 1){
            return ANTI_HORARIO;
        }
    } 
    return WRONG;
}

/* ISR to be called when test CSV says voltage should be changed */
void
stopISR(){
    setPWM(0, 20000);    
}

void
restartExecution(){
    memset(pulses, 0, 1251 * sizeof(int));
    memset(infoState[0], 0, 1251 * sizeof(int));
    memset(infoState[1], 0, 1251 * sizeof(int));
    memset(infoState[2], 0, 1251 * sizeof(int));
    memset(infoState[3], 0, 1251 * sizeof(int));

    timerms = 0;
    pulseCounter = 0;

    setPWM(testVoltages[testCounter], 20000);

    Timer0.start(1000);
    Timer1.start(testTimes[testCounter++]);
    Timer2.start(1200000);
    Timer4.stop();
}

float
readPos(int pulses){
    float res = pulses / CPR *2 * PI;
    return res; 
}