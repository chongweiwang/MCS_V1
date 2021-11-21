/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      back-calculation
 * @details:  kc:0.3~3*Ki/Kp  default:Ki/Kp  
 * @changelog:
 * date         author          notes
 * 2021.11.20   wangchongwei    back-calculation    
 **/
#ifndef  _BACK_CAL_PID_H_
#define  _BACK_CAL_PID_H_

#ifdef  __cplusplus
extern  "C"  {
#endif

#include <string.h>

struct BackCalPID
{
    void (*setCtrlParm)(struct BackCalPID *that, float kp, float ki, float kd);
    void (*setKcParm)(struct BackCalPID *that, float kc);
    void (*setClampParm)(struct BackCalPID *that, float out_up, float out_low);

    float (*p  )(struct BackCalPID *that, float err);
    float (*pi )(struct BackCalPID *that, float err);
    float (*pid)(struct BackCalPID *that, float err);

    float  i_err;
    float  o_out;

    struct  
    {
        float  m_kp;
        float  m_ki;
        float  m_kd;
        float  m_kc;

        float  m_out_up;
        float  m_out_low;

        float  m_sum;
        float  m_last_err;
        float  m_pre_out;
    }pvt;
};

//  Object  construction
struct BackCalPID * BackCalPID_Constructor(struct BackCalPID *that);

#ifdef  __cplusplus
}
#endif

#endif  /*_BACK_CAL_PID_H_*/
