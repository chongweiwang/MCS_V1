/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      Incremental pid controller
 * @changelog:
 * date         author          notes
 * 2021.11.21   wangchongwei    inc pid     
 **/
#ifndef  _INC_PID_H_
#define  _INC_PID_H_

#ifdef  __cplusplus
extern  "C"  {
#endif

#include <string.h>

struct IncPID
{
    void (*setCtrlParm)(struct IncPID *that, float kp, float ki, float kd);
    void (*setClampParm)(struct IncPID *that, float out_up, float out_low);

    float (*p  )(struct IncPID *that, float err);
    float (*pi )(struct IncPID *that, float err);
    float (*pid)(struct IncPID *that, float err);

    float  i_err;
    float  o_out;
 
    struct  
    {
        float  m_kp;
        float  m_ki;
        float  m_kd;

        float  m_out_up;
        float  m_out_low;

        float  m_delta_out;
        float  m_sum;

        float  m_last_pre_err;
        float  m_last_err;

        float  m_pre_out;
    }pvt;
};

//  Object  construction
struct IncPID * IncPID_Constructor(struct IncPID *that);

#ifdef  __cplusplus
}
#endif

#endif  /*_INC_PID_H_*/
