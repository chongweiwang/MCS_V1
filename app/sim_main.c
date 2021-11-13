/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License
 * @brief:      main function 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.10   wangchongwei    first version
 **/

#include "sim_main.h"
#include "components/service/base_obj.h"

struct BaseObj gBaseObj;    

void sim_main(void)
{
    BaseObj_Constructor(&gBaseObj);
    gBaseObj.globalInitTime(&gBaseObj, SIM_STEP_SIZE);

    simInitHook();

    uint64_t cnt = 0;
    uint64_t end = SIM_TIME_LEN;

    for (cnt =0; cnt < end; cnt++)
    {
        // sim Time stepping 
        gBaseObj.globalTimeStep(&gBaseObj);

        simTaskHook();
    }

    // 波形打印
    gBaseObj.globalPlot(&gBaseObj);
}


/**
 * 工程初始化函数,负责调用初始化工程init
 * 
*/
void simInitHook(void)
{
    //sim_test_dc_brush_init();
    //sim_test_pmsm_init();


    //sim_dc_brush_ctrl_init();
    sim_pmsm_foc_init();
}
/**
 * 工程仿真hook函数,对应工程的主要仿真
*/
void simTaskHook(void)
{
    //sim_test_dc_brush_task();
    //sim_test_pmsm_task();

    //sim_dc_brush_ctrl_task();
    sim_pmsm_foc_task();
}


