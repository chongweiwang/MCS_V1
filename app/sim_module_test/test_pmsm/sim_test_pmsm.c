/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  PMSM test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.21   wangchongwei    just test pmsm model
 **/

#include "test_pmsm.h"
#include "sim_test_pmsm.h"


struct TstPmsm gTstPmsm;

void sim_test_pmsm_init(void)
{
    struct TstPmsm *pTstPmsm;
    pTstPmsm = TstPmsm_Constructor(&gTstPmsm);
    pTstPmsm->init(pTstPmsm);
}



void sim_test_pmsm_task(void)
{
    gTstPmsm.simTask(&gTstPmsm);
}
