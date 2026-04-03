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
#ifndef _SYSTEM_FUNCTION_H_
#define _SYSTEM_FUNCTION_H_

void Delay_ms(unsigned int milliseconds);

uint16_t Read_ADC(uint8_t Channel);

void UART_Transmit(char data);

void UART2_WriteString(const char *str);

int major(int A, int B, int C, int D, int E);

int majorB(int A, int B, int C);

void SVM(float *Anpha, float Vref, uint8_t Udc, uint16_t Tz);

#endif