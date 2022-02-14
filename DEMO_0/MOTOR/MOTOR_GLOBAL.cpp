#include "MOTOR_GLOBAL.hpp"
extern MOTOR motor;

//编码器外部中断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
    motor.OnEdge(GPIO_pin);
}

void GlobalPosPID(void *argument)
{
    while (1)
    {
         motor.OnPosPID();
        osDelay(10);
    }
}

void GlobalSpeedPID(void *argument)
{
    while (1)
    {
        motor.OnSpeedPID();
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10);
        osDelay(20);
    }
}

//计算速度-全局
void GlobalCalSpeed(const char *name)
{
    motor.CalSpeed(std::string(name));
}
