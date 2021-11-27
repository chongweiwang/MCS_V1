/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      svpwm test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.25   wangchongwei    svpwm test
 **/

#include "test_svpwm.h"
#include "sim_test_svpwm.h"


struct TstSvpwm gTstSvpwm;

void sim_test_svpwm_init(void)
{
    struct TstSvpwm *pTstSvpwm;
    pTstSvpwm = TstSvpwm_Constructor(&gTstSvpwm);
    pTstSvpwm->init(pTstSvpwm);
}



void sim_test_svpwm_task(void)
{
    gTstSvpwm.simTask(&gTstSvpwm);
}
