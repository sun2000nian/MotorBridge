#ifndef _MOTOR_HPP
#define _MOTOR_HPP
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "PWM.hpp"
#include "PID.hpp"
#include <string>
#include <stdio.h>
#pragma pack(4)

#define CAL_SPEED_LEN 40
#define CAL_SPEED_SEGLEN 25
#define SPEED_LIMIT 150
#define SPEED_IGNORE_VALUE 3
#define VOLTAGE_TOGGLE_SPEED 10

class MOTOR
{
    // OnPosPID=>>10ms
    // OnSpeedPID=>>20ms
private:
    uint16_t IN_A_PIN, IN_B_PIN, OUT_A_PIN, OUT_B_PIN;
    GPIO_TypeDef *IN_A_PORT, *IN_B_PORT, *OUT_A_PORT, *OUT_B_PORT;
    char state, newState, move;
    int position, positionTarget;
    osThreadId_t CalSpeedTaskHandle, SpeedPIDTaskHandle, PosPIDTaskHandle;
    osThreadAttr_t CalSpeedTask_attributes, SpeedPIDTask_attributes, PosPIDTask_attributes;

    int speedData[CAL_SPEED_LEN];
    int speedSUM;
    int datapos, lastpos;
    int temp, dataerr;

public:
    std::string name;
    PWM *pwm_A, *pwm_B;
    int speed, speedTarget; //速度
    float posPIDresult, speedPIDresult;

    PID posPID;   //位置PID
    PID speedPID; //速度PID
    MOTOR(
        std::string name,
        GPIO_TypeDef *IN_A_PORT,
        uint16_t IN_A_PIN,
        GPIO_TypeDef *IN_B_PORT,
        uint16_t IN_B_PIN,
        GPIO_TypeDef *OUT_A_PORT,
        uint16_t OUT_A_PIN,
        GPIO_TypeDef *OUT_B_PORT,
        uint16_t OUT_B_PIN);

    //设置电压，范围=-100 to 100，可超限
    void SetVoltage(float v);

    //编码器脉冲中断
    void OnEdge();                  //直接更新位置
    void OnEdge(uint16_t GPIO_pin); //传参，外部中断引脚

    void OnPosPID();   //位置PID更新
    void OnSpeedPID(); //速度PID更新

    int Position();                  //返回位置
    float Speed();                   //返回速度
    void SetPosition(int pos);       //设置位置
    void SetSpeed(float speed);      //设置速度
    void CalSpeed(std::string name); //计算速度-多线程任务函数
    ~MOTOR();
};

#endif
