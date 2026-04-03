/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#define LED2 LATGbits.LATG15 
#define LED1 LATFbits.LATF5

#define S2 PORTGbits.RG1
#define S3 PORTCbits.RC7

#define HALL_A PORTCbits.RC9
#define HALL_B PORTEbits.RE14
#define HALL_C PORTAbits.RA14

#define PWM1H LATBbits.LATB14
#define PWM1L LATBbits.LATB15
#define PWM2H LATBbits.LATB12
#define PWM2L LATBbits.LATB13
#define PWM3H LATBbits.LATB10
#define PWM3L LATBbits.LATB11

void CLK_Initialize(void);

void GPIO_Initialize(void);

void TMR1_Initialize(void);

void TMR2_Initialize(void);

void TMR3_Initialize(void);

void EVIC_Initialize(void);

void CN_Initialize(void);

void ADCHS_Initialize(void);

void MCPWM_Initialize(void);

void UART2_Initialize(void);

void DMA0_Initialize(void);

#endif
