/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Provide waveform printing, simulation time interface
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.08   wangchongwei    first version   
 **/

#ifndef BASE_OBJ_H
#define BASE_OBJ_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "app/sim_config.h"

#ifdef  __cplusplus
extern "C" {
#endif





// if 20kh  100s   = 20000 * 100 = 2000000lu
//#define  BASEOBJ_WAVE_MAX_SIZE  4000000lu
//#define  BASEOBJ_WAVE_MAX_CH  9


// wave log err
struct BaseObj
{
    /* 设置波形对应通道 name*/
    void (*setChName)(struct BaseObj *that, uint16_t ch, char*name,uint32_t len);
    /* 对应通道添加波形数据*/
    void (*addWave)(struct BaseObj *that, uint16_t ch, double data);

    /* 获取时间cnt*/
    uint64_t (*getTimeCnt)(struct BaseObj *that);
    /* 获取时间ms*/
    double (*getTimeMs)(struct BaseObj *that);
    /* 获取时间us*/
    double (*getTimeUs)(struct BaseObj *that);
    /* 获取时间S*/
    double (*getTimeS)(struct BaseObj *that);
    double (*getTimeHs)(struct BaseObj *that);

    /* 全局波形打印*/
    void (*globalPlot)(struct BaseObj *that);
    /* 全局时间设置补偿*/
    void (*globalInitTime)(struct BaseObj *that,double hs);

    /* 周期循环仿真全局时间步进*/
    void (*globalTimeStep)(struct BaseObj *that);
};

#define CLASS_BaseObj(obj) union{\
        struct BaseObj (obj);\
        struct \
        {\
            void (*setChName)(struct BaseObj *that, uint16_t ch, char*name,uint32_t len);\
            void (*addWave)(struct BaseObj *that, uint16_t ch, double data);\
            uint64_t (*getTimeCnt)(struct BaseObj *that);\
            double (*getTimeMs)(struct BaseObj *that);\
            double (*getTimeUs)(struct BaseObj *that);\
            double (*getTimeS)(struct BaseObj *that);\
            double (*getTimeHs)(struct BaseObj *that);\
            void (*globalPlot)(struct BaseObj *that);\
            void (*globalInitTime)(struct BaseObj *that,double hs);\
            void (*globalTimeStep)(struct BaseObj *that);\
        };\
}

struct BaseObj * BaseObj_Constructor(struct BaseObj *that);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // BASE_OBJECT_H
