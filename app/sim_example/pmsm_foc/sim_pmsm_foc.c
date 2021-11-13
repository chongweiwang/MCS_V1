/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  pmsm_foc demo 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.09   wangchongwei    create file 
 **/

#include "sim_pmsm_foc.h"
#include "pmsm_foc.h"

struct PmsmFoc gPmsmFoc;

void sim_pmsm_foc_init(void)
{
    struct PmsmFoc *pPmsmFoc;
    pPmsmFoc = PmsmFoc_Constructor(&gPmsmFoc);
    pPmsmFoc->init(pPmsmFoc);
}


void sim_pmsm_foc_task(void)
{
    gPmsmFoc.simTask(&gPmsmFoc);
}