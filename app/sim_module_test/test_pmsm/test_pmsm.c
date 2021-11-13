/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  PMSM test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.21   wangchongwei    just test pmsm model
 **/
#include "test_pmsm.h"
#include <math.h>

#define own (that->pvt)

static void _init(struct TstPmsm *that)
{
    own.m_Pmsm.c_B = 0.008;
    own.m_Pmsm.c_R = 2.875;
    own.m_Pmsm.c_Lq = 0.008;
    own.m_Pmsm.c_Ld = 0.008;
    own.m_Pmsm.c_Pn = 4;
    own.m_Pmsm.c_psi = 0.175;

    own.m_Pmsm.init(&own.m_Pmsm);

    that->setChName(&that->m_obj,0,"rpm",strlen("rpm"));
    that->setChName(&that->m_obj,1,"Te,N*m",strlen("Te,N*m"));
    that->setChName(&that->m_obj,2,"Ia,A",strlen("Ia,A"));
    that->setChName(&that->m_obj,3,"Ib,A",strlen("Ib,A"));
    that->setChName(&that->m_obj,4,"Ic,A",strlen("Ic,A"));
    that->setChName(&that->m_obj,5,"theta",strlen("theta"));
}

static void _simTask(struct TstPmsm *that)
{
    #define SAMPLE_PERIOD_US  50

    double ms = that->getTimeMs(&that->m_obj);
    double hs = that->getTimeHs(&that->m_obj);
    double us = that->getTimeUs(&that->m_obj);

    static double last_us = 0;

    own.m_Pmsm.i_TL = 1;
    own.m_Pmsm.i_J =  0.01;

    // 2s 加负载
    if (ms>2000)  own.m_Pmsm.i_TL = 4;


    // set input vol 
    double uq = 24;
    double ud = 0;

    double st = sin(own.m_Pmsm.o_thetaE);
    double ct = cos(own.m_Pmsm.o_thetaE);

    // dq->abc 得到三相电压
    dq2abc_amp(ud,uq,st,ct,&own.m_Pmsm.i_Ua,&own.m_Pmsm.i_Ub,&own.m_Pmsm.i_Uc);

    // pmsm电机仿真
    own.m_Pmsm.simulation(&own.m_Pmsm,ms,hs);


    if (us-last_us > SAMPLE_PERIOD_US)
    {
        last_us = us;

        // 添加波形
        that->addWave(&that->m_obj,0,own.m_Pmsm.o_rpm);
        that->addWave(&that->m_obj,1,own.m_Pmsm.o_Te);
        that->addWave(&that->m_obj,2,own.m_Pmsm.o_Ia);
        that->addWave(&that->m_obj,3,own.m_Pmsm.o_Ib);
        that->addWave(&that->m_obj,4,own.m_Pmsm.o_Ic);
        that->addWave(&that->m_obj,5,own.m_Pmsm.o_thetaE);
    }
}



// 仿真环境的构造函数
struct TstPmsm * TstPmsm_Constructor(struct TstPmsm *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct TstPmsm));

    that->init = _init;
    that->simTask = _simTask;

    Pmsm_Constructor(&own.m_Pmsm);
    BaseObj_Constructor(&that->m_obj);

    return that;
}
