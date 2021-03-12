#include "main.h"

const int CPRE_CLK = 84000000;
static int actualFreq;

int
setPWM(float volt, int freq){
    direccion dir;
    /* Voltage saturation, direction selection and conversion to dutycycle */
    if(volt > 12){
        volt = 12;
    } else if(volt < -12){
        volt = -12;
    }

    if(volt > 0){
        dir = HORARIO;
    } else{
        dir = ANTI_HORARIO;
    }

    int CPRD_counter = (int) (CPRE_CLK/freq);
    uint16_t pwm_level = abs(volt)/12 * CPRD_counter;


    /* Change update registers */
    

    if(dir == ANTI_HORARIO){
        REG_PWM_CDTYUPD0 |= PWM_CDTYUPD_CDTYUPD(pwm_level);
        REG_PWM_CDTYUPD1 |= PWM_CDTYUPD_CDTYUPD(0);
    } else{
        REG_PWM_CDTYUPD0 |= PWM_CDTYUPD_CDTYUPD(0);
        REG_PWM_CDTYUPD1 |= PWM_CDTYUPD_CDTYUPD(pwm_level);
    }

    if(actualFreq != freq){
        /* Clear bits to be able to write in PWM_CPRDUPDx */
        REG_PWM_WPSR &= !PWM_WPSR_WPSWS3;
        REG_PWM_WPSR &= !PWM_WPSR_WPHWS3;
        REG_PWM_CPRDUPD0 = 0;
        REG_PWM_CPRDUPD0 |= PWM_CPRD_CPRD(CPRD_counter);
        REG_PWM_CPRDUPD1 = 0;
        REG_PWM_CPRDUPD1 |= PWM_CPRD_CPRD(CPRD_counter);

        actualFreq = freq;
    }

    return 0;
}

int 
configurePWM(float volt, int freq){

    direccion dir;

    /* Steps followed for configuration in page 996 (ATMEL SAM3X datasheet) */

    /* Interrupt sources of the NVIC needed (PWM's id is 36) (interrupts not needed) */
    
    /* This bit needs to be clear in order to write in other needed PIO's registers */
    REG_PIOC_WPMR &= !TC_WPMR_WPEN;

    /* PMC configuration */
    /* Enable peripheral clocks (PWM's id is 36) */
    REG_PMC_PCER1 |= PMC_PCER1_PID36;

    /* PIO configuration */
    /* Select which peripheral is to be used (in our case -> B) */
    REG_PIOC_ABSR |= PIO_ABSR_P3;
    REG_PIOC_ABSR |= PIO_ABSR_P5;

    /* Select whether pin is to be controlled through the corresponding on-chip peripheral (A or B) or by the PIO controller */
    REG_PIOC_PDR = PIO_PDR_P3 | PIO_PDR_P5;

    /* PWM configuration */
    /* PWM_WPCR Register configuration. WPCMD */
    // REG_PWM_WPCR |= PWM_WPCR_WPCMD(1);
    // REG_PWM_WPCR |= PWM_WPCR_WPRG0;
    // REG_PWM_WPCR |= PWM_WPCR_WPRG1;

    /* Make sure some registers are 0 in order to be able to write afterwards */
    // REG_PWM_WPSR &= !PWM_WPSR_WPSWS3;
    // REG_PWM_WPSR &= !PWM_WPSR_WPHWS3;
    // REG_PWM_WPSR &= !PWM_WPSR_WPSWS2;
    // REG_PWM_WPSR &= !PWM_WPSR_WPHWS2;
    // REG_PWM_WPSR &= !PWM_WPSR_WPSWS0;
    // REG_PWM_WPSR &= !PWM_WPSR_WPHWS0;

    /* Clock selection for each channel (Channel 0 and Channel 1) PWM_CMRx
    Configure waveform allignment PWM_CMRx
    Counter event selection PWM_CMRx
    Configure output waveform polarity PWM_CMRx */

    REG_PWM_CLK |= PWM_CLK_DIVA(1);
    REG_PWM_CLK |= PWM_CLK_PREA(0);

    REG_PWM_CMR0 |= PWM_CMR_CPRE_CLKA;
    REG_PWM_CMR0 &= !PWM_CMR_CALG;
    REG_PWM_CMR0 &= !PWM_CMR_CES;
    REG_PWM_CMR0 |= PWM_CMR_CPOL;
    
    REG_PWM_CMR1 |= PWM_CMR_CPRE_CLKA;
    REG_PWM_CMR1 &= !PWM_CMR_CALG;
    REG_PWM_CMR1 &= !PWM_CMR_CES;
    REG_PWM_CMR1 |= PWM_CMR_CPOL;    

    /* Configure period for each channel PWM_CPRD */
    int CPRD_counter = (int) (CPRE_CLK/freq);
    actualFreq = freq;

    REG_PWM_CPRD0 |= PWM_CPRD_CPRD(CPRD_counter);
    REG_PWM_CPRD1 |= PWM_CPRD_CPRD(CPRD_counter);

    /* Configure initial duty cycle PWM_CDTYx */
    /* Voltage saturation, direction selection and conversion to dutycycle */
    if(volt > 12){
        volt = 12;
    } else if(volt < -12){
        volt = -12;
    }

    if(volt > 0){
        dir = HORARIO;
    } else{
        dir = ANTI_HORARIO;
    }

    uint16_t pwm_level = abs(volt)/12 * CPRD_counter;

    if(dir == ANTI_HORARIO){
        REG_PWM_CDTY0 |= PWM_CDTY_CDTY(pwm_level);
        REG_PWM_CDTY1 |= PWM_CDTY_CDTY(0);
    } else{
        REG_PWM_CDTY0 |= PWM_CDTY_CDTY(0);
        REG_PWM_CDTY1 |= PWM_CDTY_CDTY(pwm_level);
    }

    /* Configure deadtime (not necessary right now) */

    /* Selection of synchronous channels PWM_SCM (not necessary right now) */

    /* Configure WRDY flag PWM_SCM (not necessary right now) */

    /* Configurate of update mode (not necessary right now) */

    /* Configurate of update period (not necessary right now) */

    /* Configurate comparisons (not necessary right now) */

    /* Configurate event lines (not necessary right now) */

    /* Configurate fault inputs polarity (not necessary right now) */

    /* Configurate fault protection (not necessary right now) */

    /* Enable interrupts (not necessary) */

    /* Enable PWM channels PWM_ENA */
    REG_PWM_ENA |= PWM_ENA_CHID0;
    REG_PWM_ENA |= PWM_ENA_CHID1;

    return 0;  
}