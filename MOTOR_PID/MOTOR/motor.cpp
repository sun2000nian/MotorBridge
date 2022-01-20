#include "motor.hpp"
#include <stdio.h>
#include "PID.hpp"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
char states[] = {0, 1, 3, 2};
int result = 0, pos = 0, lastpos = 0;
int tim3_cnt = 0, edgecnt = 0;

MOTOR motor(GPIOD, GPIO_PIN_0, GPIOD, GPIO_PIN_1, GPIOB, GPIO_PIN_1, GPIOB, GPIO_PIN_2);
PID posPID(2, 0.03, 5), speedPID(1, 0.01, 1);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
    motor.OnEdge();
    // if (tim3_cnt == 0)
    // {
    //     edgecnt++;
    // }
    // else
    // {
    //     motor.speed = 1000.0 / tim3_cnt;
    //     printf("speed=%f\n", motor.speed);
    //     tim3_cnt = 0;
    // }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2)
    {
        PWM_Update();
    }
    else if (htim == &htim3)
    {
        //1ms
        //updatePID
        pos = motor.Position();
        result = posPID.Update(pos);
        motor.SetVoltage(result);

        // if (edgecnt == 0)
        // {
        //     tim3_cnt++;
        //     if (tim3_cnt == 1000)
        //         motor.speed = 0;
        // }
        // else
        // {
        //     motor.speed = 1000.0 * edgecnt;
        //     printf("speed=%f\n", motor.speed);
        //     edgecnt = 0;
        // }
    }
}

void MOTOR::SetPosition(int pos)
{
    posPID.SetAIM(pos);
}

void MOTOR::SetSpeed(int speed)
{
    speedPID.SetAIM(speed);
}

void MOTOR::OnEdge()
{
    static char newState, move;
    newState = HAL_GPIO_ReadPin(this->IN_A_PORT, this->IN_A_PIN) * 2 +
               HAL_GPIO_ReadPin(this->IN_B_PORT, this->IN_B_PIN);
    newState = states[newState];
    //printf("STATE=%d\n", newState);
    move = (newState - state + 4) % 4;
    if (move == 1)
    {
        position++;
    }
    else if (move == 3)
    {
        position--;
    }
    else if (move == 0)
    {
        printf("///SAME,new=%d,old=%d\n", newState, state);
    }
    else
    {
        printf("///ERR,new=%d,old=%d\n", newState, state);
    }
    state = newState;
    //printf("pos=%d\n", position);
}

int MOTOR::Position()
{
    return position;
}
int MOTOR::Speed()
{
    return speed;
}

void MOTOR::SetVoltage(float v)
{
    if (v > 100)
        v = 100;
    else if (v < -100)
        v = -100;
    if (v >= 0)
    {
        PWM_SetFreq(this->OUT_A_PORT, this->OUT_A_PIN, v, 100);
        PWM_SetFreq(this->OUT_B_PORT, this->OUT_B_PIN, 0, 100);
    }
    else
    {
        PWM_SetFreq(this->OUT_A_PORT, this->OUT_A_PIN, 0, 100);
        PWM_SetFreq(this->OUT_B_PORT, this->OUT_B_PIN, -v, 100);
    }
}

MOTOR::MOTOR(
    GPIO_TypeDef *IN_A_PORT,
    uint16_t IN_A_PIN,
    GPIO_TypeDef *IN_B_PORT,
    uint16_t IN_B_PIN,
    GPIO_TypeDef *OUT_A_PORT,
    uint16_t OUT_A_PIN,
    GPIO_TypeDef *OUT_B_PORT,
    uint16_t OUT_B_PIN)
{
    this->IN_A_PORT = IN_A_PORT;
    this->IN_B_PORT = IN_B_PORT;
    this->OUT_A_PORT = OUT_A_PORT;
    this->OUT_B_PORT = OUT_B_PORT;

    this->IN_A_PIN = IN_A_PIN;
    this->IN_B_PIN = IN_B_PIN;
    this->OUT_A_PIN = OUT_A_PIN;
    this->OUT_B_PIN = OUT_B_PIN;

    this->state =
        HAL_GPIO_ReadPin(this->IN_A_PORT, this->IN_A_PIN) * 2 +
        HAL_GPIO_ReadPin(this->IN_B_PORT, this->IN_B_PIN);
    this->state = states[this->state];
    position = 0;
    posPID.SetAIM(0);
    // pwm_A = (PWM *)malloc(sizeof(PWM));
    // pwm_B = (PWM *)malloc(sizeof(PWM));
    // PWM_Init_Node(pwm_A, OUT_A_PIN, OUT_A_PORT, 0, 100);
    // PWM_Init_Node(pwm_B, OUT_B_PIN, OUT_B_PORT, 0, 100);
}

MOTOR::~MOTOR()
{
}
