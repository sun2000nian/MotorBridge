#include "PWM.hpp"
#define PWM_TIMER_CNT 10000
PWM *PWM_L;
PWM *p, *q, *r;

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
    q = PWM_L;
    while (q->next != NULL)
    {
        if (q->next->pin == pin && q->next->port == port)
            break;
        q = q->next;
    }
    if (q->next == NULL)
    {
        r = (PWM *)malloc(sizeof(PWM));
        PWM_Init_Node(r, pin, port, percent, freq);
        q->next = r;
    }
    else
    {
        q = q->next;
        q->limit = (unsigned int)PWM_TIMER_CNT / freq;
        q->limit_high = q->limit * percent / 100;
        q->limit_low = q->limit - q->limit_high;
        // p->cnt = 0;
        // p->status = 0;
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
    p = PWM_L->next;
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
        else if (p->cnt >= p->limit_low && p->status == 0)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_SET);
            p->status = 1;
            p->cnt = 0;
        }
        else if (p->cnt >= p->limit_high && p->status == 1)
        {
            HAL_GPIO_WritePin(p->port, p->pin, GPIO_PIN_RESET);
            p->status = 0;
            p->cnt = 0;
        }
        p = p->next;
    }
}
