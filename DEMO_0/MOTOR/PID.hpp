#ifndef PID_PID_HPP_
#define PID_PID_HPP_

#pragma pack(4)
class PID
{
private:
    float aimValue;
    float returnValue;
    float err, err_last;

    float Kp, Ki, Kd;
    float integral;
    int integralLIMIT;

public:
    PID(float Kp, float Ki, float Kd, int integralLIMIT);
    PID();
    ~PID();
    void SetAttr(float Kp, float Ki, float Kd, int integralLIMIT);
    float Update(float result);
    void SetAIM(float aim);
};

#endif /* PID_PID_HPP_ */
