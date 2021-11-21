/**
 * @copyright   Copyright 2021 zerodrive wangchongwei.
 * @license:    Apache-2.0 License  
 * @brief:      General limit integral saturation pid controller  
 * @changelog:
 * date         author          notes
 * 2021.10.19   wangchongwei    first version: base Discretized pid
 * 2021.11.20   wangchongwei    Modify integral limit
 **/

#include "pid.h"

#define own (that->pvt)


// 设置控制参数
static void _setCtrParm(struct PID *that, float kp, float ki, float kd)
{
    that->pvt.m_kp = kp;
    that->pvt.m_ki = ki;
    that->pvt.m_kd = kd;
}

// 设置积分限幅
static void _setIntegralParm(struct PID *that, float integral_up, float integral_low)
{
    that->pvt.m_integral_up = integral_up;
    that->pvt.m_integral_low = integral_low;
}

// 比例控制器
static float _p(struct PID *that, float err)
{
    that->i_err = err;
    that->o_out = that->i_err * that->pvt.m_kp;

    return that->o_out;
}

// 比例积分控制器
static float _pi(struct PID *that, float err)
{
    that->i_err = err;
    // m_kp*err+m_ki*err_m_sum
    that->o_out = that->pvt.m_kp * that->i_err + own.m_sum;

    own.m_sum += that->i_err * own.m_ki;
    if (own.m_sum > own.m_integral_up)
    {
        own.m_sum = own.m_integral_up;
    }
    else if (own.m_sum < own.m_integral_low)
    {
        own.m_sum = own.m_integral_low;
    }

    return that->o_out;
}

// PID控制器
static float _pid(struct PID *that, float err)
{
    that->i_err = err;
    
    that->o_out = own.m_kp * that->i_err + own.m_sum + 
                own.m_kd*(that->i_err - own.m_last_err);

    // 误差积分
    own.m_sum += that->i_err * own.m_ki;
    if (own.m_sum > own.m_integral_up)
    {
        own.m_sum = own.m_integral_up;
    }
    else if (own.m_sum < own.m_integral_low)
    {
        own.m_sum = own.m_integral_low;
    }

    // 记录上次误差
    that->pvt.m_last_err = that->i_err;

    return that->o_out;
}

// 构造函数将接口绑定
struct PID * PID_Constructor(struct PID *that)
{
    memset(that , 0 , sizeof(struct PID));

    that->setCtrlParm = _setCtrParm;
    that->setIntegralParm = _setIntegralParm;

    that->p = _p;
    that->pi = _pi;
    that->pid = _pid;

    return that;
}
