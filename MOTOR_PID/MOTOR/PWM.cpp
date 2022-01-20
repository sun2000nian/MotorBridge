#include "PWM.hpp"

PWM *PWM_L;

void PWM_Init_Node(PWM *pwm, uint16_t pin, GPIO_TypeDef *port, float percent, unsigned int freq)
{
    pwm->cnt = 0;
    pwm->pin = pin;
    pwm->port = port;
    pwm->next = NULL;

    pwm->limit = (unsigned int)PWM_TIMER_CNT / freq;
    pwm->limit_high = pwm->limit * percent / 100;
    pwm->limit_low = pwm->limit - pwm->limit_high;
    pwm->status = 0;
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void PWM_SetFreq(GPIO_TypeDef *port, uint16_t pin, float percent, unsigned int freq)
{
    PWM *p = PWM_L;
    while (p->next != NULL)
    {
        if (p->next->pin == pin && p->next->port == port)
            break;
        p = p->next;
    }
    if (p->next == NULL)
    {
        PWM *q = (PWM *)malloc(sizeof(PWM));
        PWM_Init_Node(q, pin, port, percent, freq);
        p->next = q;
    }
    else
    {
        p = p->next;
        p->limit = (unsigned int)PWM_TIMER_CNT / freq;
        p->limit_high = p->limit * percent / 100;
        p->limit_low = p->limit - p->limit_high;
        p->cnt = 0;
        p->status = 0;
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    }
}

void PWM_Init()
{
    PWM_L = (PWM *)malloc(sizeof(PWM));
    PWM_L->next = NULL;
}

void PWM_Update()
{
    PWM *p = PWM_L->next;
    while (p != NULL)
    {
        p->cnt++;
        if (p->limit_high == 0)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_RESET);
            p->cnt = 0;
        }
        else if (p->limit_low == 0)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_SET);
            p->cnt = 0;
        }
        else if (p->cnt == p->limit_low && p->status == 0)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_SET);
            p->status = 1;
            p->cnt = 0;
        }
        else if (p->cnt == p->limit_high && p->status == 1)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_RESET);
            p->status = 0;
            p->cnt = 0;
        }
        p = p->next;
    }
}