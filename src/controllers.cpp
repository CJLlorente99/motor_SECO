#include "main.h"

float
proportionalController(float finalRad, float actualRad, float Kp){
    float u = (finalRad*REDUCTORA-actualRad)*Kp;
    setPWM(u, FREQ);
    return u;
}

float
proportionalDerivativeController(float finalRad, float actualRad, float lastRad, float Kp, float tauD, float period){
    float error = finalRad*REDUCTORA - actualRad;
    float errorDerivative = (lastRad - actualRad)/(period/1000000);
    float u = Kp*error + Kp*tauD*errorDerivative;
    setPWM(u, FREQ);
    return u; 
}

float
proportionalIntegralController(float finalRad, float actualRad, float lastErrorArray[], int sizeError, float Kp, float tauI, float period){
    float error = finalRad*REDUCTORA - actualRad;

    float errorIntegral = 0;
    for (int i = 0; i < sizeError; i++)
    {
        errorIntegral += lastErrorArray[i] * period/1000000;
    }
     
    float u = Kp*error + Kp * errorIntegral / tauI;
    setPWM(u, FREQ);
    return u;
}

float
proportionalIntegralDerivativeController(float finalRad, float actualRad, float lastRad, float lastErrorArray[], int sizeError, float Kp, float tauI, float tauD, float period){
    float error = finalRad*REDUCTORA - actualRad;
    float errorDerivative = (lastRad - actualRad)/(period/1000000);

    float errorIntegral = 0;
    for (int i = 0; i < sizeError; i++)
    {
        errorIntegral += lastErrorArray[i] * period/1000000;
    }
    
    float u = Kp*error + Kp/tauI * errorIntegral + Kp*tauD*errorDerivative;
    setPWM(u, FREQ);
    return u;
}