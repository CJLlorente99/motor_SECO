#include "main.h"
#include "testCSV.h"

/* Test variables */
int testCounter;
uint32_t* testTimes;
float* testVoltages;
int testSize;

/* Encoder variables */
volatile int pulseCounter;
int lastPulseCounter;
int timerms;
float voltage;
static volatile state_t currentState;
static volatile state_t previousState;
int dir;

/* Flag activated by timer to send serial data */
int show_Serial;

void setup() {
    delay(5000);
    int rv;

    /* Init counters */
    voltage = 6;
    
    pulseCounter = 0;
    lastPulseCounter = 0;
    testCounter = 0;

    show_Serial = 1;

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
    Serial.begin(9600);

    /* Init CSV parser */
    CSV_Parser cp(csv_str, /* format */ "Lf"); // TIME, VOLTAGE // L = uint_32, f = float
    testTimes = (uint32_t*)cp["TIME"]; 
    testVoltages = (float*)cp["VOLTAGE"];
    testSize = cp.getRowsCount();

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&serialActivate).start(1000);

    /* Configure PWM */
    rv = configurePWM(voltage, 20000);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");

    // Hardly consider placing a sleep in order to be able to start python script. If done so, care about Timer0.
    Serial.println("Entering infinite loop");
    Serial.println("TIME,VOLTAGE,N_ENCODER1,N_ENCODER2");

    Serial.print(timerms);
    Serial.print(", ");
    Serial.print(voltage);
    Serial.print(", ");
    Serial.print(pulseCounter - lastPulseCounter);
    Serial.print(", ");
    Serial.print(dir);
    Serial.print(", ");

    Serial.print(currentState.stateEncoder1);
    Serial.print(", ");
    Serial.print(currentState.stateEncoder2);
    Serial.print(", ");
    Serial.print(previousState.stateEncoder1);
    Serial.print(", ");
    Serial.println(previousState.stateEncoder2);

    /* Test initialization */
    rv = testInitialization(testTimes, testVoltages);
    if(rv != 0){
        Serial.println("ERROR IN TEST INITIALIZATION");
    }
}

void loop() {
    if(Serial && show_Serial){
        dir = decide_direction();

        Serial.print(timerms);
        Serial.print(", ");
        Serial.print(voltage);
        Serial.print(", ");
        Serial.print(pulseCounter - lastPulseCounter);
        Serial.print(", ");
        Serial.print(dir);
        Serial.print(", ");

        Serial.print(currentState.stateEncoder1);
        Serial.print(", ");
        Serial.print(currentState.stateEncoder2);
        Serial.print(", ");
        Serial.print(previousState.stateEncoder1);
        Serial.print(", ");
        Serial.println(previousState.stateEncoder2);

        lastPulseCounter = pulseCounter;
        show_Serial = 0;
    }
}

/* Timer routine fo showing serial info periodically */
void
serialActivate(){
    show_Serial = 1;
    timerms++;
}

/* ISR for encoder interruptions */
void
ENCODER1_ISR(){
    previousState.stateEncoder1 = !digitalRead(port_ENCODER_IN1);
    currentState.stateEncoder1 = digitalRead(port_ENCODER_IN1);
    pulseCounter++;
}

void
ENCODER2_ISR(){
    previousState.stateEncoder2 = !digitalRead(port_ENCODER_IN2);
    currentState.stateEncoder2 = digitalRead(port_ENCODER_IN2);
    pulseCounter++;
}

/* Decide direction base on states and previous states */
int
decide_direction(){
    if(currentState.stateEncoder1 == 0 && currentState.stateEncoder2 == 0){
        if(previousState.stateEncoder1 == 0 && previousState.stateEncoder2 == 1){
            return HORARIO;
        } else if(currentState.stateEncoder1 == 1 && currentState.stateEncoder2 == 0){
            return ANTI_HORARIO;
        }
    } else if(currentState.stateEncoder1 == 0 && currentState.stateEncoder2 == 1){
        if(previousState.stateEncoder1 == 1 && previousState.stateEncoder2 == 1){
            return HORARIO;
        } else if(currentState.stateEncoder1 == 0 && currentState.stateEncoder2 == 0){
            return ANTI_HORARIO;
        }
    } else if(currentState.stateEncoder1 == 1 && currentState.stateEncoder2 == 0){
        if(previousState.stateEncoder1 == 0 && previousState.stateEncoder2 == 0){
            return HORARIO;
        } else if(currentState.stateEncoder1 == 1 && currentState.stateEncoder2 == 1){
            return ANTI_HORARIO;
        }
    } else if(currentState.stateEncoder1 == 1 && currentState.stateEncoder2 == 0){
        if(previousState.stateEncoder1 == 1 && previousState.stateEncoder2 == 0){
            return HORARIO;
        } else if(currentState.stateEncoder1 == 0 && currentState.stateEncoder2 == 1){
            return ANTI_HORARIO;
        }
    } 
    return WRONG;
}

/* Initializate test with CSV values */
int
testInitialization(uint32_t* testTimes, float* testVoltages){
    setPWM(testVoltages[testCounter++], 20000);
    Timer1.attachInterrupt(&testISR).start(testTimes[testCounter]);

    return 0;
}

/* ISR to be called when test CSV says voltage should be changed */
void
testISR(){
    setPWM(testVoltages[testCounter++], 20000);

    if(testCounter + 1 >= testSize){
        Serial.println("END OF TEST");
        Timer1.stop();
        // Timer0.stop();
        return;
    }
    Timer1.start(testTimes[testCounter]);
}