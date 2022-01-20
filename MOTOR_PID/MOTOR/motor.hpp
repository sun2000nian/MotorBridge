#ifndef _MOTOR_HPP
#define _MOTOR_HPP
#include "stm32f4xx_hal.h"
#include "PWM.hpp"

class MOTOR
{
private:
    uint16_t IN_A_PIN, IN_B_PIN, OUT_A_PIN, OUT_B_PIN;
    GPIO_TypeDef *IN_A_PORT, *IN_B_PORT, *OUT_A_PORT, *OUT_B_PORT;
    char state;
    long position;
    

public:
    PWM *pwm_A, *pwm_B;
    float speed;
    MOTOR(
        GPIO_TypeDef *IN_A_PORT,
        uint16_t IN_A_PIN,
        GPIO_TypeDef *IN_B_PORT,
        uint16_t IN_B_PIN,
        GPIO_TypeDef *OUT_A_PORT,
        uint16_t OUT_A_PIN,
        GPIO_TypeDef *OUT_B_PORT,
        uint16_t OUT_B_PIN);
    void SetVoltage(float v);
    void OnEdge();
    int Position();
    int Speed();
    void SetPosition(int pos);
    void SetSpeed(int speed);
    ~MOTOR();
};

struct MOTORnode
{
    int len;
    MOTOR *motor;
    struct MOTORnode *next;
};

void Init_Group();

#endif
