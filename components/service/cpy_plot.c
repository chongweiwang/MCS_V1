/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  C language calls py to print the waveform
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.09.27   wangchongwei    first version   
 **/
#include "cpy_plot.h"
#include <Python.h>


// 初始化python调用模块
static void _init(struct CpyPlot *that)
{
    DEBUG_MSG("Py_Initialize");

    Py_SetPythonHome(Py_Home);
    Py_Initialize();


    if (!Py_IsInitialized())
    {
        DEBUG_MSG("err: python init fail");
        return;
    }

    PyRun_SimpleString("import sys");

    // 导入py模块
    that->pvt.pModule = PyImport_ImportModule("wave_plot");
    if (NULL == that->pvt.pModule){
        DEBUG_MSG("err: python pModule fail");
        return;
    }

    // 获取
    that->pvt.pDict = PyModule_GetDict(that->pvt.pModule);
    if (NULL == that->pvt.pDict){
        DEBUG_MSG("err: python pDict fail");
        return;
    }

    // 获取类名实例化
    that->pvt.pClass = PyObject_GetAttrString(that->pvt.pModule, "MPlot");
    if (NULL == that->pvt.pClass){
        DEBUG_MSG("err: python pClass fail");
        return;
    }

    PyObject *arg= Py_BuildValue("(i)", 1);
    that->pvt.pInstance = PyObject_Call(that->pvt.pClass, arg, NULL);
    if (NULL == that->pvt.pInstance){
        DEBUG_MSG("err: python pInstance fail");
        return;
    }

}

// 传入波形
static void _addWave(struct CpyPlot *that,struct WaveProperty *Prop,double *x, double *y, uint32_t size)
{
    PyObject *xList  = PyList_New(size);
    PyObject *yList  = PyList_New(size);

    // 传入pylist
    for(int i = 0; i < PyList_Size(xList); i++){
        PyList_SetItem(xList,i, PyFloat_FromDouble(x[i]));
    }

    // 传入pylist
    for(int i = 0; i < PyList_Size(yList); i++){
        PyList_SetItem(yList,i, PyFloat_FromDouble(y[i]));
    }

    // 得到类的addwave函数
    PyObject *pFunc = PyObject_GetAttrString(that->pvt.pInstance, "addWave");
    if (NULL == pFunc){
        DEBUG_MSG("err: python pFunc addWave fail");
        return;
    }

    PyObject *pArgList = PyTuple_New(3);//定义一个Tuple对象，Tuple对象的长度与Python函数参数个数一直，上面Python参数个数为1，所以这里给的长度为1
    PyObject *pNum = Py_BuildValue("i", Prop->id);
    PyTuple_SetItem(pArgList, 0, pNum);
    PyTuple_SetItem(pArgList, 1, xList);
    PyTuple_SetItem(pArgList, 2, yList);

    PyObject * pReturn = PyEval_CallObject(pFunc, pArgList);

    // 得到类的setWaveName函数
    pFunc = PyObject_GetAttrString(that->pvt.pInstance, "setWaveName");
    if (NULL == pFunc){
        DEBUG_MSG("err: python pFunc setWaveName fail");
        return;
    }

    pArgList = PyTuple_New(2);
    pNum = Py_BuildValue("i", Prop->id);
    PyObject *pStr = Py_BuildValue("s", Prop->name);
    PyTuple_SetItem(pArgList, 0, pNum);
    PyTuple_SetItem(pArgList, 1, pStr);

    pReturn = PyEval_CallObject(pFunc, pArgList);
}

static void _setWaveNum(struct CpyPlot *that,uint32_t cnt)
{
    PyObject * pFunc2 = PyObject_GetAttrString(that->pvt.pInstance, "setWaveNum");
    PyObject * pArg = PyTuple_New(1);
    PyObject * pNum = Py_BuildValue("i", cnt);
    PyTuple_SetItem(pArg, 0, pNum);

    PyObject * pReturn = PyEval_CallObject(pFunc2, pArg);
}


// 画图
static void _runPlot(struct CpyPlot *that)
{
    // PyObject * pFunc3 = PyObject_GetAttrString(that->pvt.pInstance, "plt");
    // PyObject * pArg = PyTuple_New(0);
    // PyObject * pReturn = PyEval_CallObject(pFunc3, pArg);

    
    PyObject * pReturn = PyObject_CallMethod(that->pvt.pInstance,"plt", NULL);
    DEBUG_MSG("plot function");
}

// 结束
static void _destroy(struct CpyPlot *that)
{
    Py_Finalize();
    DEBUG_MSG("Py_Finalize,destroy");
}

struct CpyPlot *CpyPlot_Constructor(struct CpyPlot *that)
{
    memset(that , 0 , sizeof(struct CpyPlot));
    that->addWave = _addWave;
    that->destroy = _destroy;
    that->setWaveNum = _setWaveNum;
    that->init = _init;
    that->runPlot = _runPlot;

    return that;
}
