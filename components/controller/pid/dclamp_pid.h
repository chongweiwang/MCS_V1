/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      anti-windup pid,dynamic  clamping Integrator
 * @changelog:
 * date         author          notes
 * 2021.11.20   wangchongwei    dynamic  clamping Integrator     
 **/
#ifndef  _DCLAMP_PID_H_
#define  _DCLAMP_PID_H_

#ifdef  __cplusplus
extern  "C"  {
#endif

#include <string.h>

struct DClampPID
{
    void (*setCtrlParm)(struct DClampPID *that, float kp, float ki, float kd);
    void (*setClampParm)(struct DClampPID *that, float out_up, float out_low);

    float (*p  )(struct DClampPID *that, float err);
    float (*pi )(struct DClampPID *that, float err);
    float (*pid)(struct DClampPID *that, float err);

    float  i_err;
    float  o_out;

    struct  
    {
        float  m_kp;
        float  m_ki;
        float  m_kd;

        float  m_out_up;
        float  m_out_low;

        float  m_sum;
        float  m_last_err;

        float  m_pre_out;
    }pvt;
};

//  Object  construction
struct DClampPID * DClampPID_Constructor(struct DClampPID *that);

#ifdef  __cplusplus
}
#endif

#endif  /*_DCLAMP_PID_H_*/
