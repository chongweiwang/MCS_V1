/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Permanent magnet synchronous motor, dq coordinate system modeling
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.14   wangchongwei    first version   
 **/

#include "pmsm.h"

#define own that->pvt 

/**
 * @brief: pmsm mathematical model 
 * @details: Differential equation:
 *  dId/dt = (Ud-RdId+we*LqIq)/Ld
 *  dIq/dt = (Uq-RqIq-we*LdId-we*Psi)/Lq
 *  Te = 3/2*Pn*Iq(Id(Ld-Lq)+Psi)
 *  dwr/dt = (Te-TL-Bwr)/J
 *  dthetaR/dt = wr
 *  we = Pn*wr
 *  dthetaE/dt = we
 *  rpm = (30/pi) *wr
 *
 *  dx[id,iq,dwr,dthetaR,dthetaE]  x[id,iq,wr,thetaR,thetaE]
*/
#define pi 3.141592
static void dynamic(struct Pmsm *that, double *dx, double *x, uint32_t size)
{
    dx[0] = (own.m_Ud - that->c_R*x[0] + that->o_omegaE*that->c_Lq*x[1]) / that->c_Ld;
    dx[1] = (own.m_Uq - that->c_R*x[1] - that->o_omegaE*that->c_Ld*x[0] - that->o_omegaE*that->c_psi)/that->c_Lq;
    that->o_Te = 1.5*that->c_Pn*x[1]*(x[0]*(that->c_Ld -that->c_Lq) + that->c_psi);
    dx[2] = (that->o_Te - that->i_TL -that->c_B*x[2])/that->i_J;
    dx[3] = x[2];
    that->o_omegaE = that->c_Pn*x[2];
    dx[4] = that->o_omegaE;
}

// can init some var
static void _init(struct Pmsm *that)
{

}

static void _simulation(struct Pmsm *that,double t, double hs)
{
    // 1.input vol abc->dq
    float st = sin(that->o_thetaE);
    float ct = cos(that->o_thetaE);

    abc2dq_amp(that->i_Ua,that->i_Ub,that->i_Uc,st,ct,&own.m_Ud,&own.m_Uq);

    // 2. simulation, solve differential equations
    own.m_rk4.rk4_solve(&own.m_rk4, t, hs, own.m_dx, own.m_x, own.m_size);
    
    // 3. get output I omega theta rpm
    own.m_Id = own.m_x[0];
    own.m_Iq = own.m_x[1];
    that->o_omegaR = own.m_x[2];
    that->o_thetaR = own.m_x[3];
    that->o_rpm = (30/pi) * that->o_omegaR;
    
    // 4. processing angle information
    if (own.m_x[4]>2*pi) 
    {
        own.m_x[4] = own.m_x[4]-2*pi;
    }
    else if(own.m_x[4]<0) 
    {
        own.m_x[4] = own.m_x[4]+2*pi;
    }

    that->o_thetaE = own.m_x[4];

    // 5. cur dq->abc
    st = sin(that->o_thetaE);
    ct = cos(that->o_thetaE);

    dq2abc_amp(own.m_Id,own.m_Iq,st,ct,&that->o_Ia,&that->o_Ib,&that->o_Ic);

}


struct Pmsm * Pmsm_Constructor(struct Pmsm *that)
{
    // 1. that function bind
    memset(that , 0 , sizeof(struct Pmsm));
    that->simulation = _simulation;
    that->init = _init;

    // 2. Construct submodule
    RK4_Constructor(&own.m_rk4);

    // bind the dynamic function, and set size
    own.m_rk4.init(&own.m_rk4,that,&dynamic);
    own.m_size = 5;

    return that; 
}
