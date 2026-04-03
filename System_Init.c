/*
 * File:   System_Init.c
 * Author: Admin
 *
 * Created on October 6, 2025, 9:41 AM
 */

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include <proc/p32mk1024mcm100.h>

void CLK_Initialize(void)
{
    /* unlock system for clock configuration */
    SYSKEY = 0x00000000U;
    SYSKEY = 0xAA996655U;
    SYSKEY = 0x556699AAU;
    
    /* Peripheral Module Disable Configuration */

    CFGCONbits.PMDLOCK = 0;

//    PMD1 = 0x370U;
//    PMD2 = 0x17001fU;
//    PMD3 = 0xffffffffU;
//    PMD4 = 0xff801feU;
//    PMD5 = 0xf30f3f3dU;
//    PMD6 = 0xf0d0000U;
//    PMD7 = 0x0U;
    PMD1 = PMD2 = PMD3 = PMD4 = PMD5 = PMD6 = PMD7 = 0;

    CFGCONbits.PMDLOCK = 1;

 
        /* Set up Reference Clock 1 */
    /* REFO1CON register */
    /* ROSEL =  SYSCLK */
    /* DIVSWEN = 1 */
    /* RODIV = 0 */
    REFO1CON = 0x200;

    /* REFO1TRIM register */
    /* ROTRIM = 0 */
    REFO1TRIM = 0x0;

    /* Enable oscillator (ON bit) and Enable Output (OE bit) */
    REFO1CONSET = 0x00001000 | 0x00008000;

  

    /* Lock system since done with clock configuration */
    SYSKEY = 0x33333333U;
}

void GPIO_Initialize ( void )
{
    /*PORT A*/
    TRISAbits.TRISA1 = 1; //V_M3
    ANSELAbits.ANSA1 = 1;
    TRISAbits.TRISA4 = 1; //Ia AN24
    ANSELAbits.ANSA4 = 1;
    TRISAbits.TRISA11 = 1; //Ib AN9
    ANSELAbits.ANSA11 = 1;
    /*PORT B*/
    TRISBbits.TRISB14 = 0; //PWM1H
    TRISBbits.TRISB15 = 0; //PWM1L
    TRISBbits.TRISB12 = 0; //PWM2H
    TRISBbits.TRISB13 = 0; //PWM2L
    TRISBbits.TRISB10 = 0; //PWM3H
    TRISBbits.TRISB11 = 0; //PWM3L
    
    TRISBbits.TRISB7 = 1; //I_SUM;
    ANSELBbits.ANSB7 = 1;
    TRISBbits.TRISB2 = 1; //V_M2
    ANSELBbits.ANSB2 = 1; 
    /*PORT C*/
    TRISCbits.TRISC7 = 1; //BT_S3
    TRISCbits.TRISC2 = 1; //V_M1
    ANSELCbits.ANSC2 = 1;
    TRISCbits.TRISC8 = 1; //UART RX
    /*PORT D*/

    /*PORT E*/
    TRISEbits.TRISE15 = 1; //POT
    ANSELEbits.ANSE15 = 1;
    TRISEbits.TRISE1 = 1; //V3
    ANSELEbits.ANSE1 = 1;
    //ANSELECLR = 0x4000U; /* Digital Mode Enable */
    /*PORT F*/
    TRISFbits.TRISF5 = 0; //LED1
    /*PORT G*/
    TRISGbits.TRISG15 = 0; //LED2
    TRISGbits.TRISG1 = 1; //BT_S2
    TRISGbits.TRISG9 = 1; //V1
    ANSELGbits.ANSG9 = 1;
    TRISGbits.TRISG8 = 1; //V2
    ANSELGbits.ANSG8 = 1;
}

void TMR1_Initialize(void)
{
    /* Disable Timer */
    T1CONbits.ON = 0;

    /*
    SIDL = 0
    TWDIS = 0
    TECS = 1
    TGATE = 0
    TCKPS = 0x111 256
    T32 = 1
    TSYNC = 0
    TCS = 0
    */
    T1CONSET = 0x78;
    

    /* Clear counter */
    TMR1 = 0x0;

    /*Set period */
    PR1 = 600000 - 1;

    /* Setup TMR1 Interrupt */
    IEC0bits.T1IE = 1;  /* Enable interrupt on the way out */
}

void TMR2_Initialize(void)
{
    /* Disable Timer */
    T2CONbits.ON = 0;

    /*
    SIDL = 0
    SYNC = 0
    TGATE = 0
    TCKPS =7
    T32   = 1
    TCS = 0
    */
    T2CONSET = 0x78;

    /* Clear counter */
    TMR2 = 0x0;

    /*Set period */
    //PR2 = 234374U; 1s
    PR2 = 600000 - 1; //1ms

    IEC0bits.T2IE = 1;  /* Enable interrupt on the way out */

}

void TMR3_Initialize(void)
{
    /* Disable Timer */
    T3CONbits.ON = 0;

    /*
    SIDL = 0
    SYNC = 0
    TGATE = 0
    TCKPS =0
    T32   = 1
    TCS = 0
    */
    //T3CONbits.T32 = 1;
    T3CONSET = 0x78;

    /* Clear counter */
    TMR3 = 0x0;

    /*Set period */
    PR3 = 120000 - 1; //rpm = 50hz

    IEC0bits.T3IE = 1;  /* Enable interrupt on the way out */    
}

void EVIC_Initialize( void )
{
    INTCONSET = _INTCON_MVEC_MASK;

    /* Set up priority and subpriority of enabled interrupts */
    IPC1bits.T1IP = 1;  /* TIMER_1:  Priority 1 / Subpriority 0 */
    IPC2bits.T2IP = 1;  /* TIMER_2:  Priority 1 / Subpriority 0 */
    IPC3bits.T3IP = 1;  /* TIMER_3:  Priority 1 / Subpriority 0 */
    IPC11bits.CNAIP = 2;  /* CHANGE_NOTICE_A:  Priority 1 / Subpriority 0 */
    IPC11bits.CNCIP = 2;  /* CHANGE_NOTICE_C:  Priority 1 / Subpriority 0 */
    IPC12bits.CNEIP = 2;  /* CHANGE_NOTICE_E:  Priority 1 / Subpriority 0 */
    IPC43bits.PWM1IP = 1; /*PWM1*/
    IPC43bits.PWM2IP = 5; /*PWM2*/
    IPC11bits.CNBIP = 5; /*CHANGE_NOTICE_B*/
    IPC14bits.U2RXIP = 1;
    IPC14bits.U2TXIP = 1;
    //IPC14SET = 0x400U | 0x0U;  /* UART2_RX:  Priority 1 / Subpriority 0 */
    //IPC14SET = 0x40000U | 0x0U;  /* UART2_TX:  Priority 1 / Subpriority 0 */
    /* Configure Shadow Register Set */
    PRISS = 0x76543210;

    while (PRISS != 0x76543210U)
    {
        /* Wait for PRISS value to take effect */
    }
}

void CN_Initialize(void)
{
    TRISCbits.TRISC9 = 1; //HALL_A
    TRISEbits.TRISE14 = 1; //HALL_B
    TRISAbits.TRISA14 = 1; //HALL_C
    TRISBbits.TRISB4 = 1; //FAULT MOTOR
  
    ANSELEbits.ANSE14 = 0; //digital
    ANSELAbits.ANSA14 = 0; //digital
    
    /*enable CN*/
    CNCONCbits.ON = 1; //HALL_A

//    CNCONEbits.ON = 1; //HALL_B
//
//    CNCONAbits.ON = 1; //HALL_C
//    
//    CNCONBbits.ON = 1; //FAULT MOTOR
    
    /*enable interupt*/
    IFS1bits.CNCIF = 0;
    IEC1bits.CNCIE = 1; //HALL_A
//    IFS1bits.CNEIF = 0;
//    IEC1bits.CNEIE = 1; //HALL_B
//    IFS1bits.CNAIF = 0; 
//    IEC1bits.CNAIE = 1; //HALL_C 
//    IFS1bits.CNBIF = 0;
//    IEC1bits.CNBIE = 1; //FAULT MOTOR
}

void ADCHS_Initialize(void)
{
    ADCCON1bits.ON = 0;

    ADC7CFG = DEVADC7;

    ADCCON1 = 0x600000U;
    ADCCON2 = 0x10001U;
    ADCCON3 = 0x1000000U;

    ADCTRGMODE = 0x0U;

    ADCTRG1 = 0x0U; 
    ADCTRG2 = 0x0U; 
    ADCTRG3 = 0x0U; 
    ADCTRG4 = 0x0U; 
    ADCTRG5 = 0x0U; 
    ADCTRG6 = 0x0U; 
    ADCTRG7 = 0x0U; 

    ADCTRGSNS = 0x0U;

    ADCIMCON1 = 0x0U;
    ADCIMCON2 = 0x0U; 
    ADCIMCON3 = 0x0U; 
    ADCIMCON4 = 0x0U; 
    
    ADCIMCON1bits.SIGN9 = 1;
    ADCIMCON2bits.SIGN24 = 1;
    
    ADCIMCON2bits.SIGN16 = 1;
    ADCIMCON2bits.SIGN17 = 1;
    ADCIMCON3bits.SIGN41 = 1;

    /* Input scan */
    ADCCSS1 = 0x0U;
    ADCCSS2 = 0x0U; 

    /* Turn ON ADC */
    ADCCON1bits.ON = 1;
    while(ADCCON2bits.BGVRRDY == 0U) // Wait until the reference voltage is ready
    {
        /* Nothing to do */
    }
    while(ADCCON2bits.REFFLT != 0U) // Wait if there is a fault with the reference voltage
    {
        /* Nothing to do */
    }

    /* ADC 7 */
    ADCANCONbits.ANEN7 = 1;      // Enable the clock to analog bias
    while(ADCANCONbits.WKRDY7 == 0U) // Wait until ADC is ready
    {
        /* Nothing to do */
    }
    ADCCON3bits.DIGEN7 = 1;      // Enable ADC
}

void MCPWM_Initialize(void)
{
    /* PTCON register  */
    /*  SEVTPS  = 0 */
    /*  PCLKDIV = 0 */
    /*  SEIEN   = false */
    PTCON = 0x0;

    /* STCON register  */
    /*  SEVTPS  = 0 */
    /*  SCLKDIV = 0 */
    /*  SSEIEN = false */
    STCON = 0x0;

    PTPER = 6000;
    STPER = 2000;
    SEVTCMP = 10;
    SSEVTCMP = 10;

    /*********** Channel 1 Configurations **********/
    /* PWMCON1 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  1 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = true */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON1 = 0x200410;

    /* IOCON1 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 3 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 0  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON1 = 0x3c000;

    PDC1 = 3000;
    SDC1 = 500;
    PHASE1 = 0;

    /* Dead Time */
    DTR1 = 100;
    ALTDTR1 = 100;

    /* Trigger Generator */
    TRGCON1 = 0x1000;
    TRIG1 = 0;
    STRIG1 = 0;

    /* leading edge blanking */
    /* LEBCON1 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON1 = 0x0;
    LEBDLY1 = 10;

    /* Enable interrupt */
    IEC5SET = _IEC5_PWM1IE_MASK;

    /*********** Channel 2 Configurations **********/
    /* PWMCON2 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  1 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON2 = 0x200410;

    /* IOCON2 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 3 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 0  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON2 = 0x3c000;

    PDC2 = 3000;
    SDC2 = 500;
    PHASE2 = 0;

    /* Dead Time */
    DTR2 = 100;
    ALTDTR2 = 100;

    /* Trigger Generator */
    TRGCON2 = 0x1000;
    TRIG2 = 5999;
    STRIG2 = 0;

    /* leading edge blanking */
    /* LEBCON2 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON2 = 0x0;
    LEBDLY2 = 10;

    /* Enable interrupt */
    IEC5SET = _IEC5_PWM2IE_MASK;
    
    /*********** Channel 3 Configurations **********/
    /* PWMCON3 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  1 */
    /*  ECAM   =  1 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON3 = 0x410;

    /* IOCON3 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 3 */
    /*  PENH  = 1 */
    /*  PENL  = 1 */
    /*  FLTPOL  = 0  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON3 = 0x3c000;

    PDC3 = 3000;
    SDC3 = 500;
    PHASE3 = 0;

    /* Dead Time */
    DTR3 = 100;
    ALTDTR3 = 100;

    /* Trigger Generator */
    TRGCON3 = 0x0;
    TRIG3 = 0;
    STRIG3 = 0;

    /* leading edge blanking */
    /* LEBCON3 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON3 = 0x0;
    LEBDLY3 = 10;
}

void UART2_Initialize( void )
{

    /* PPS Input Remapping */
    U2RXR = 6;

    /* PPS Output Remapping */
    RPB9R = 2; //RX
    /* Set up UxMODE bits */
    /* STSEL  = 0*/
    /* PDSEL = 0 */
    /* BRGH = 1 */
    /* RXINV = 0 */
    /* ABAUD = 0 */
    /* LPBACK = 0 */
    /* WAKE = 0 */
    /* SIDL = 0 */
    /* RUNOVF = 0 */
    /* CLKSEL = 0 */
    /* SLPEN = 0 */
    /* UEN = 0 */
    U2MODE = 0x8;

    /* Enable UART2 Receiver, Transmitter and TX Interrupt selection */
    //U2STASET = (_U2STA_UTXEN_MASK | _U2STA_URXEN_MASK | _U2STA_UTXISEL1_MASK );
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    
    U2STAbits.UTXISEL = 2;    
    U2STAbits.URXISEL = 1;

    /* BAUD Rate register Setup */
    //U2BRG = 1562;
    U2BRG = 129;

    /* Disable Interrupts */
    IEC1CLR = _IEC1_U2EIE_MASK;

    IEC1CLR = _IEC1_U2RXIE_MASK;

    IEC1CLR = _IEC1_U2TXIE_MASK;

    /* Initialize instance object */
//    uart2Obj.rxBuffer = NULL;
//    uart2Obj.rxSize = 0;
//    uart2Obj.rxProcessedSize = 0;
//    uart2Obj.rxBusyStatus = false;
//    uart2Obj.rxCallback = NULL;
//    uart2Obj.txBuffer = NULL;
//    uart2Obj.txSize = 0;
//    uart2Obj.txProcessedSize = 0;
//    uart2Obj.txBusyStatus = false;
//    uart2Obj.txCallback = NULL;
//    uart2Obj.errors = UART_ERROR_NONE;

    //IEC1SET = _IEC1_U2TXIE_MASK;
    /* Turn ON UART2 */
    //U2MODESET = _U2MODE_ON_MASK;
    //IEC1bits.U2TXIE = 1;
    IFS1CLR = _IFS1_U2RXIF_MASK;
    
    IEC1bits.U2RXIE = 0;
    IEC1bits.U2EIE = 0;
    
    U2MODEbits.ON = 1;    
}

void DMA0_Initialize(void)
{
    /* DMACON register */
    /* ON = 1          */
    DMACON = 0x8000U;

    /* DMA channel 0 configuration */
    /* CHPRI = 0, CHAEN= 0, CHCHN= 0, CHCHNS= 0x0, CHAED= 0 */
    DCH0CON = 0x0U;
    /* CHSIRQ = 58, SIRQEN = 1 */
    DCH0ECON = 0x3a10U;
    /* CHBCIE = 1, CHTAIE=1, CHERIE=1, CHSHIE= 0, CHDHIE= 0 */
    DCH0INT = 0xb0000U;


    /* Enable DMA channel interrupts */
    //IEC2SET = 0U | 0x100U;
}