/**
 * @copyright Copyright 2021 zerodrive wangchongwei.
 * @license:    Apache-2.0 License  
 * @brief:  General limit integral saturation pid controller  
 * @changelog:
 * date         author          notes
 * 2021.10.19   wangchongwei    first version: base Discretized pid
 **/

#include "pid.h"

// 设置控制参数
static void _setCtrPrm(struct PID *that, float kp, float ki, float kd)
{
    that->pvt.m_kp = kp;
    that->pvt.m_ki = ki;
    that->pvt.m_kd = kd;
}

// 设置积分限幅
static void _setIntegralPrm(struct PID *that, float integral_up, float integral_low)
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
    that->o_out = that->pvt.m_kp * that->i_err + that->pvt.m_ki * that->pvt.m_sum;

    // 误差积分
    that->pvt.m_sum += that->i_err;

    // 积分限幅
    if (that->pvt.m_sum > that->pvt.m_integral_up) that->pvt.m_sum = that->pvt.m_integral_up;
    if (that->pvt.m_sum < that->pvt.m_integral_low) that->pvt.m_sum = that->pvt.m_integral_low;

    return that->o_out;
}

// PID控制器
static float _pid(struct PID *that, float err)
{
    that->i_err = err;
    
    that->o_out = that->pvt.m_kp * that->i_err + 
                that->pvt.m_ki * that->pvt.m_sum + 
                that->pvt.m_kd*(that->i_err - that->pvt.m_pre_err);

    // 误差积分
    that->pvt.m_sum += that->i_err;

    // 积分限幅
    if (that->pvt.m_sum > that->pvt.m_integral_up) that->pvt.m_sum = that->pvt.m_integral_up;
    if (that->pvt.m_sum < that->pvt.m_integral_low) that->pvt.m_sum = that->pvt.m_integral_low;

    // 记录上次误差
    that->pvt.m_pre_err = that->i_err;

    return that->o_out;
}

// 构造函数将接口绑定
struct PID * PID_Constructor(struct PID *that)
{
    memset(that , 0 , sizeof(struct PID));

    that->setCtrlPrm = _setCtrPrm;
    that->setIntegralPrm = _setIntegralPrm;

    that->p = _p;
    that->pi = _pi;
    that->pid = _pid;

    return that;
}
