/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  dc brush motor ctrl test 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.24   wangchongwei    dc brush motor ctrl test
 **/
#include "dc_brush_ctrl.h"
#include "sim_dc_brush_ctrl.h"



struct DcBrushCtrl gDcBrushCtrl;

void sim_dc_brush_ctrl_init(void)
{
    struct DcBrushCtrl *pDcBrushCtrl;
    pDcBrushCtrl = DcBrushCtrl_Constructor(&gDcBrushCtrl);
    pDcBrushCtrl->init(pDcBrushCtrl);
}


void sim_dc_brush_ctrl_task(void)
{
    gDcBrushCtrl.simTask(&gDcBrushCtrl);
}
