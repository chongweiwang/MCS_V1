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
#include "sim_test_dc_brush.h"



struct TstDcBrush gTstDcBrush;

void sim_test_dc_brush_init(void)
{
    struct TstDcBrush *pTstDcBrush;
    pTstDcBrush = TstDcBrush_Constructor(&gTstDcBrush);
    gTstDcBrush.init(pTstDcBrush);
}



void sim_test_dc_brush_task(void)
{
    gTstDcBrush.simTask(&gTstDcBrush);
}