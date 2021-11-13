/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  C language calls py to print the waveform
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.09.27   wangchongwei    first version   
 **/


#ifndef CPY_PLOT_H
#define CPY_PLOT_H


#ifdef  __cplusplus
extern "C" {
#endif

#include "app/sim_config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <Python.h>


#define  DEBUG_MSG(x)   printf("%s \r\n",x);fflush(stdout)


struct WaveProperty
{
    uint16_t id;     // 序号
    uint16_t ch;     // 通道
    char unit[128];  // 单位
    char name[128];  // 名字
};

struct  CpyPlot
{
    // 初始化python调用模块
    void (*init)(struct CpyPlot *that);
    // 传入波形
    void (*addWave)(struct CpyPlot *that,struct WaveProperty *Prop,double *x, double *y, uint32_t size);
    // 设置波形num
    void (*setWaveNum)(struct CpyPlot *that,uint32_t cnt);
    // 画图
    void (*runPlot)(struct CpyPlot *that);
    // 结束销毁
    void (*destroy)(struct CpyPlot *that);

    struct
    {
        PyObject *pModule;   // python 加载模块
        PyObject *pDict;     // 对象字典
        PyObject *pClass;    // 类
        PyObject *pInstance; // 类的实例化
        PyObject *pFunc;     // 成员函数
    }pvt;
};


struct CpyPlot * CpyPlot_Constructor(struct CpyPlot *that);


#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // CPY_PLOT_H
