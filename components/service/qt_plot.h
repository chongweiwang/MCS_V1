/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      c arr -> qt wave
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.18   wangchongwei    first version   
 **/
#ifndef QT_PLOT_H
#define QT_PLOT_H

#include <stdint.h>
#ifdef  __cplusplus
extern "C" {
#endif

int getInstance(int * handle);
int qtSetWaveNum(int handle , uint32_t cnt);
int runPlot(int handle);
int qtAddWave(int handle,char *name,uint32_t id,double *x, double *y, uint32_t size);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif