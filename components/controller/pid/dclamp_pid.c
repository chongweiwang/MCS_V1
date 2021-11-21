/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      anti-windup pid,dynamic clamping Integrator
 * @changelog:
 * date         author          notes
 * 2021.11.20   wangchongwei    dynamic clamping Integrator     
 **/

#include "dclamp_pid.h"

#define own (that->pvt)

/**
 * set controller parm
 * */
static void _setCtrParm(struct DClampPID *that, float kp, float ki, float kd)
{
    own.m_kp = kp;
    own.m_ki = ki;
    own.m_kd = kd;
}

/**
 * set clamp parm
 * */
static void _setClampParm(struct DClampPID *that, float out_up, float out_low)
{
    own.m_out_up = out_up;
    own.m_out_low = out_low;
}


/**
 *  P controller
 * */
static float _p(struct DClampPID *that, float err)
{
    that->i_err = err;

    own.m_pre_out = that->i_err * own.m_kp;
    that->o_out = own.m_pre_out;


    /*output clamp*/
    if (that->o_out > own.m_out_up)
    {
        that->o_out = own.m_out_up;
    }
    else if (that->o_out < own.m_out_low)
    {
        that->o_out = own.m_out_low;
    }

    return that->o_out;
}

/**
 *  PI controller
 * */
static float _pi(struct DClampPID *that, float err)
{
    that->i_err = err;

    own.m_pre_out = that->i_err * own.m_kp + own.m_sum;

    that->o_out = own.m_pre_out;

    /*output clamp*/
    if (that->o_out > own.m_out_up)
    {
        that->o_out = own.m_out_up;
    }
    else if (that->o_out < own.m_out_low)
    {
        that->o_out = own.m_out_low;
    }

    /*dynamic Integrator clamp*/
    own.m_sum += that->i_err * own.m_ki + that->o_out - own.m_pre_out ;

    return that->o_out;
}

/**
 *  PID controller
 * */
static float _pid(struct DClampPID *that, float err)
{
    that->i_err = err;

    own.m_pre_out = that->i_err * own.m_kp + own.m_sum + own.m_kd*(that->i_err - own.m_last_err);;

    that->o_out = own.m_pre_out;

    /*output clamp*/
    if (that->o_out > own.m_out_up)
    {
        that->o_out = own.m_out_up;
    }
    else if (that->o_out < own.m_out_low)
    {
        that->o_out = own.m_out_low;
    }

    /*dynamic Integrator clamp*/
    own.m_sum += that->i_err * own.m_ki + that->o_out - own.m_pre_out ;

    that->pvt.m_last_err = that->i_err;

    return that->o_out;
}

/**
 *  Constructor function
 * */ 
struct DClampPID * DClampPID_Constructor(struct DClampPID *that)
{
    memset(that , 0 , sizeof(struct DClampPID));

    that->setCtrlParm = _setCtrParm;
    that->setClampParm = _setClampParm;

    that->p = _p;
    that->pi = _pi;
    that->pid = _pid;

    return that;
}
