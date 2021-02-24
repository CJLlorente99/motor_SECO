#include "main.h"

static int counterEncoder1;
static int counterEncoder2;
static int lastCounterEncoder1;
static int lastCounterEncoder2;
static int rotation_direction;
static int cambioDeSentido1;
static int cambioDeSentido2;

static volatile unsigned int derecha;
static volatile unsigned int izquierda;

void setup() {
  // init counters
  counterEncoder1 = 0;
  counterEncoder2 = 0;
  lastCounterEncoder1 = 0;
  lastCounterEncoder2 = 0;

  // init flags
  derecha = 1;
  izquierda = 0;

  // Set pinMode
  pinMode(port_PWM_IN1, OUTPUT);
  pinMode(port_PWM_IN2, OUTPUT);
  pinMode(port_ENCODER_IN1, INPUT);
  pinMode(port_ENCODER_IN2, INPUT);

  // TODO 
  // Change PWM frequency
  // Can be hardcoded changing PWM_FREQUENCY constant in variants.h (see Arduino.h, variant.h reference is at the bottom)

  // Create interruptions for encoder
  // Maybe is more correct use FALLING instead of RISING
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN1), ENCODER1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN2), ENCODER2_ISR, RISING);

  // initialize serial communication
  Serial.begin(9600);

  // Timer creation and configuration
  // Periodic information shown throw console
  Timer0.attachInterrupt(showSerialInfo);

  //Start timers
  Timer0.start(500000);

  // Main timer
  Timer1.attachInterrupt(main_loop).start(50);

}

void loop() {
  while(1){
    // hola caracola
  }
}

// Periodic interruption to get info through console
void
showSerialInfo(void){
  Serial.print("El sentido de rotación es");
  Serial.println(rotation_direction);
  Serial.print("El numero de pulsos detectado por el encoder1 es");
  Serial.println(counterEncoder1 - lastCounterEncoder1);
  Serial.print("El numero de pulsos detectado por el encoder2 es");
  Serial.println(counterEncoder2 - lastCounterEncoder2);
  Serial.print("La velocidad es de ");
  float vel = max(counterEncoder2-lastCounterEncoder2, counterEncoder1-lastCounterEncoder1)/(12*60*0.5);
  Serial.print(vel);
  Serial.println(" rpm");

  lastCounterEncoder1 = counterEncoder1;
  lastCounterEncoder2 = counterEncoder2;
}

// Function to be called in order to change rotation direction and angular velocity
void
speedUpRight(void){
  setPWM(DERECHA, 100);
  izquierda = 1;
}

void
speedUpLeft(void){
  setPWM(IZQUIERDA, 100);
  derecha =  1;
}

// Main loop to be executed periodically
void
main_loop(void){
  if(derecha == 1){
      Timer2.attachInterrupt(speedUpRight).start(2000000);
      derecha = 0;
      cambioDeSentido1 = 1;
      cambioDeSentido2 = 1;
    }else if(izquierda == 1){
      Timer2.attachInterrupt(speedUpLeft).start(2000000);
      izquierda = 0;
      cambioDeSentido1 = 1;
      cambioDeSentido2 = 1;
    }
}

// ISR for encoder interruptions
void
ENCODER1_ISR(void){
    counterEncoder1 += 1;
    if(cambioDeSentido1 == 1){
      counterEncoder1 = 0;
      cambioDeSentido1 = 0;
    }
    if(counterEncoder1 > counterEncoder2)
        rotation_direction = 0;
}

void
ENCODER2_ISR(void){
    counterEncoder2 += 1;
    if(cambioDeSentido2 == 1){
      counterEncoder2 = 0;
      cambioDeSentido2 = 0;
    }
    if(counterEncoder2 > counterEncoder1)
        rotation_direction = 1;
}