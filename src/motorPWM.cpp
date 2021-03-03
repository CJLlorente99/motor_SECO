#include "motorPWM.h"

int setPWM(direccion dir, float dutycycle){
    // TODO
    // Change update registers

    // int dutycycle_int = (int)(dutycycle * 100);
    // long pwm_level = (long) map(dutycycle_int, 0, 10000, 0 , 255);
    
    // if(dir == HORARIO){
    //     analogWrite(port_PWM_OUT1, (uint32_t) pwm_level);
    //     analogWrite(port_PWM_OUT2, (uint32_t) 0);
    // } else if(dir == ANTI_HORARIO){
    //     analogWrite(port_PWM_OUT2, (uint32_t) pwm_level);
    //     analogWrite(port_PWM_OUT1, (uint32_t) 0);
    // }

    return 0;
}

int configurePWM(direccion dir, float dutycycle, int freq){
    uint32_t value;

    // Steps followed for configuration in page 996 (ATMEL SAM3X datasheet)
    // REG_PMC_PCER1 |= PMC_PCER1_PID36;
    REG_PIOC_WPMR &= !TC_WPMR_WPEN;
    REG_PIOC_ABSR |= PIO_ABSR_P3;
    REG_PIOC_ABSR |= PIO_ABSR_P7;
    REG_PIOC_PDR |= PIO_PDR_P7;
    REG_PIOC_PDR |= PIO_PDR_P3;

    // PWM_WPCR Register configuration. WPCMD
    uint32_t* PWM_WPCR = (uint32_t*) REG_PWM_WPCR;
    value = 0;
    value |= PWM_WPCR_WPCMD(1);
    value |= PWM_WPCR_WPRG0;
    value |= PWM_WPCR_WPRG1;
    *PWM_WPCR = value;

    uint32_t* PWM_WPSR = (uint32_t*) 0x400940E8;
    value = *PWM_WPSR;
    value &= !PWM_WPSR_WPSWS3;
    value &= !PWM_WPSR_WPHWS3;
    value &= !PWM_WPSR_WPSWS2;
    value &= !PWM_WPSR_WPHWS2;
    value &= !PWM_WPSR_WPSWS0;
    value &= !PWM_WPSR_WPHWS0;
    *PWM_WPSR = value;

    // Clock configuration PWM_CLK
    // uint32_t* PWM_CLK = (uint32_t*) REG_PWM_CLK;
    // value = 0;
    // value |= PWM_CLK_DIVA(1);
    // value |= PWM_CLK_DIVB(1);
    // value |= PWM_CLK_PREA(8);
    // value |= PWM_CLK_PREB(8);
    // *PWM_CLK = value;

    // Clock selection for each channel (Channel 0 and Channel 1) PWM_CMRx
    // Configure waveform allignment PWM_CMRx
    // Counter event selection PWM_CMRx
    // Configure output waveform polarity PWM_CMRx
    int CPRE_CLK = 84 * 10^6;

    uint32_t* PWM_CMR0 = (uint32_t*) REG_PWM_CMR0;
    uint32_t* PWM_CMR1 = (uint32_t*) REG_PWM_CMR1;
    value = 0;
    value |= PWM_CMR_CPRE_MCK;
    value |= !PWM_CMR_CALG;
    value |= !PWM_CMR_CES;
    value |= !PWM_CMR_CPOL;
    *PWM_CMR0 = value;
    *PWM_CMR1 = value;    

    // Configure period for each channel PWM_CPRD
    int CPRD_counter = (int) (CPRE_CLK/freq);

    uint32_t* PWM_CPRD0 = (uint32_t*) REG_PWM_CPRD0;
    uint32_t* PWM_CPRD1 = (uint32_t*) REG_PWM_CPRD1;
    value = 0;
    value |= PWM_CPRD_CPRD(CPRD_counter);
    *PWM_CPRD0 = value;
    *PWM_CPRD1 = value;

    // Configure duty cycle PWM_CDTYx
    int dutycycle_int = (int)(dutycycle * 100);
    uint16_t pwm_level = (uint16_t) map(dutycycle_int, 0, 100, 0 , CPRD_counter);

    if(dir == HORARIO){
        uint32_t* PWM_CDTY0 = (uint32_t*) REG_PWM_CDTY0;
        uint32_t* PWM_CDTY1 = (uint32_t*) REG_PWM_CDTY1;
        value = 0;
        value |= PWM_CDTY_CDTY(pwm_level);
        *PWM_CDTY0 = value;
        *PWM_CDTY1 = value;
    } else{
        uint32_t* PWM_CDTY0 = (uint32_t*) REG_PWM_CDTY0;
        uint32_t* PWM_CDTY1 = (uint32_t*) REG_PWM_CDTY1;
        value = 0;
        value |= PWM_CDTY_CDTY(pwm_level);
        *PWM_CDTY1 = value;
        *PWM_CDTY0 = value;
    }

    // Configure deadtime (idle)

    // Selection of synchronous channels PWM_SCM
    uint32_t* PWM_SCM = (uint32_t*) REG_PWM_SCM;
    value = 0;
    value |= PWM_SCM_SYNC0;
    value |= PWM_SCM_SYNC1;
    *PWM_SCM = value;

    // Configure WRDY flag PWM_SCM (not neccessary right now)

    // Configurate of update mode (not neccessary right now)

    // Configurate of update period (not neccessary right now)

    // Configurate comparisons
    // uint32_t* PWM_CMPM0 = (uint32_t*) REG_PWM_CMPM0;
    // uint32_t* PWM_CMPM1 = (uint32_t*) REG_PWM_CMPM1;
    // value = 0;
    // *PWM_CMPM0 = value;
    // *PWM_CMPM1 = value;

    // Configurate event lines (not neccessary right now)

    // Configurate fault inputs polarity (not neccessary right now)

    // Configurate fault protection (not neccessary right now)

    // Enable interrupts
    uint32_t* PWM_IER1 = (uint32_t*) REG_PWM_IER1;
    value = 0;
    value |= PWM_ENA_CHID0;
    value |= PWM_ENA_CHID1;
    value |= PWM_IER1_FCHID0;
    value |= PWM_IER1_FCHID1;
    *PWM_IER1 = value;

    uint32_t* PWM_IER2 = (uint32_t*) REG_PWM_IER2;
    value = 0;
    value |= PWM_IDR2_WRDY;
    value |= PWM_IER2_ENDTX;
    value |= PWM_IER2_TXBUFE;
    value |= PWM_IER2_UNRE;
    *PWM_IER2 = value;

    // Enable PWM channels PWM_ENA
    uint32_t* PWM_ENA = (uint32_t*) REG_PWM_ENA;
    value = 0;
    value |= PWM_ENA_CHID0;
    value |= PWM_ENA_CHID1;
    *PWM_ENA = value;

    return 0;  
}