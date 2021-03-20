#include "main.h"

void
proportionalController(float finalRad, float actualRad, float Kp){
    float u = (finalRad-actualRad)*Kp;
    setPWM(u, 20000);
}

void
proportionalDerivativeController(float finalRad, float actualRad, float lastRad, float Kp, float tauD){

}

void
proportionalIntegralController(float finalRad, float actualRad, float lastRadArray[] ,float Kp, float tauI){

}

void
proportionalIntegralDerivativeController(float finalRad, float actualRad, float lastRad, float lastRadArray[], float Kp, float tauI, float tauD){
    
}