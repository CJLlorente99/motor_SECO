#include "main.h"

static int counterEncoder1;
static int counterEncoder2;
static int lastCounterEncoder1;
static int lastCounterEncoder2;
static int rotation_direction;

static unsigned long lastTime;

static int show_Serial;

void setup() {
    /* Init counters */
    counterEncoder1 = 0;
    counterEncoder2 = 0;
    lastCounterEncoder1 = 0;
    lastCounterEncoder2 = 0;

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

    /* initialize serial communication */
    Serial.begin(9600);

    /* Timer creation and configuration. Timer is  */
    Timer0.attachInterrupt(&serialActivate).start(1000);

    /* Configure PWM */
    configurePWM(HORARIO, 50, 20000);

    /* Set enable always to HIGH */
    digitalWrite(port_ENABLE, HIGH);

    Serial.println("Setup completed");

    // Hardly consider placing a sleep in order to be able to start python script. If done so, care about Timer0.
}

void loop() {
    Serial.println("Entering infinite loop");
    Serial.println("TIME,N_ENCODER1,N_ENCODER2,DIRECTION");
    // setPWM(HORARIO, 50, 20000);
    while(true){
        if(Serial && show_Serial){
            /* Print valuable info */
            // Serial.print("El sentido de rotación es ");
            // if(rotation_direction == 0){
            //     Serial.println("antihorario");
            // } else{
            //     Serial.println("horario");
            // }

            // Serial.print("El numero de pulsos detectado por el encoder1 es ");
            // Serial.println(counterEncoder1 - lastCounterEncoder1);

            // Serial.print("El numero de pulsos detectado por el encoder2 es ");
            // Serial.println(counterEncoder2 - lastCounterEncoder2);

            // Serial.print("Time elapsed ");
            int elapsedTime = millis()-lastTime;
            // Serial.print(elapsedTime);
            // Serial.println(" ms");

            Serial.print(elapsedTime);
            Serial.print(",");
            Serial.print(counterEncoder1 - lastCounterEncoder1);
            Serial.print(",");
            Serial.print(counterEncoder2 - lastCounterEncoder2);
            Serial.print(",");
            Serial.println(rotation_direction);

            lastCounterEncoder1 = counterEncoder1;
            lastCounterEncoder2 = counterEncoder2;
            lastTime = millis();

            show_Serial = 0;
        }
    }
}

// Timer routine fo showing serial info periodically
void
serialActivate(void){
    show_Serial = 1;
}

// ISR for encoder interruptions
void
ENCODER1_ISR(void){
    counterEncoder1 += 1;

    if(counterEncoder1 > counterEncoder2)
        rotation_direction = 0;
}

void
ENCODER2_ISR(void){
    counterEncoder2 += 1;

    if(counterEncoder2 > counterEncoder1)
        rotation_direction = 1;
}