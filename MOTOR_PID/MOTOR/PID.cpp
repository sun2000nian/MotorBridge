#include "PID.hpp"

PID::PID(double Kp, double Ki, double Kd)
{
    this->Kd = Kd;
    this->Ki = Ki;
    this->Kp = Kp;
    aimValue = 0.0;
    err = 0;
    err_last = 0;
}

PID::~PID()
{
}

double PID::Update(double result)
{
    err = aimValue - result;
    integral += err;
    if (integral > 5000)
        integral = 5000;
    else if (integral < -5000)
        integral = -5000;
    returnValue = Kp * err + Ki * integral + Kd * (err - err_last);
    err_last = err;
    return returnValue;
}

void PID::SetAIM(double aim)
{
    aimValue = aim;
}
