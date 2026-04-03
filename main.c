/*
 * File:   main.c
 * Author: Admin
 *
 * Created on October 6, 2025, 9:38 AM
 */
/*INCLUDE*/
#include <xc.h>
#include <proc/p32mk1024mcm100.h>
#include <sys/attribs.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "System_Init.h"
#include "System_Function.h"
#include "System.h"
#include "System_Controller.h"
/*INCLUDE*/

/*TYPEDEF*/
PRControllerTypedef PR_Ua;
PRControllerTypedef PR_Ub;
PRControllerTypedef PR_Uc;
/*TYPEDEF*/

/*Define*/
#define PI                  3.14159265358979323846f  /* pi */
#define UDC                 700.0f
#define Tz                  6000
#define DEADTIME            300
#define FEADBACK            0
//#define HESO_KHUECHDAI_U    0.203885f
#define HESO_KHUECHDAI_U    0.162017587f

//voltage
#define KP_U                0.1f
#define KI_U                25.0f

/*Define*/

/*USER CODE*///*****************************************************************

/*UART*/
char Buffer_TX[150];
/*UART*/

/*SATE*/

/*STATE*/

/*DUTYCYCLE*/

/*DUTYCYCLE*/

/*CURRENT*/

/*CURRENT*/

/*VOLTAGE*/
float Vref = 0.0f;
float Ua_ref;
float Ua_real;
float Ub_ref;
float Ub_real;
float Uc_ref;
float Uc_real;
float ua;
float ub;
float uc;
float Ua_real_current;
float Ua_real_pre = 0.0f;
float Ua_real_pre_pre = 0.0f;
float Ub_real_current;
float Ub_real_pre = 0.0f;
float Ub_real_pre_pre = 0.0f;
float Uc_real_current;
float Uc_real_pre = 0.0f;
float Uc_real_pre_pre = 0.0f;
/*VOLTAGE*/

/*ROTATION*/

/*ROTATION*/

/*ANGLE*/
uint8_t t = 0;
/*ANGLE*/

/*USER CODE*///*****************************************************************

/*INTERUPT*//******************************************************************/
void __ISR(_TIMER_1_VECTOR, IPL1SRS) T1Interrupt(void) //Timer1
{
    IFS0bits.T1IF = 0;
}

void __ISR(_TIMER_2_VECTOR, IPL1SRS) T2Interrupt(void) //Timer2
{
    IFS0bits.T2IF = 0; 
}

void __ISR(_TIMER_3_VECTOR, IPL1SRS) T3Interrupt(void) //Timer3
{
    IFS0bits.T3IF = 0;
}

void __ISR(_CHANGE_NOTICE_B_VECTOR, IPL1SRS) CNBInterrupt(void) //FAUL MOTOR
{
    PORTB;
    IFS1bits.CNBIF = 0;
}

void __ISR(_CHANGE_NOTICE_C_VECTOR, IPL3SRS) CNCInterrupt(void) //HALL_A
{
    PORTC;
    IFS1bits.CNCIF = 0;
    
    if(PORTCbits.RC9 == 1)
    {
        PTCONbits.PTEN  = 0x0; //PWM
        LED2 = 1;
        while(1);
    }
}

void __ISR(_CHANGE_NOTICE_E_VECTOR, IPL3SRS) CNEInterrupt(void) //HALL_B
{
    PORTE;
    IFS1bits.CNEIF = 0;
}

void __ISR(_CHANGE_NOTICE_A_VECTOR, IPL3SRS) CNAInterrupt(void) //HALL_C
{
    PORTA;
    IFS1bits.CNAIF = 0;
}

void __ISR(_PWM1_VECTOR, IPL1SRS) PWM1Interrupt(void) //PWM
{
    IFS5bits.PWM1IF = 0;
    PWMCON1bits.TRGIF = 0;
}

void __ISR(_PWM2_VECTOR, IPL5SRS) PWM2Interrupt(void) //PWM
{
    IFS5bits.PWM2IF = 0;
    PWMCON2bits.TRGIF = 0;
    LED1 = 1;;
    
#if FEADBACK
    
    /*MEASURE*/
    Ua_real_current = (float)(Read_ADC(16))*HESO_KHUECHDAI_U;
    Ub_real_current = (float)(Read_ADC(17))*HESO_KHUECHDAI_U;
    Uc_real_current = (float)(Read_ADC(41))*HESO_KHUECHDAI_U;
    
    Ua_real = (Ua_real_current + Ua_real_pre + Ua_real_pre_pre)/3.0f;
    Ub_real = (Ub_real_current + Ub_real_pre + Ub_real_pre_pre)/3.0f;
    Uc_real = (Uc_real_current + Uc_real_pre + Uc_real_pre_pre)/3.0f;
    /*MEASURE*/
    
    /*VOLTAGE REFERENCE*/
    t++;
    t = t%200;
    
    Ua_ref = Vref*sinf(0.01f*PI*t);
    Ub_ref = Vref*sinf(0.01f*PI*t - 2.0f*PI/3.0f);
    Uc_ref = Vref*sinf(0.01f*PI*t + 2.0f*PI/3.0f);
    /*VOLTAGE REFERENCE*/
    
    /*VOLTAGE PR CONTROLLER*///*************************************************
    PR_Cal(&PR_Ua, Ua_ref - Ua_real, &ua);
//    PR_Cal(&PR_Ub, Ub_ref - Ub_real, &ub);
//    PR_Cal(&PR_Uc, Uc_ref - Uc_real, &uc);
    /*VOLTAGE PR CONTROLLER*///*************************************************
    
    /*CURRENT PR CONTROLLER*///*************************************************
    /*CURRENT PR CONTROLLER*///*************************************************
    
    /*PWM*/
    ua = ua/UDC;
    ua = (ua + 1.0f)/2.0f;
    ub = ub/UDC;
    ub = (ub + 1.0f)/2.0f;
    uc = uc/UDC;
    uc = (uc + 1.0f)/2.0f;
    
    //PDC1 = ua*6000;
    //PDC2 = ub*6000;
    //PDC3 = uc*6000;
    /*PWM*/
    
    /*pre = current*/
    Ua_real_pre_pre = Ua_real_pre;
    Ub_real_pre_pre = Ub_real_pre;
    Uc_real_pre_pre = Uc_real_pre;
    
    Ua_real_pre = Ua_real_current;
    Ub_real_pre = Ub_real_current;
    Uc_real_pre = Uc_real_current;
    /*pre = current*/
    
#else    
    
    /*VOLTAGE REFERENCE*/
    t++;
    t = t%200;
    
    Ua_ref = Vref*sinf(0.01f*M_PI*t);
    Ub_ref = Vref*sinf(0.01f*M_PI*t - 2.0f*M_PI/3.0f);
    Uc_ref = Vref*sinf(0.01f*M_PI*t + 2.0f*M_PI/3.0f);
    
    Ua_ref = fabsf(Ua_ref);
    Ub_ref = fabsf(Ub_ref);
    Uc_ref = fabsf(Uc_ref);
    
//    Ua_ref = (Ua_ref + 1.0f)/2.0f;
//    Ub_ref = (Ub_ref + 1.0f)/2.0f;
//    Uc_ref = (Uc_ref + 1.0f)/2.0f;
    /*VOLTAGE REFERENCE*/
    
    /*PWM*/
    if(t > 0 && t < 100)
    {
        PDC2 = 0;
        PDC1 = Ua_ref*6000;
    }
    else if(t > 100 && t <= 199)
    {
        PDC1 = 0;
        PDC2 = Ua_ref*6000;
    }
    else if(t == 0 || t == 100)
    {
        PDC1 = 0;
        PDC2 = 0;
    }
//    PDC1 = Ua_ref*6000;
//    PDC2 = Ub_ref*6000;
//    PDC3 = Uc_ref*6000;
    /*PWM*/
    
#endif
    
    LED1 = 0;
}

void __attribute__((used)) __ISR(_UART2_RX_VECTOR, ipl1SRS) UART2_RX_Handler (void)
{
    IFS1CLR = _IFS1_U2RXIF_MASK;
}

void __attribute__((used)) __ISR(_UART2_TX_VECTOR, ipl1SRS) UART2_TX_Handler (void)
{
    IFS1CLR = _IFS1_U2TXIF_MASK;
}
void __attribute__((used)) __ISR(_DMA0_VECTOR, ipl1SRS) DMA0_Handler (void)
{

}
/*INTERRUPT*//*****************************************************************/

int main(void)
{    
    /*System Initialize*/
    SYS_Initialize(NULL);
    /*System Initialize*/
    
    /*ENABLE*/
    Enable_System();
    /*ENABLE*/
    
    /*USER_0*/
    initialize_PIcontroller(&PR_Ua, KP_U, KI_U, UDC, -UDC);
    initialize_PIcontroller(&PR_Ub, KP_U, KI_U, UDC, -UDC);
    initialize_PIcontroller(&PR_Uc, KP_U, KI_U, UDC, -UDC);
    /*USER_0*/
    
    /*USER_1*/    
    while(S2 == 1){}
    while(S2 == 0){}

    Start_Motor();
    
    /*USER_1*/
    
    while(1)
    {    
#if FEADBACK
        Vref = Read_ADC(15)*UDC/2.0f/4095.0f;  
#else
        Vref = Read_ADC(15)*1.0f/4095.0f;
#endif
        
//        sprintf(Buffer_TX, "PR_Ia.ek = %.2f, PR_Ia.xk = %.2f, PR_Ia.yk = %.2f, Ia_ref = %.2f\r\n", PR_Ua.ek, PR_Ua.xk, PR_Ua.yk, Ua_ref);
//        sprintf(Buffer_TX, "PR_Ia.a1 = %.2f, PR_Ia.b0 = %.2f, PR_Ia.b1 = %.2f, PR_Ia.b2 = %.2f\r\n", PR_Ia.a1, PR_Ia.b0, PR_Ia.b1, PR_Ia.b2);
//        sprintf(Buffer_TX, "ua = %.2f, ub = %.2f, uc = %.2f\r\n", Ua_real, Ub_real, Uc_real);
//        UART2_WriteString(Buffer_TX);
        Delay_ms(10);
    }
}
