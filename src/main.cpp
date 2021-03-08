#include "main.h"

int counterEncoder1;
int lastCounterEncoder1;
int counterEncoder2;
int lastCounterEncoder2;
int rotation_direction;

unsigned long lastTime;

int show_Serial;

void setup() {
    delay(5000);
    int rv;
    /* Init counters */
    counterEncoder1 = 0;
    lastCounterEncoder1 = 0;
    counterEncoder2 = 0;
    lastCounterEncoder2 = 0;
    rotation_direction = 0; // 0 -> ANTIHORARIO; 1 -> HORARIO

    lastTime = 0;

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

    /* Maybe is more correct use FALLING instead of RISING */
    attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN1), &ENCODER1_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN2), &ENCODER2_ISR, RISING);

    /* Initialize serial communication */
    /* Baudrate  */
    Serial.begin(9600);

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&serialActivate).start(10000);

    /* Configure PWM */
    rv = configurePWM(HORARIO, 20, 20000);
    if(rv != 0){
        Serial.println("ERROR IN CONFIGURE PWM");
    }

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");

    // Hardly consider placing a sleep in order to be able to start python script. If done so, care about Timer0.
    Serial.println("Entering infinite loop");
    Serial.println("TIME,N_ENCODER1,N_ENCODER2,DIRECTION");

}

void loop() {
    if(Serial && show_Serial){

        if(counterEncoder2 == counterEncoder1){
            // do nothing. keep rotation direction as it is
        } else{
            rotation_direction = (counterEncoder2 > counterEncoder1);
        }

        int elapsedTime = millis()-lastTime;

        Serial.print(elapsedTime);
        Serial.print(",");
        Serial.print(counterEncoder1 - lastCounterEncoder1);
        Serial.print(",");
        Serial.print(counterEncoder2 - lastCounterEncoder2);
        Serial.print(",");
        Serial.println(rotation_direction);

        lastTime = millis();
        lastCounterEncoder1 = counterEncoder1;
        lastCounterEncoder2 = counterEncoder2;

        show_Serial = 0;
    }
}

/* Timer routine fo showing serial info periodically */
void
serialActivate(void){
    show_Serial = 1;
}

/* ISR for encoder interruptions */
void
ENCODER1_ISR(void){
    counterEncoder1 += 1;
}

void
ENCODER2_ISR(void){
    counterEncoder2 += 1;
}