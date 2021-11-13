/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Provide waveform printing, simulation time interface
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.08   wangchongwei    first version   
 **/
#include "base_obj.h"


// 对图像的管理
#include "cpy_plot.h"
struct ObjWave
{
    
    struct 
    {
        uint64_t cnt;
        double data[BASEOBJ_WAVE_MAX_SIZE];
        double time[BASEOBJ_WAVE_MAX_SIZE];
        char name[128];
        uint32_t len;
    }ch[BASEOBJ_WAVE_MAX_CH+1];

    int32_t selectCh[BASEOBJ_WAVE_MAX_CH+1];
    int32_t selectNum;
    int32_t chSave[BASEOBJ_WAVE_MAX_CH+1];
    uint64_t maxCnt;
};




struct Time
{
    double   hs_step;
    uint64_t mTime_cnt;
    double   mTime_s;
    double   mTime_ms;
    double   mTime_us;
};


// 测试，这玩意得全局出问题的概率小点，局部声明经常出问题
static struct CpyPlot mCpy;
static struct ObjWave gWave;
static struct Time    gTime;

static uint64_t _getTimeCnt(struct BaseObj *that)
{
    return gTime.mTime_cnt;
}

static double _getTimeMs(struct BaseObj *that)
{
    return gTime.mTime_ms;
}

static double _getTimeUs(struct BaseObj *that)
{
    return gTime.mTime_us;
}

/* 获取时间S*/
static double _getTimeS(struct BaseObj *that)
{
    return gTime.mTime_s;
}
static double _getTimeHs(struct BaseObj *that)
{
    return gTime.hs_step;
}

static void _globalTimeStep(struct BaseObj *that)
{
    gTime.mTime_cnt++;
    gTime.mTime_ms = gTime.mTime_cnt*gTime.hs_step*1000;
    gTime.mTime_s  = gTime.mTime_cnt*gTime.hs_step;
    gTime.mTime_us = gTime.mTime_cnt*gTime.hs_step*1000000;
}

static void _globalInitTime(struct BaseObj *that,double hs)
{
    memset(&gTime , 0 , sizeof(struct Time));

    gTime.hs_step = hs;

    // 波形数据清零
    memset(&gWave , 0 , sizeof(struct ObjWave));

    for (int i = 0; i <= BASEOBJ_WAVE_MAX_CH;i++){
        gWave.selectCh[i] = -1;
    }
}

// 设置通道的名字
static void _setChName(struct BaseObj *that, uint16_t ch, char*name, uint32_t len)
{
    if (ch > BASEOBJ_WAVE_MAX_CH) return;

    gWave.ch[ch].len = len;
    strcpy(gWave.ch[ch].name,name);

}
// 加入对应通道数据
static void _addWave(struct BaseObj *that, uint16_t ch, double data)
{
    if (ch > BASEOBJ_WAVE_MAX_CH) return;

    if (gWave.ch[ch].cnt > BASEOBJ_WAVE_MAX_SIZE-1) return;

    gWave.ch[ch].data[gWave.ch[ch].cnt] = data;
    gWave.ch[ch].time[gWave.ch[ch].cnt] = gTime.mTime_ms;

    gWave.ch[ch].cnt++;

    gWave.selectCh[ch] = 1;
}



// 主要设计调用这个函数
static void _globalPlot(struct BaseObj *that)
{

    // 得到画图的数量，和对应的通道
    for (int i = 0; i <= BASEOBJ_WAVE_MAX_CH; i++ )
    {
        if (gWave.selectCh[i] == 1)
        {
            gWave.chSave[gWave.selectNum] = i;
            gWave.selectNum++;

            // 找到最大长度
            if (gWave.ch[i].cnt > gWave.maxCnt){
                gWave.maxCnt = gWave.ch[i].cnt;
            }
        }
    }

    // 加载python模块
    CpyPlot_Constructor(&mCpy);
    mCpy.init(&mCpy);

    mCpy.setWaveNum(&mCpy,gWave.selectNum);

    // 传入name 传入xy
    for (int i = 0; i < gWave.selectNum; i++ )
    {
        struct WaveProperty  wave;
        wave.id = i;
        strcpy(wave.name, gWave.ch[gWave.chSave[i]].name);

        mCpy.addWave(&mCpy,&wave,gWave.ch[gWave.chSave[i]].time,gWave.ch[gWave.chSave[i]].data,gWave.ch[gWave.chSave[i]].cnt);
    }

    mCpy.runPlot(&mCpy);
    mCpy.destroy(&mCpy);
}



struct BaseObj * BaseObj_Constructor(struct BaseObj *that)
{
    memset(that , 0 , sizeof(struct BaseObj));

    that->addWave = _addWave;
    that->setChName = _setChName;
    that->globalPlot = _globalPlot;
    that->getTimeCnt = _getTimeCnt;
    that->getTimeMs = _getTimeMs;
    that->getTimeS = _getTimeS;
    that->getTimeHs = _getTimeHs;
    that->globalInitTime = _globalInitTime;
    that->globalTimeStep = _globalTimeStep;
    that->getTimeUs = _getTimeUs;

    return that;
}
