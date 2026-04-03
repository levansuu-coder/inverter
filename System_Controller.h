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

#ifndef _SYSTEM_CONTROLLER_H    /* Guard against multiple inclusion */
#define _SYSTEM_CONTROLLER_H
/*CURENT*/

/*VOLTAGE*/


typedef struct{
    float kp;
    float ki;
    float w0;
    float wc;
    float Ts;
    float ek;
    float ek1;
    float ek2;
    float xk;
    float xk1;
    float xk2;
    float yk;
    float a1;
    float b0;
    float b1;
    float b2;
    float max_output;
    float min_output;
}PRControllerTypedef;

void initialize_PIcontroller(PRControllerTypedef *controller, float kp, float ki, float max_output, float min_output);

void PR_Cal(PRControllerTypedef *controller, float e, float *u);

void abc_to_dq(float *a, float *b, float *c, float *d, float *q, float *theta);

void dq_to_anphabeta(float *d, float *q, float *anpha, float *beta, float *theta);

void DCVTKG(float *anpha, float *beta, float udc, uint16_t tz);

void dq_to_SVM(float *theta, float *vref, uint8_t udc, uint16_t tz);
#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
