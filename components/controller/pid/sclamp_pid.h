/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      anti-windup pid,static clamping Integrator
 * @changelog:
 * date         author          notes
 * 2021.11.20   wangchongwei    static clamping Integrator     
 **/
#ifndef  _SCLAMP_PID_H_
#define  _SCLAMP_PID_H_

#ifdef  __cplusplus
extern  "C"  {
#endif

#include <string.h>

struct SClampPID
{
    void (*setCtrlParm)(struct SClampPID *that, float kp, float ki, float kd);
    void (*setClampParm)(struct SClampPID *that, float out_up, float out_low, float integral_up,float integral_low);

    float (*p  )(struct SClampPID *that, float err);
    float (*pi )(struct SClampPID *that, float err);
    float (*pid)(struct SClampPID *that, float err);

    float  i_err;
    float  o_out;

    struct  
    {
        float  m_kp;
        float  m_ki;
        float  m_kd;

        float  m_out_up;
        float  m_out_low;

        float  m_integral_up;
        float  m_integral_low;

        float  m_sum;
        float  m_last_err;

        float  m_pre_out;
    }pvt;
};

//  Object  construction
struct SClampPID * SClampPID_Constructor(struct SClampPID *that);

#ifdef  __cplusplus
}
#endif

#endif  /*SClampPID*/
