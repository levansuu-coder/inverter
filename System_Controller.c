/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <xc.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "System_Controller.h"

#define PI            3.14159265358979323846F  /* pi */

void initialize_PIcontroller(PRControllerTypedef *controller, float kp, float ki, float max_output, float min_output)
{
    controller->kp  = kp;
    controller->ki  = ki;
    controller->w0  = 2.0f*PI*50.0f;
    controller->wc  = 0.5f;
    controller->Ts  = 1.0f/10000.0f;
    controller->a1  = 4.0f*controller->ki*controller->Ts*controller->wc;
    controller->b0  = controller->Ts*controller->Ts*controller->w0*controller->w0 + 4.0f*controller->Ts*controller->wc + 4.0f;
    controller->b1  = 2.0f*controller->Ts*controller->Ts*controller->w0*controller->w0 - 8.0f;
    controller->b2  = controller->Ts*controller->Ts*controller->w0*controller->w0 - 4.0f*controller->Ts*controller->wc + 4.0f;
    controller->ek1 = 0.0f;
    controller->ek2 = 0.0f;
    controller->xk  = 0.0f;
    controller->xk1 = 0.0f;
    controller->xk2 = 0.0f;
    controller->yk  = 0.0f;
    controller->max_output = max_output;
    controller->min_output = min_output;
}

void PR_Cal(PRControllerTypedef *controller, float e, float *u)
{
    controller->ek  = e;
    controller->xk  = (controller->a1*controller->ek - controller->a1*controller->ek2 - controller->b1*controller->xk1 - controller->b2*controller->xk2)/controller->b0;
    controller->yk  = controller->ek*controller->kp + controller->xk;
    controller->xk2 = controller->xk1;
    controller->xk1 = controller->xk;
    controller->ek2 = controller->ek1;
    controller->ek1 = controller->ek;
    
    if((controller->yk <= controller->max_output) && (controller->yk >= controller->min_output))
    {
        *u = controller->yk;        
    }
    else
    {
        if(controller->yk > controller->max_output)
        {
           *u = controller->max_output; 
           controller->yk = controller->max_output;
        }
        else if(controller->yk < controller->min_output)
        {
            *u = controller->min_output;
            controller->yk = controller->min_output;
        }
    }
}

void abc_to_dq(float *a, float *b, float *c, float *d, float *q, float *theta)
{
    *d = 2.0f/3.0f*((*a)*cosf((*theta))+(*b)*cosf((*theta)-2.0f*PI/3.0f)+(*c)*cosf((*theta)+2.0f*PI/3.0f));
    *q = 2.0f/3.0f*(-(*a)*sinf((*theta))-(*b)*sinf((*theta)-2.0f*PI/3.0f)-(*c)*sinf((*theta)+2.0f*PI/3.0f));
}

void dq_to_anphabeta(float *d, float *q, float *anpha, float *beta, float *theta)
{
    *anpha  = (*d)*cosf((*theta)) - (*q)*sinf((*theta));
    *beta   = (*d)*sinf((*theta)) + (*q)*cosf((*theta));
}

void DCVTKG(float *anpha, float *beta, float udc, uint16_t tz)
{
    static uint8_t sector;
    static float beta2;
    static float d0;
    static float d1;
    static float d2;

    beta2 = (*beta)/sqrtf(3.0f);
    
    //xac dinh sector
    if(beta2 >= 0)
    {
        if(beta2 < (*anpha))
        {
            sector = 1;
        }
        else 
        {
            if(beta2 < -(*anpha))
            {
                sector = 3;
            }
            else 
            {
                sector = 2;
            }
        }
    }
    else
    {
        if(beta2 > (*anpha))
        {
            sector = 4;
        }
        else
        {
            if(beta2 < -(*anpha))
            {
                sector = 5;
            }
            else
            {
                sector = 6;
            }
        }
    }
    
    //tinh d0, d1, d2
    switch(sector)
    {
        case 1:
            d1=1/udc*(3.0f/2.0f*(*anpha)-sqrtf(3.0f)/2.0f*(*beta));
            d2=1/udc*sqrtf(3.0f)*(*beta);
            d0=1.0f-d1-d2;
            
            PDC2=(d0/2.0f+d2)*tz;
            PDC1=(d0/2.0f+d1+d2)*tz;
            PDC3=d0/2.0f*tz;
            break;
        case 2:
            d1=1.0f/udc*(-3.0f/2.0f*(*anpha)+sqrtf(3.0f)/2.0f*(*beta));
            d2=1.0f/udc*(3.0f/2.0f*(*anpha)+sqrtf(3.0f)/2.0f*(*beta));
            d0=1.0f-d1-d2;
            
            PDC1=(d0/2.0f+d2)*tz;
            PDC3=d0/2.0f*tz;
            PDC2=(d0/2.0f+d1+d2)*tz;
            break;
        case 3:
            d1=1.0f/udc*sqrtf(3.0f)*(*beta);
            d2=1.0f/udc*(-3.0f/2.0f*(*anpha)-sqrtf(3.0f)/2.0f*(*beta));
            d0=1.0f-d1-d2;
            
            PDC2=(d0/2.0f+d1+d2)*tz;
            PDC1=d0/2.0f*tz;
            PDC3=(d0/2.0f+d2)*tz;
            break;
        case 4:
            d1=1.0f/udc*-sqrtf(3.0f)*(*beta);
            d2=1.0f/udc*(-3.0f/2.0f*(*anpha)+sqrtf(3.0f)/2.0f*(*beta));
            d0=1.0f-d1-d2;
            
            PDC3=(d0/2.0f+d1+d2)*tz;
            PDC2=(d0/2.0f+d2)*tz;
            PDC1=d0/2.0f*tz;
            break;
        case 5:
            d1=1.0f/udc*(-3.0f/2.0f*(*anpha)-sqrtf(3.0f)/2.0f*(*beta));
            d2=1.0f/udc*(3.0f/2.0f*(*anpha)-sqrtf(3.0f)/2.0f*(*beta));
            d0=1.0f-d1-d2;
            
            PDC1=(d0/2.0f+d2)*tz;
            PDC3=(d0/2.0f+d1+d2)*tz;
            PDC2=d0/2.0f*tz;
            break;
        case 6:
            d1=1.0f/udc*(3.0f/2.0f*(*anpha)+sqrtf(3.0f)/2.0f*(*beta));
            d2=1.0f/udc*-sqrtf(3.0f)*(*beta);
            d0=1.0f-d1-d2;
            
            PDC2=d0/2.0f*tz;
            PDC1=(d0/2.0f+d1+d2)*tz;
            PDC3=(d0/2.0f+d2)*tz;
            break;
    }
}

void dq_to_SVM(float *theta, float *vref, uint8_t udc, uint16_t tz)
{
    static uint16_t T0 = 0;
    static uint16_t T1 = 0;
    static uint16_t T2 = 0;
    static uint8_t sector = 0;
    
    if(*theta >= 0 && *theta < PI/3.0f)
    {
        sector = 1;
    }
    else if(*theta >= PI/3.0f && *theta < 2.0f*PI/3.0f)
    {
        sector = 2;
    }
    else if(*theta >= 2.0f*PI/3.0f && *theta < 3.0f*PI/3.0f)
    {
        sector = 3;
    }
    else if(*theta >= 3.0f*PI/3.0f && *theta < 4.0f*PI/3.0f)
    {
        sector = 4;
    }
    else if(*theta >= 4.0f*PI/3.0f && *theta < 5.0f*PI/3.0f)
    {
        sector = 5;
    }
    else if(*theta >= 5.0f*PI/3.0f && *theta < 6.0f*PI/3.0f)
    {
        sector = 6;
    }
    
    T1 = sqrtf(3.0f)*tz*(*vref)*sinf(PI/3.0f - (*theta) + (float)(sector - 1)*PI/3.0f)/udc;
    T2 = sqrtf(3.0f)*tz*(*vref)*sinf((*theta) - (float)(sector - 1)*PI/3.0f)/udc;
    T0 = tz - T1 - T2;
    
    switch(sector)
    {
        case 1:
            PDC1 = T1 + T2 + T0/2.0f;
            PDC2 = T2 + T0/2.0f;
            PDC3 = T0/2.0f;
            break;
        case 2:            
            PDC1 = T1 + T0/2.0f;
            PDC2 = T1 + T2 + T0/2.0f;
            PDC3 = T0/2.0f;
            break;
        case 3:            
            PDC1 = T0/2.0f;
            PDC2 = T1 + T2 + T0/2.0f;
            PDC3 = T2 + T0/2.0f;
            break;
        case 4:            
            PDC1 = T0/2.0f;
            PDC2 = T1 + T0/2.0f;
            PDC3 = T1 + T2 + T0/2.0f;
            break;
        case 5:            
            PDC1 = T2 + T0/2.0f;
            PDC2 = T0/2.0f;
            PDC3 = T1 + T2 + T0/2.0f;
            break;
        case 6:            
            PDC1 = T1 + T2 + T0/2.0f;
            PDC2 = T0/2.0f;
            PDC3 = T1 + T0/2.0f;
            break;
    }
}