/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      General limit integral saturation pid controller  
 * @changelog:
 * date         author          notes
 * 2021.10.19   wangchongwei    first version   
 **/
#ifndef  _PID_H_
#define  _PID_H_

#ifdef  __cplusplus
extern  "C"  {
#endif

#include <string.h>

struct PID
{
    void (*setCtrlParm)(struct PID *that, float kp, float ki, float kd);
    void (*setIntegralParm)(struct PID *that, float up, float low);

    float (*p  )(struct PID *that, float err);
    float (*pi )(struct PID *that, float err);
    float (*pid)(struct PID *that, float err);

    float  i_err;
    float  o_out;

    struct  
    {
        float  m_kp;
        float  m_ki;
        float  m_kd;

        float  m_integral_up;
        float  m_integral_low;
        float  m_sum;
        float  m_last_err;
    }pvt;
};

//  Object  construction
struct PID * PID_Constructor(struct PID *that);

#ifdef  __cplusplus
}
#endif

#endif  /*_PID_H_*/
