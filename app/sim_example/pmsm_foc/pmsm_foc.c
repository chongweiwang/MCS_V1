/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:     pmsm_foc demo
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.07   wangchongwei    create file 
 **/
#include "pmsm_foc.h"

#define own (that->pvt)

static void _init(struct PmsmFoc *that)
{
    // init parm
    own.m_Pmsm.c_B = 0.0001;
    own.m_Pmsm.c_R = 0.21;
    own.m_Pmsm.c_Lq = 0.0008;
    own.m_Pmsm.c_Ld = 0.0008;
    own.m_Pmsm.c_Pn = 2;
    own.m_Pmsm.c_psi = 0.034;
    own.m_Pmsm.i_J = 0.0119;

    own.m_Pmsm.c_rated_cur = 5.9;
    own.m_Pmsm.c_rated_pow = 100;
    own.m_Pmsm.c_rated_tor = 0.32;
    own.m_Pmsm.c_rated_vol = 24;
    own.m_Pmsm.c_rated_vel = 3000;

    own.m_Pmsm.init(&own.m_Pmsm);

    // inverter parm 
    own.m_Inverter.i_busVol = own.m_Pmsm.c_rated_vol;

    // init  pid parm
    //own.m_Id_pid.setCtrlPrm(&own.m_Id_pid,own.m_Pmsm.c_Lq*80,own.m_Pmsm.c_R,0);
    own.m_Id_pid.setCtrlPrm(&own.m_Id_pid,0.5,0.003,0);
    own.m_Id_pid.setIntegralPrm(&own.m_Id_pid, 200, -200);

    //own.m_Iq_pid.setCtrlPrm(&own.m_Iq_pid,own.m_Pmsm.c_Lq*80,own.m_Pmsm.c_R,0);
    own.m_Iq_pid.setCtrlPrm(&own.m_Iq_pid,0.5,0.03,0);
    own.m_Iq_pid.setIntegralPrm(&own.m_Iq_pid, 200, -200);

    own.m_rpm_pid.setCtrlPrm(&own.m_rpm_pid,4,0.02,0);
    own.m_rpm_pid.setIntegralPrm(&own.m_rpm_pid,10,-10);

    own.m_pos_pid.setCtrlPrm(&own.m_pos_pid,4,0,0);

    // config wave info
    that->setChName(&that->m_obj,0,"rpm",strlen("rpm"));
    that->setChName(&that->m_obj,1,"Te,N*m",strlen("Te,N*m"));
    that->setChName(&that->m_obj,2,"thetaR",strlen("thetaR"));
    that->setChName(&that->m_obj,3,"thetaE",strlen("thetaE"));
    that->setChName(&that->m_obj,4,"Id",strlen("Id"));
    that->setChName(&that->m_obj,5,"Iq",strlen("Iq"));
    that->setChName(&that->m_obj,6,"Ua",strlen("Ua"));
}


/**
 * current loop input
 * 1. Electrical angle  电角度
 * 2. Target dq current    目标dq电流
 * 3. current           3or2 current
*/
/**
 * current loop output
 * 1. pwm -> inverter  pwm到逆变器
 * */
static void _currentLoop(struct PmsmFoc *that)
{
    /*1. get electrical angle -> sin cos,一般这是查表*/
    double sint = sin(own.Tor.i_ele_angle);
    double cost = cos(own.Tor.i_ele_angle);

    /*2. cur ab(c) -> alpha beta -> dq*/
    ab_clark_amp(own.Tor.i_cur[0], own.Tor.i_cur[1], &own.Tor.m_cur_alpha, &own.Tor.m_cur_beta);
    park(own.Tor.m_cur_alpha, own.Tor.m_cur_beta, sint, cost, &own.Tor.m_Id, &own.Tor.m_Iq);

    /*3. control iq id=0 -> get Vq Vd*/
    own.Tor.o_Vq_pu = own.m_Iq_pid.pi(&own.m_Iq_pid, own.Tor.i_ref_Iq_pu - own.Tor.m_Iq);
    own.Tor.o_Vd_pu = own.m_Id_pid.pi(&own.m_Id_pid, own.Tor.i_ref_Id_pu - own.Tor.m_Id);

    /*4. Vq Vd -> svpwm -> abc pwm*/
    inv_park(own.Tor.o_Vd_pu, own.Tor.o_Vq_pu,sint, cost,&own.Tor.m_vol_alpha,&own.Tor.m_vol_beta);

    // 这应该用svpwm
    inv_clark_amp(own.Tor.m_vol_alpha,own.Tor.m_vol_beta,&own.Tor.o_Ua_pu, &own.Tor.o_Ub_pu, &own.Tor.o_Uc_pu);
}


/**
 * speed loop input
 * 1. Target vel    目标速度
 * 2. feedback      反馈速度
 */
/**
 * speed loop output
 * 1. tor control ctrl  力矩(电流)控制的输入
 * */
static void _speedLoop(struct PmsmFoc *that)
{
    #define  LIMIT(x,max,min)  (x=(x>=max?max:(x<=min?min:x)))

    own.Vel.o_tor_ctrl_pu =  own.m_rpm_pid.pi(&own.m_rpm_pid, own.Vel.i_ref_rmp - own.Vel.i_rpm_fb);

    LIMIT(own.Vel.o_tor_ctrl_pu,1,-1);
    
}
/** position loop input
 * 1. Target position
 * 2. feedback
 * */
/** speed loop output
 * 1. vel control ctrl
 * */
static void _positionLoop(struct PmsmFoc *that)
{
    #define  LIMIT(x,max,min)  (x=(x>=max?max:(x<=min?min:x)))

    own.Pos.o_vel_ctrl_pu = own.m_pos_pid.p(&own.m_pos_pid, own.Pos.i_ref_pos - own.Pos.i_fb_pos);

}

/**
 *  simulation dc brush control
*/
static void _simTask(struct PmsmFoc *that)
{
    #define CTRL_PERIOD_US  50

    double hs = that->getTimeHs(&that->m_obj);   /* 步长*/
    double ms = that->getTimeMs(&that->m_obj);
    double us = that->getTimeUs(&that->m_obj);
    static double last_us = 0;

    /*控制模式、控制量的设置*/
    own.Ctrl.c_ctrl_mode = PMSM_FOC_CTRL_VEL;
    own.Ctrl.c_vel = 50;
    own.Ctrl.c_pos = 20;
    own.Ctrl.c_iq = 1;
    own.Ctrl.c_id = 0;

    /*control task*/
    if (us-last_us >= CTRL_PERIOD_US)
    {
        last_us = us;

        /* 控制命令选择，环路输入输出选择*/
        switch(own.Ctrl.c_ctrl_mode)
        {
            case PMSM_FOC_CTRL_TUR: 
                own.Tor.i_ref_Id_pu = own.Ctrl.c_id;            /* 目标D轴电流*/
                own.Tor.i_ref_Iq_pu = own.Ctrl.c_iq;            /* 目标Q轴电流*/
            break;
            case PMSM_FOC_CTRL_VEL: 
                own.Tor.i_ref_Id_pu = 0;
                own.Tor.i_ref_Iq_pu = own.Vel.o_tor_ctrl_pu;    /* 速度环控制量->电流环q轴目标*/
                own.Vel.i_ref_rmp   = own.Ctrl.c_vel;           /* 速度指令*/
            break;
            case PMSM_FOC_CTRL_POS: 
                own.Tor.i_ref_Id_pu = 0;
                own.Tor.i_ref_Iq_pu = own.Vel.o_tor_ctrl_pu;    /* 速度环控制量->电流环q轴目标*/
                own.Vel.i_ref_rmp   = own.Pos.o_vel_ctrl_pu;    /* 位置环控制量->速度环目标*/
                own.Pos.i_ref_pos   = own.Ctrl.c_pos;           /* 位置指令*/
            break;
        }

        /*current loop */
        own.Tor.i_ele_angle = own.m_Pmsm.o_thetaE;      /* 电角度 */
        own.Tor.i_cur[0] = own.m_Pmsm.o_Ia;             /* a相电流*/
        own.Tor.i_cur[1] = own.m_Pmsm.o_Ib;             /* b相电流*/
        own.Tor.i_cur[2] = own.m_Pmsm.o_Ic;             /* c相电流*/

        _currentLoop(that);

        /*speed loop*/
        own.Vel.i_rpm_fb = own.m_Pmsm.o_rpm;
        _speedLoop(that);

        /*positon loop*/
        own.Pos.i_fb_pos  = own.m_Pmsm.o_thetaR;
        own.Pos.i_ref_pos = own.Ctrl.c_pos;
        _positionLoop(that);


        /*add wave*/
        that->addWave(&that->m_obj,0,own.m_Pmsm.o_rpm);
        that->addWave(&that->m_obj,1,own.m_Pmsm.o_Te);
        that->addWave(&that->m_obj,2,own.m_Pmsm.o_thetaR);
        that->addWave(&that->m_obj,3,own.m_Pmsm.o_thetaE);
        that->addWave(&that->m_obj,4,own.Tor.m_Id);
        that->addWave(&that->m_obj,5,own.Tor.m_Iq);
        that->addWave(&that->m_obj,6,own.Tor.o_Ua_pu);
    }


    /*  Inverter */
    own.m_Inverter.i_Ua_pu = own.Tor.o_Ua_pu;
    own.m_Inverter.i_Ub_pu = own.Tor.o_Ub_pu;
    own.m_Inverter.i_Uc_pu = own.Tor.o_Uc_pu;

    own.m_Inverter.simulation(&own.m_Inverter,us,hs);


    /*pmsm simulation*/
    own.m_Pmsm.i_Ua = own.m_Inverter.o_Ua;
    own.m_Pmsm.i_Ub = own.m_Inverter.o_Ub;
    own.m_Pmsm.i_Uc = own.m_Inverter.o_Uc;

    own.m_Pmsm.i_J = 0.0119;
    own.m_Pmsm.i_TL = 0.02;

    own.m_Pmsm.simulation(&own.m_Pmsm,us,hs);
}



// 仿真环境的构造函数
struct PmsmFoc * PmsmFoc_Constructor(struct PmsmFoc *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct PmsmFoc));

    that->init = _init;
    that->simTask = _simTask;

    // 子对象，组合对象的构造
    BaseObj_Constructor(&that->m_obj);

    Pmsm_Constructor(&own.m_Pmsm); 

    Inverter_Constructor(&own.m_Inverter);

    PID_Constructor(&own.m_Id_pid);
    PID_Constructor(&own.m_Iq_pid);
    PID_Constructor(&own.m_rpm_pid);
    PID_Constructor(&own.m_pos_pid);

    return that;
}
