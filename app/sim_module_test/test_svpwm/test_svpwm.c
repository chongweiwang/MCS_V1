/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      svpwm test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.25   wangchongwei    svpwm test
 **/
#include "test_svpwm.h"
#include <math.h>

#define own (that->pvt)

static void _init(struct TstSvpwm *that)
{
    that->setChName(&that->m_obj,0,"a_duty",strlen("a_duty"));
    that->setChName(&that->m_obj,1,"b_duty",strlen("b_duty"));
    that->setChName(&that->m_obj,2,"c_duty",strlen("c_duty"));
    that->setChName(&that->m_obj,3,"ab_duty",strlen("ab_duty"));
    that->setChName(&that->m_obj,4,"ac_duty",strlen("ac_duty"));
    that->setChName(&that->m_obj,5,"bc_duty",strlen("bc_duty"));
}



static void _simTask(struct TstSvpwm *that)
{
    #define SAMPLE_PERIOD_US  50

    float ms = that->getTimeMs(&that->m_obj);
    float hs = that->getTimeHs(&that->m_obj);
    float us = that->getTimeUs(&that->m_obj);

    static float last_us = 0;


    /*svpwm test*/
    static float theta = 0;


    float uq = 0.3; // 最大内切圆，等于 sqrt(3)/3
    float ud = 0;

    float alpha = 0;
    float beta = 0;

    theta += 0.00005;

    float st = sin(theta);
    float ct = cos(theta);

    // inv_park
    inv_park(ud, uq, st, ct, &alpha, &beta);

    // svpwm
    own.m_svpwm.i_vol_alpha_pu = alpha;
    own.m_svpwm.i_vol_beta_pu = beta;

    own.m_svpwm.run(&own.m_svpwm);

    if (us-last_us > SAMPLE_PERIOD_US)
    {
        last_us = us;

        // 添加波形
        that->addWave(&that->m_obj,0,own.m_svpwm.o_abc_duty_pu[0]);
        that->addWave(&that->m_obj,1,own.m_svpwm.o_abc_duty_pu[1]);
        that->addWave(&that->m_obj,2,own.m_svpwm.o_abc_duty_pu[2]);
        that->addWave(&that->m_obj,3,own.m_svpwm.o_abc_duty_pu[0] - own.m_svpwm.o_abc_duty_pu[1]);
        that->addWave(&that->m_obj,4,own.m_svpwm.o_abc_duty_pu[0] - own.m_svpwm.o_abc_duty_pu[2]);
        that->addWave(&that->m_obj,5,own.m_svpwm.o_abc_duty_pu[1] - own.m_svpwm.o_abc_duty_pu[2]);

    }

}



// 仿真环境的构造函数
struct TstSvpwm * TstSvpwm_Constructor(struct TstSvpwm *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct TstSvpwm));

    that->init = _init;
    that->simTask = _simTask;

    Svpwm_Constructor(&own.m_svpwm);
    BaseObj_Constructor(&that->m_obj);

    return that;
}
