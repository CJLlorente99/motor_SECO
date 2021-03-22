#include "main.h"

void
proportionalController(float finalRad, float actualRad, float Kp){
    float u = (finalRad-actualRad)*Kp;
    setPWM(u, FREQ);
}

void
proportionalDerivativeController(float finalRad, float actualRad, float lastRad, float Kp, float tauD, int period){
    float error = finalRad - actualRad;
    float errorDerivative = (lastRad - actualRad)/period;
    float u = Kp*error + Kp*tauD*errorDerivative;
    setPWM(u, FREQ); 
}

void
proportionalIntegralController(float finalRad, float actualRad, float lastErrorArray[], int sizeError, float Kp, float tauI, int period){
    float error = finalRad - actualRad;

    float errorIntegral = 0;
    for (int i = 0; i < sizeError; i++)
    {
        errorIntegral += lastErrorArray[i] * period;
    }
     
    float u = Kp*error + Kp/tauI * errorIntegral;
    setPWM(u, FREQ);
}

void
proportionalIntegralDerivativeController(float finalRad, float actualRad, float lastRad, float lastErrorArray[], int sizeError, float Kp, float tauI, float tauD, int period){
    float error = finalRad - actualRad;
    float errorDerivative = (lastRad - actualRad)/period;

    float errorIntegral = 0;
    for (int i = 0; i < sizeError; i++)
    {
        errorIntegral += lastErrorArray[i] * period;
    }
    
    float u = Kp*error + Kp/tauI * errorIntegral + Kp*tauD*errorDerivative;
    setPWM(u, FREQ);
}