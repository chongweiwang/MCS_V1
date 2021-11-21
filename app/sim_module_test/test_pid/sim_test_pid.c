/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      pid test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.21   wangchongwei    pid test
 **/
#include "test_pid.h"
#include "sim_test_pid.h"



struct TstPid gTstPid;

void sim_test_pid_init(void)
{
    struct TstPid *pTstPid;
    pTstPid = TstPid_Constructor(&gTstPid);
    gTstPid.init(pTstPid);
}



void sim_test_pid_task(void)
{
    gTstPid.simTask(&gTstPid);
}