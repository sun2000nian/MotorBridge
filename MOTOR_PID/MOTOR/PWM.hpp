#ifndef _PWM_HPP
#define _PWM_HPP
#include "main.h"
#include <stdlib.h>

struct PWM
{
    uint16_t pin;
    GPIO_TypeDef *port;
    char status;
    unsigned int cnt;
    unsigned int limit, limit_high, limit_low;
    struct PWM *next;
};

void PWM_Init();
void PWM_Init_Node(PWM *pwm, uint16_t pin, GPIO_TypeDef *port, float percent, unsigned int freq);
void PWM_SetFreq(GPIO_TypeDef *port, uint16_t pin, float percent, unsigned int freq);
void PWM_Update();

#endif
