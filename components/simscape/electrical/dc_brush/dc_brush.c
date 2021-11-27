/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Mathematical Model of DC Brushed Motor
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.09.28   wangchongwei    first version   
 **/

#include "dc_brush.h"

#define own that->pvt

/**
 * @brief: pmsm mathematical model 
 * @details: Differential equation:
 *  di/dt = (u-R*i-ke*w)/L
 *  dw/dt = (Kt*i -TL)/J
 *  dtheta/dt = w
 *  dx[i,w,theta]  x[i,w,theta]
*/
static void dynamic(struct DcBrush *that, float *dx, float *x, uint32_t size)
{
    dx[0] = (that->i_Ua - that->c_R*x[0] - that->c_psi*x[1])/ that->c_L;
    that->o_Te = that->c_Kt*x[0];
    dx[1] = (that->o_Te - that->i_TL)/ that->i_J;
    dx[2] = x[1];
}


static void _init(struct DcBrush *that)
{

}

#define pi 3.141592
static void _simulation(struct DcBrush *that,float t, float hs)
{
    
    // 1. simulation, solve differential equations
    own.m_rk4.rk4_solve(&own.m_rk4,t,hs,own.m_dx,own.m_x,own.m_size);

    // 2. get output I omega theta rpm
    that->o_Ia   = own.m_x[0];
    that->o_omega = own.m_x[1];
    that->o_rpm    = (30/pi)*that->o_omega;

    
    if (own.m_x[2]>2*pi) 
    {
        own.m_x[2] = own.m_x[2]-2*pi;
    } 
    else if (own.m_x[2]<0)
    {
         own.m_x[2] = own.m_x[2]+2*pi;
    }
    that->o_theta = own.m_x[2];

}


struct DcBrush *DcBrush_Constructor(struct DcBrush *that)
{
    // 1. that function bind
    memset(that , 0 , sizeof(struct DcBrush));
    that->simulation = _simulation;
    that->init = _init;

    // 2. Construct submodule
    RK4_Constructor(&own.m_rk4);

    // bind the dynamic function, and set size
    own.m_rk4.init(&own.m_rk4,that,&dynamic);
    own.m_size = 3; 

    return that;
}
