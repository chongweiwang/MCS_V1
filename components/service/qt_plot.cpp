/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      c arr -> qt wave
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.18   wangchongwei    first version   
 **/

#include "qt_plot.h"
#include "page/waveplot.h"
#include <vector>


#ifdef __cplusplus
extern "C" {
#endif

WavePlot *gQtWave;

int getInstance(int * handle)
{
    gQtWave = WavePlot::instance();
    *handle = 0;
    return 1;
}


int qtAddWave(int handle,char *name,uint32_t id,double *x, double *y, uint32_t size)
{
    WaveInfo tmp_info;
    tmp_info.id = id;
    memcpy(tmp_info.name, name,128);

    gQtWave->addSimWave(tmp_info,x,y,size);

    return 1;
}

int qtSetWaveNum(int handle , uint32_t cnt)
{

    gQtWave->setSimWaveCnt(cnt);
    return 1;
}

int runPlot(int handle)
{
    gQtWave->SimWavePlot();
    return 1;
}

#ifdef __cplusplus
};
#endif
