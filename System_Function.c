/*
 * File:   System_Function.c
 * Author: Admin
 *
 * Created on October 6, 2025, 10:15 AM
 */


#include <xc.h>
#include "System_Init.h"
#include <math.h>

void Delay_ms(unsigned int milliseconds)
{
    unsigned int i, j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 4000; j++) {
            // ?i?u ch?nh con s? 4000 d?a tr?n t?n s? xung nh?p c?a vi ?i?u khi?n
            // v? th? nghi?m ?? c? ???c th?i gian ch?nh x?c.
            Nop(); // Nop() l? m?t l?nh kh?ng l?m g? c? trong Microchip MPLAB XC16
        }
    }
}

uint32_t Read_ADC(uint8_t Channel)
{
    ADCCON3bits.ADINSEL = Channel;
    ADCCON3bits.RQCNVRT = 1;
    
    return (uint32_t)(*((&ADCDATA0) + (Channel << 2)));
}

void UART_Transmit(char data) {
    while (U2STAbits.UTXBF == 1); // Cho den khi bo dem truyen san sang
    U2TXREG = data; // Gui du lieu
}

void UART2_WriteString(const char *str) 
{
    while (*str != '\0') {     // Gui tung ky tu cua chuoi cho den khi gap ky tu ket thu chuoi '\0'
        if(U2STAbits.TRMT == 1)
        {
            UART_Transmit(*str);
            str++;  
        }
    }
}

int major(int A, int B, int C, int D, int E)
{
    return (A && B && C) || (A && B && D) || (A && B && E) ||
           (A && C && D) || (A && C && E) || (A && D && E) ||
           (B && C && D) || (B && C && E) || (B && D && E) ||
           (C && D && E);
}

int majorB(int A, int B, int C)
{
    return (A && B) || (A && C) || (B && C);
}

void SVM(float *Anpha, float Vref, uint8_t Udc, uint16_t Tz)
{
    static uint16_t T0 = 0;
    static uint16_t T1 = 0;
    static uint16_t T2 = 0;
    static uint8_t Sector;
    
    *Anpha = fmodf(*Anpha, 2*M_PI);
    if(*Anpha >= 0 && *Anpha < M_PI/3)
    {
        Sector = 1;
    }
    else if(*Anpha >= M_PI/3 && *Anpha < 2*M_PI/3)
    {
        Sector = 2;
    }
    else if(*Anpha >= 2*M_PI/3 && *Anpha < 3*M_PI/3)
    {
        Sector = 3;
    }
    else if(*Anpha >= 3*M_PI/3 && *Anpha < 4*M_PI/3)
    {
        Sector = 4;
    }
    else if(*Anpha >= 4*M_PI/3 && *Anpha < 5*M_PI/3)
    {
        Sector = 5;
    }
    else if(*Anpha >= 5*M_PI/3 && *Anpha < 6*M_PI/3)
    {
        Sector = 6;
    }
    
    T1 = sqrt(3)*Tz*Vref*sin(M_PI/3 - (*Anpha) + (Sector - 1)*M_PI/3)/Udc;
    T2 = sqrt(3)*Tz*Vref*sin((*Anpha) - (Sector - 1)*M_PI/3)/Udc;
    T0 = Tz - T1 - T2;
    
    switch(Sector)
    {
        case 1:
            PDC1 = T1 + T2 + T0/2;
            PDC2 = T2 + T0/2;
            PDC3 = T0/2;
            break;
        case 2:            
            PDC1 = T1 + T0/2;
            PDC2 = T1 + T2 + T0/2;
            PDC3 = T0/2;
            break;
        case 3:            
            PDC1 = T0/2;
            PDC2 = T1 + T2 + T0/2;
            PDC3 = T2 + T0/2;
            break;
        case 4:            
            PDC1 = T0/2;
            PDC2 = T1 + T0/2;
            PDC3 = T1 + T2 + T0/2;
            break;
        case 5:            
            PDC1 = T2 + T0/2;
            PDC2 = T0/2;
            PDC3 = T1 + T2 + T0/2;
            break;
        case 6:            
            PDC1 = T1 + T2 + T0/2;
            PDC2 = T0/2;
            PDC3 = T1 + T0/2;
            break;
    }
}
