/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  dc brush motor test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.12   wangchongwei    test dc brush motor model
 **/

#include "test_dc_brush.h"

#define own (that->pvt)

static void _init(struct TstDcBrush *that)
{
    // 参数初始化
    own.m_DcBrush.c_psi = 0.1;
    own.m_DcBrush.c_Kt  = 0.1;
    own.m_DcBrush.c_R   = 0.3;
    own.m_DcBrush.c_L   = 0.003;

    own.m_DcBrush.init(&own.m_DcBrush);

    that->setChName(&that->m_obj,0,"rpm",strlen("rpm"));
    that->setChName(&that->m_obj,1,"Te,N*m",strlen("Te,N*m"));
    that->setChName(&that->m_obj,2,"Ia,A",strlen("Ia,A"));
}

// 直流有刷电机主要在这里进行仿真
static void _simTask(struct TstDcBrush *that)
{
    #define SAMPLE_PERIOD_US  50

    float us = that->getTimeUs(&that->m_obj);
    float ms = that->getTimeMs(&that->m_obj);
    float hs = that->getTimeHs(&that->m_obj);
    static float last_us = 0;

    // 输入电压，负载
    own.m_DcBrush.i_Ua = 24;
    own.m_DcBrush.i_TL = 1;
    own.m_DcBrush.i_J  = 0.001;

    // 3s加负载
    if (ms>3000)  own.m_DcBrush.i_TL = 4;

    // 电机仿真
    own.m_DcBrush.simulation(&own.m_DcBrush,ms,hs);

    if (us-last_us > SAMPLE_PERIOD_US)
    {
        last_us = us;

        // 添加波形数据
        that->addWave(&that->m_obj,0,own.m_DcBrush.o_rpm);
        that->addWave(&that->m_obj,1,own.m_DcBrush.o_Te);
        that->addWave(&that->m_obj,2,own.m_DcBrush.o_Ia);
    }
}



// 仿真环境的构造函数
struct TstDcBrush * TstDcBrush_Constructor(struct TstDcBrush *that)
{
    // 本对象的构造
    memset(that , 0 , sizeof(struct TstDcBrush));

    that->init = _init;
    that->simTask = _simTask;

    // 子对象，组合对象的构造
    DcBrush_Constructor(&own.m_DcBrush);
    BaseObj_Constructor(&that->m_obj);

    return that;
}
