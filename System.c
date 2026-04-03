/*
 * File:   System_Interupt.c
 * Author: Admin
 *
 * Created on October 6, 2025, 10:34 AM
 */
#include <xc.h>
#include  <sys/attribs.h>
#include "System_Init.h"

void SYS_Initialize(void* data)
{

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    /* Start out with interrupts disabled before configuring any modules */
    (void)__builtin_disable_interrupts();

  
    CLK_Initialize();

    /* Configure CP0.K0 for optimal performance (cached instruction pre-fetch) */
    __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3U));

    /* Configure Wait States and Prefetch */
    CHECONbits.PFMWS = 2;
    CHECONbits.PREFEN = 1;
    
    /*Initialize*/
	GPIO_Initialize();

//    TMR1_Initialize();
//
//    TMR2_Initialize();
//    
//    TMR3_Initialize();
//
    CN_Initialize();
    
    ADCHS_Initialize();

    MCPWM_Initialize();
//
//    UART2_Initialize();
//    
//    DMA0_Initialize();
    /*Initialize*/

    EVIC_Initialize();

	/* Enable global interrupts */
    (void)__builtin_enable_interrupts();
}

void Enable_System(void)
{
    T1CONbits.ON = 0;  // timer 1   
    
    T2CONbits.ON = 0;  // timer 2 
    
    T3CONbits.ON = 0;  // timer 3

    CNENBbits.CNIEB4    = 1; //FAULT MOTOR
    
    ADCCON3bits.DIGEN7  = 1; //ADC
}

void Start_Motor(void)
{
    CNENCbits.CNIEC9    = 1; //HALL_A
    CNENEbits.CNIEE14   = 0; //HALL_B
    CNENAbits.CNIEA14   = 0; //HALL_C
    
    PTCONbits.PTEN      = 0x1; //PWM 
    
    //T3CONbits.ON = 1;  // timer 3
}

void Stop_Motor(void)
{
    CNENCbits.CNIEC9    = 0; //HALL_A
    CNENEbits.CNIEE14   = 0; //HALL_B
    CNENAbits.CNIEA14   = 0; //HALL_C
    
    T1CONbits.ON    = 0;  // timer 1
    
    PTCONbits.PTEN  = 0x0; //PWM
    
    PWM1H = 0;
    PWM2H = 0;
    PWM3H = 0;
}

void Over_Current(void)
{
    CNENCbits.CNIEC9 = 0; //HALL_A
    CNENEbits.CNIEE14 = 0; //HALL_B
    CNENAbits.CNIEA14 = 0; //HALL_C
    CNENBbits.CNIEB4 = 0; //FAULT MOTOR
    
    PTCONbits.PTEN = 0x0; //PWM
    
    PWM1H = 0;
    PWM2H = 0;
    PWM3H = 0;
    
    LED1 = 1;
    LED2 = 1;
    
    while(1);
}
