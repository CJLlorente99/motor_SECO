#include "main.h"

static int counterEncoder1;
static int counterEncoder2;
static int lastCounterEncoder1;
static int lastCounterEncoder2;
static int rotation_direction;
static int cambioDeSentido1;
static int cambioDeSentido2;

static unsigned long lastTime;

static volatile unsigned int derecha;
static volatile unsigned int izquierda;

static int show_Serial;

void setup() {
  // init counters
  counterEncoder1 = 0;
  counterEncoder2 = 0;
  lastCounterEncoder1 = 0;
  lastCounterEncoder2 = 0;
  
  lastTime = 0;

  show_Serial = 0;

  // init flags
  derecha = 1;
  izquierda = 0;

  // Set pinMode
//   pinMode(port_PWM_H_IN1, INPUT);
//   pinMode(port_PWM_H_IN2, INPUT);
  pinMode(port_PWM_OUT1, OUTPUT);
  pinMode(port_PWM_OUT2, OUTPUT);
  pinMode(port_ENCODER_IN1, INPUT);
  pinMode(port_ENCODER_IN2, INPUT);
  pinMode(port_ENABLE, OUTPUT);
  pinMode(port_DEBUG_LED, OUTPUT);

  // TODO 
  // Change PWM frequency
  // Can be hardcoded changing PWM_FREQUENCY constant in variants.h (see Arduino.h, variant.h reference is at the bottom)
  uint32_t* PWM_CPRD = (uint32_t*) 0x400942EC;
  uint32_t value = 0;
  value = PWM_CPRD_CPRD(525);
  memset(PWM_CPRD, 0, sizeof(uint32_t));
  memcpy(PWM_CPRD, &value, sizeof(uint32_t));

//   uint32_t* PWM_MCLK = (uint32_t*) 0x400942E0;
//   uint32_t value = PWM_CMR_CPRE_MCK_DIV_8;
//   memset(PWM_MCLK, 0, sizeof(uint32_t));
//   memcpy(PWM_MCLK, &value, sizeof(uint32_t));

  uint32_t* PWM_DIV = (uint32_t*) 0x40094000;
  value = 0;
  value |= PWM_CLK_DIVA(1);
  value |= PWM_CLK_DIVB(1);
  value |= PWM_CLK_PREA(8);
  value |= PWM_CLK_PREB(8);
  memset(PWM_DIV, 0, sizeof(uint32_t));
  memcpy(PWM_DIV, &value, sizeof(uint32_t));


  // Maybe is more correct use FALLING instead of RISING
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN1), &ENCODER1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(port_ENCODER_IN2), &ENCODER2_ISR, RISING);

  // initialize serial communication
  Serial.begin(9600);

  // Timer creation and configuration
  Timer0.attachInterrupt(&serialActivate).start(1000000);
  // Main timer
  // Timer1.attachInterrupt(&main_loop).start(50);

  digitalWrite(port_ENABLE,1);

}

void loop() {
  while(1){
      setPWM(ANTI_HORARIO,100);
    // hola caracola
    if(Serial && show_Serial){
        Serial.print(F("El sentido de rotación es "));
        if(rotation_direction == 0){
            Serial.println("horario");
        } else{
            Serial.println("antihorario");
        }
        Serial.print("El numero de pulsos detectado por el encoder1 es ");
        Serial.println(counterEncoder1 - lastCounterEncoder1);
        Serial.print("El numero de pulsos detectado por el encoder2 es ");
        Serial.println(counterEncoder2 - lastCounterEncoder2);
        Serial.print("La velocidad es de ");
        float vel = max(counterEncoder2-lastCounterEncoder2, counterEncoder1-lastCounterEncoder1)/(12*0.001*(millis()-lastTime));
        Serial.print(vel);
        Serial.println(" rps");
        lastCounterEncoder1 = counterEncoder1;
        lastCounterEncoder2 = counterEncoder2;

        lastTime = millis();

        show_Serial = 0;
    }
  }
}

void
serialActivate(void){
    show_Serial = 1;
}

// Function to be called in order to change rotation direction and angular velocity
void
speedUpRight(void){
  setPWM(HORARIO, 100);
  izquierda = 1;
}

void
speedUpLeft(void){
  setPWM(ANTI_HORARIO, 100);
  derecha =  1;
}

// Main loop to be executed periodically
void
main_loop(void){
  if(derecha == 1){
      Timer2.attachInterrupt(&speedUpRight).start(2000000);
      derecha = 0;
    //   cambioDeSentido1 = 1;
    //   cambioDeSentido2 = 1;
    }else if(izquierda == 1){
      Timer2.attachInterrupt(&speedUpLeft).start(2000000);
      izquierda = 0;
    //   cambioDeSentido1 = 1;
    //   cambioDeSentido2 = 1;
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