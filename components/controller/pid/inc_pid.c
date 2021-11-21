/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      Incremental pid controller
 * @changelog:
 * date         author          notes
 * 2021.11.21   wangchongwei    inc pid     
 **/

#include "inc_pid.h"

#define own (that->pvt)

/**
 * set controller parm
 * */
static void _setCtrParm(struct IncPID *that, float kp, float ki, float kd)
{
    own.m_kp = kp;
    own.m_ki = ki;
    own.m_kd = kd;
}

/**
 * set clamp parm
 * */
void _setClampParm(struct IncPID *that, float out_up, float out_low)
{
    own.m_out_up = out_up;
    own.m_out_low = out_low;
}


/**
 *  P controller
 * */
static float _p(struct IncPID *that, float err)
{
    that->i_err = err;

    own.m_delta_out = own.m_kp*(that->i_err - own.m_last_err);

    own.m_pre_out = own.m_delta_out + own.m_sum;
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

    own.m_sum = that->o_out;

    own.m_last_err = that->i_err;

    return that->o_out;
}

/**
 *  PI controller
 * */
static float _pi(struct IncPID *that, float err)
{
    that->i_err = err;

    own.m_delta_out = own.m_kp*(that->i_err - own.m_last_err) + own.m_ki*that->i_err;

    own.m_pre_out = own.m_delta_out + own.m_sum;
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
    
    own.m_sum = that->o_out;

    own.m_last_err = that->i_err;

    return that->o_out;
}

/**
 *  PID controller
 * */
static float _pid(struct IncPID *that, float err)
{
    that->i_err = err;

    own.m_delta_out =   own.m_kp*(that->i_err - own.m_last_err) + 
                        own.m_ki*that->i_err+
                        own.m_kd*(that->i_err -2*own.m_last_err + own.m_last_pre_err);

    own.m_pre_out = own.m_delta_out + own.m_sum;
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
    
    own.m_sum = that->o_out;

    own.m_last_err = that->i_err;
    own.m_last_pre_err = own.m_last_err;

    return that->o_out;
}

/**
 *  Constructor function
 * */ 
struct IncPID * IncPID_Constructor(struct IncPID *that)
{
    memset(that , 0 , sizeof(struct IncPID));

    that->setCtrlParm = _setCtrParm;
    that->setClampParm = _setClampParm;

    that->p = _p;
    that->pi = _pi;
    that->pid = _pid;

    return that;
}
