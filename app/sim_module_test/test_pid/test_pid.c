/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      pid test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.21   wangchongwei    pid test
 **/

#include "test_pid.h"

#define own (that->pvt)

static void _init(struct TstPid *that)
{
    // 参数初始化
    own.m_DcBrush.c_psi = 0.03;
    own.m_DcBrush.c_Kt  = 0.03;
    own.m_DcBrush.c_R   = 0.3;
    own.m_DcBrush.c_L   = 0.001;

    own.m_DcBrush.init(&own.m_DcBrush);

    #define  KP  0.0060f
    #define  KI  0.000025f
    #define  KD  0.0f

    own.m_rpm_backcal_pid.setCtrlParm(&own.m_rpm_backcal_pid,KP, KI, KD);
    own.m_rpm_backcal_pid.setClampParm(&own.m_rpm_backcal_pid,1,-1);
    own.m_rpm_backcal_pid.setKcParm(&own.m_rpm_backcal_pid,1);

    own.m_rpm_dclamp_pid.setCtrlParm(&own.m_rpm_dclamp_pid,KP, KI, KD);
    own.m_rpm_dclamp_pid.setClampParm(&own.m_rpm_dclamp_pid,1,-1);

    own.m_rpm_inc_pid.setCtrlParm(&own.m_rpm_inc_pid,KP, KI, KD);
    own.m_rpm_inc_pid.setClampParm(&own.m_rpm_inc_pid,1,-1);

    own.m_rpm_sclamp_pid.setCtrlParm(&own.m_rpm_sclamp_pid,KP, KI, KD);
    own.m_rpm_sclamp_pid.setClampParm(&own.m_rpm_sclamp_pid,1,-1,1,-1);

    own.m_rpm_pid.setCtrlParm(&own.m_rpm_pid,KP, KI, KD);
    own.m_rpm_pid.setIntegralParm(&own.m_rpm_pid,1,-1);

    that->setChName(&that->m_obj,0,"rpm",strlen("rpm"));
    that->setChName(&that->m_obj,1,"Te,N*m",strlen("Te,N*m"));
    that->setChName(&that->m_obj,2,"Ia,A",strlen("Ia,A"));
}

enum PID_KIND{
    NORMAL,
    BACK_CAL,
    INC,
    SCLAMP,
    DCLAMP
};


// 直流有刷电机主要在这里进行仿真
static void _simTask(struct TstPid *that)
{
    float us = that->getTimeUs(&that->m_obj);
    float ms = that->getTimeMs(&that->m_obj);
    float hs = that->getTimeHs(&that->m_obj);
    static float last_us = 0;

    float rmp_cmd = 200;

    if (ms > 500) rmp_cmd = 400;

    /* the ctrl freq 200us*/
    if (us-last_us> 100)
    {
        last_us = us;
        /*vel ctrl*/
        float rmp_err = rmp_cmd-own.m_DcBrush.o_rpm;

        enum PID_KIND pid_kind = BACK_CAL;
        float vol_out = 0;

        switch (pid_kind)
        {
        case NORMAL:
            vol_out = own.m_rpm_pid.pi(&own.m_rpm_pid,rmp_err);
            break;
        case BACK_CAL:
            vol_out = own.m_rpm_backcal_pid.pi(&own.m_rpm_backcal_pid,rmp_err);
            break;
        case INC:
            vol_out = own.m_rpm_inc_pid.pi(&own.m_rpm_inc_pid,rmp_err);
            break;
        case SCLAMP:
            vol_out = own.m_rpm_sclamp_pid.pi(&own.m_rpm_sclamp_pid,rmp_err);
            break;
        case DCLAMP:
            vol_out = own.m_rpm_dclamp_pid.pi(&own.m_rpm_dclamp_pid,rmp_err);
            break;
        default:
            break;
        }

        /*假设最大电压24v*/
        own.m_DcBrush.i_Ua = 24*(vol_out);

        // add wave data
        that->addWave(&that->m_obj,0,own.m_DcBrush.o_rpm);
        that->addWave(&that->m_obj,1,own.m_DcBrush.o_Te);
        that->addWave(&that->m_obj,2,own.m_DcBrush.o_Ia);
    }

    own.m_DcBrush.i_TL = 0.20;
    own.m_DcBrush.i_J  = 0.001;

    own.m_DcBrush.simulation(&own.m_DcBrush,ms,hs);
}



// 仿真环境的构造函数
struct TstPid * TstPid_Constructor(struct TstPid *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct TstPid));

    that->init = _init;
    that->simTask = _simTask;

    // 子对象，组合对象的构造
    DcBrush_Constructor(&own.m_DcBrush);
    BaseObj_Constructor(&that->m_obj);

    BackCalPID_Constructor(&own.m_rpm_backcal_pid);
    DClampPID_Constructor(&own.m_rpm_dclamp_pid);
    PID_Constructor(&own.m_rpm_pid);
    IncPID_Constructor(&own.m_rpm_inc_pid);
    SClampPID_Constructor(&own.m_rpm_sclamp_pid);

    return that;
}
