/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  dc brush motor ctrl test 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.24   wangchongwei    dc brush motor ctrl test
 **/
#include "dc_brush_ctrl.h"

#define own (that->pvt)

static void _init(struct DcBrushCtrl *that)
{
    // init parm
    own.m_DcBrush.c_psi = 0.1;
    own.m_DcBrush.c_Kt  = 0.1;
    own.m_DcBrush.c_R   = 0.5;
    own.m_DcBrush.c_L   = 0.003;
    own.m_DcBrush.i_Ua  = 0;

    own.m_DcBrush.init(&own.m_DcBrush);

    // init  pid parm
    own.m_rpm_pid.setCtrlPrm(&own.m_rpm_pid,0.01,0.0003,0);
    own.m_rpm_pid.setIntegralPrm(&own.m_rpm_pid,300,-300);

    // config wave info
    that->setChName(&that->m_obj,0,"rpm",strlen("rpm"));
    that->setChName(&that->m_obj,1,"Te,N*m",strlen("Te,N*m"));
    that->setChName(&that->m_obj,2,"Ia,A",strlen("Ia,A"));
}

/**
 *  simulation dc brush control
*/
static void _simTask(struct DcBrushCtrl *that)
{
    double us = that->getTimeUs(&that->m_obj);
    double ms = that->getTimeMs(&that->m_obj);
    double hs = that->getTimeHs(&that->m_obj);
    static double last_us = 0;

    double rmp_cmd = 200;

    own.m_DcBrush.i_TL = 0.20;
    own.m_DcBrush.i_J  = 0.001;


    own.m_DcBrush.simulation(&own.m_DcBrush,ms,hs);

    if (ms > 600) rmp_cmd =500;

    /* the ctrl freq 200us*/
    if (us-last_us> 200)
    {
        last_us = us;
        /*vel ctrl*/
        double rmp_err = rmp_cmd-own.m_DcBrush.o_rpm;
        double vol_out = own.m_rpm_pid.pi(&own.m_rpm_pid,rmp_err);

        /*假设最大电压24v*/
        own.m_DcBrush.i_Ua = 24*(vol_out);

        if (own.m_DcBrush.i_Ua>24) own.m_DcBrush.i_Ua =24;
        if (own.m_DcBrush.i_Ua<-24) own.m_DcBrush.i_Ua =-24;


        // add wave data
        that->addWave(&that->m_obj,0,own.m_DcBrush.o_rpm);
        that->addWave(&that->m_obj,1,own.m_DcBrush.o_Te);
        that->addWave(&that->m_obj,2,own.m_DcBrush.o_Ia);
    }
}



// 仿真环境的构造函数
struct DcBrushCtrl * DcBrushCtrl_Constructor(struct DcBrushCtrl *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct DcBrushCtrl));

    that->init = _init;
    that->simTask = _simTask;

    // 子对象，组合对象的构造
    DcBrush_Constructor(&own.m_DcBrush);
    BaseObj_Constructor(&that->m_obj);

    PID_Constructor(&own.m_rpm_pid);

    return that;
}
