#include "PID.hpp"
#pragma pack(4)

PID::PID(float Kp, float Ki, float Kd, int integralLIMIT)
{
    this->Kd = Kd;
    this->Ki = Ki;
    this->Kp = Kp;
    this->integralLIMIT = integralLIMIT;
    aimValue = 0.0;
    err = 0.0;
    err_last = 0.0;
}

PID::PID()
{
    aimValue = 0.0;
    err = 0.0;
    err_last = 0.0;
    integralLIMIT = 5000;
}

PID::~PID()
{
}

void PID::SetAttr(float Kp, float Ki, float Kd, int integralLIMIT)
{
    this->Kd = Kd;
    this->Ki = Ki;
    this->Kp = Kp;
    this->integralLIMIT = integralLIMIT;
    aimValue = 0.0;
    err = 0.0;
    err_last = 0.0;
}

float PID::Update(float result)
{
    err = aimValue - result;
    integral += err;
    if (integral > (float)integralLIMIT)
        integral = (float)integralLIMIT;
    else if (integral < -(float)integralLIMIT)
        integral = -(float)integralLIMIT;
    if((err>0&&integral>0)||(err<0&&integral<0))
    		integral=0;
    returnValue = Kp * err + Ki * integral + Kd * (err - err_last);
    err_last = err;
    return returnValue;
}

void PID::SetAIM(float aim)
{
    aimValue = aim;
}
