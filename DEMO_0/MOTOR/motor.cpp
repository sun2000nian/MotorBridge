#include "motor.hpp"
#include "MOTOR_GLOBAL.hpp"
#pragma pack(4)
// #define MOTOR_SHOW_POS_PID
//#define MOTOR_SHOW_SPEED_PID
// #define MOTOR_SHOW_POS

const char states[] = {0, 1, 3, 2};

MOTOR motor("M0", GPIOD, GPIO_PIN_0, GPIOD, GPIO_PIN_1, GPIOE, GPIO_PIN_0, GPIOE, GPIO_PIN_1);

//计算速度-类成员
void MOTOR::CalSpeed(std::string name)
{
    if (name != this->name)
        return;
    lastpos = position;
    datapos = 0;
    while (1)
    {
        temp = datapos - 1;
        if (temp == -1)
            temp = CAL_SPEED_LEN - 1;
        speedData[datapos] = position - lastpos;
        speedSUM += speedData[datapos] - speedData[temp];

        lastpos = position;
        speed = speedSUM;

        datapos++;
        if (datapos == CAL_SPEED_LEN)
            datapos = 0;
        osDelay(CAL_SPEED_SEGLEN);

#ifdef MOTOR_SHOW_SPEED
        if (speedSUM == 0)
            continue;
        printf("sum=%ld,speed=%lf\n", speedSUM, speed);
#endif
    }
}

//设置位置
void MOTOR::SetPosition(int pos)
{
    positionTarget = pos;
    posPID.SetAIM(pos);
}
//设置速度
void MOTOR::SetSpeed(float speed)
{
    if (speed > SPEED_LIMIT)
        speed = SPEED_LIMIT;
    if (speed < -SPEED_LIMIT)
        speed = -SPEED_LIMIT;
    speedTarget = speed;
    speedPID.SetAIM(speed);
}

//直接更新位置
void MOTOR::OnEdge()
{
    newState = ((char)HAL_GPIO_ReadPin(this->IN_A_PORT, this->IN_A_PIN)) * ((char)2) +
               ((char)HAL_GPIO_ReadPin(this->IN_B_PORT, this->IN_B_PIN));
    newState = states[(int)newState];
    move = (newState - state + ((char)4)) % ((char)4);
    state = newState;
    if (move == (char)1)
    {
        position--;
    }
    else if (move == (char)3)
    {
        position++;
    }
    else if (move == (char)0)
    {
#ifdef MOTOR_SHOW_POS
        printf("///SAME,new=%d,old=%d\n", newState, state);
#endif
    }

#ifdef MOTOR_SHOW_POS
    // printf("state=%d,pos=%d\n", state, position);
#endif
}
//传参，外部中断引脚
void MOTOR::OnEdge(uint16_t GPIO_pin)
{
    if (GPIO_pin != this->IN_A_PIN && GPIO_pin != this->IN_B_PIN)
        return;
    OnEdge();
}

//返回位置
int MOTOR::Position()
{
    return position;
}
//返回速度
float MOTOR::Speed()
{
    return speed;
}
//设置电压，范围=-100 to 100，可超限
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

//初始化
MOTOR::MOTOR(
    std::string name,
    GPIO_TypeDef *IN_A_PORT,
    uint16_t IN_A_PIN,
    GPIO_TypeDef *IN_B_PORT,
    uint16_t IN_B_PIN,
    GPIO_TypeDef *OUT_A_PORT,
    uint16_t OUT_A_PIN,
    GPIO_TypeDef *OUT_B_PORT,
    uint16_t OUT_B_PIN)
{
    //命名
    this->name = name;

    //复制引脚设置-PORT
    this->IN_A_PORT = IN_A_PORT;
    this->IN_B_PORT = IN_B_PORT;
    this->OUT_A_PORT = OUT_A_PORT;
    this->OUT_B_PORT = OUT_B_PORT;
    //复制引脚设置-PIN
    this->IN_A_PIN = IN_A_PIN;
    this->IN_B_PIN = IN_B_PIN;
    this->OUT_A_PIN = OUT_A_PIN;
    this->OUT_B_PIN = OUT_B_PIN;

    //更新初始位置
    this->state =
        ((char)HAL_GPIO_ReadPin(this->IN_A_PORT, this->IN_A_PIN)) * ((char)2) +
        ((char)HAL_GPIO_ReadPin(this->IN_B_PORT, this->IN_B_PIN));
    this->state = states[(int)this->state];

    // PID初始化
    posPID.SetAttr(0.3, 0.2, 1, 500);
    speedPID.SetAttr(0.5, 0.0, 0.2, 200);

    //位置=0
    position = 0;
    posPID.SetAIM(0);

    std::string TaskName;

    //启动测速任务
    TaskName = name;
    TaskName.append("_CALSPEED");
    CalSpeedTask_attributes.name = TaskName.c_str();
    CalSpeedTask_attributes.priority = (osPriority_t)osPriorityNormal;
    CalSpeedTaskHandle = osThreadNew((osThreadFunc_t)GlobalCalSpeed, (void *)this->name.c_str(), &CalSpeedTask_attributes);

    //启动Speed PID任务
    TaskName = name;
    TaskName.append("_SPEEDPID");
    SpeedPIDTask_attributes.name = TaskName.c_str();
    SpeedPIDTask_attributes.priority = (osPriority_t)osPriorityNormal;
    SpeedPIDTaskHandle = osThreadNew((osThreadFunc_t)GlobalSpeedPID, NULL, &SpeedPIDTask_attributes);

    //启动Position PID任务
    TaskName = name;
    TaskName.append("_POSPID");
    PosPIDTask_attributes.name = TaskName.c_str();
    PosPIDTask_attributes.priority = (osPriority_t)osPriorityNormal;
     PosPIDTaskHandle = osThreadNew((osThreadFunc_t)GlobalPosPID, NULL, &PosPIDTask_attributes);
}

MOTOR::~MOTOR()
{
    osThreadTerminate(CalSpeedTaskHandle);
     osThreadTerminate(SpeedPIDTaskHandle);
     osThreadTerminate(PosPIDTaskHandle);
}

void MOTOR::OnPosPID() //位置PID更新
{
    posPIDresult = posPID.Update((float)position);
    if (
        posPIDresult <= SPEED_IGNORE_VALUE &&
        posPIDresult >= -SPEED_IGNORE_VALUE &&
        position == positionTarget)
        posPIDresult = 0;
    // SetVoltage(posPIDresult);
    //待改为SetSpeed
    SetSpeed(posPIDresult);
#ifdef MOTOR_SHOW_POS_PID
    printf("posPID=%d,pos=%d\n", posPIDresult, position);
#endif
}

void MOTOR::OnSpeedPID() //速度PID更新
{
    speedPIDresult = speedPID.Update((float)speed);
    if((speed>VOLTAGE_TOGGLE_SPEED&&speedPIDresult<0)||
    		(speed<-VOLTAGE_TOGGLE_SPEED&&speedPIDresult>0)){
    	SetVoltage(0);
    }
    else
    	SetVoltage(speedPIDresult);
#ifdef MOTOR_SHOW_SPEED_PID
    printf("%f,%d\n", speedPIDresult, speed);
#endif
}
