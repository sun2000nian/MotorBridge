#ifndef PID_PID_HPP_
#define PID_PID_HPP_

class PID
{

private:
    double aimValue;
    double returnValue;
    double err, err_last;

    double Kp, Ki, Kd;
    double integral;

public:
    PID(double Kp, double Ki, double Kd);
    ~PID();
    double Update(double result);
    void SetAIM(double aim);
};

#endif /* PID_PID_HPP_ */
