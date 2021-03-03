#include "main.h"

static int counterEncoder1;
static int counterEncoder2;
static int lastCounterEncoder1;
static int lastCounterEncoder2;
static int rotation_direction;

static unsigned long lastTime;

static int show_Serial;

void setup() {
  // init counters
  counterEncoder1 = 0;
  counterEncoder2 = 0;
  lastCounterEncoder1 = 0;
  lastCounterEncoder2 = 0;
  
  lastTime = 0;

  show_Serial = 0;

  // Set pinMode
  // pinMode(port_PWM_H_IN1, INPUT);
  // pinMode(port_PWM_H_IN2, OUTPUT);
  pinMode(port_PWM_OUT1, OUTPUT);
  pinMode(port_PWM_OUT2, OUTPUT);
  pinMode(port_ENCODER_IN1, INPUT);
  pinMode(port_ENCODER_IN2, INPUT);
  pinMode(port_ENABLE, OUTPUT);
  pinMode(port_DEBUG_LED, OUTPUT);

  // TODO 
  // Change PWM frequency

  // Maybe is more correct use FALLING instead of RISING
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN1), &ENCODER1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN2), &ENCODER2_ISR, RISING);

  // initialize serial communication
  Serial.begin(9600);

  // Timer creation and configuration
  Timer0.attachInterrupt(&serialActivate).start(1000);

  // Configure PWM
  configurePWM(HORARIO, 50, 20000);

  digitalWrite(port_ENABLE,1);

  Serial.println("Setup completed");

}

void loop() {
  Serial.println("Entering infinite loop");
  while(1){
    if(Serial && show_Serial){
        // Serial.print(F("El sentido de rotación es "));
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
        // float elapsedTime = millis()-lastTime;
        // Serial.print(elapsedTime);
        // Serial.println(" ms");

        // lastCounterEncoder1 = counterEncoder1;
        // lastCounterEncoder2 = counterEncoder2;
        // lastTime = millis();

        // show_Serial = 0;
    }
  }
}

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