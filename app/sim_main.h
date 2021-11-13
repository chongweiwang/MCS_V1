/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License
 * @brief:      main function 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.10   wangchongwei    first version
 **/

#ifndef TASK_MAIN_H
#define TASK_MAIN_H


#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sim_inc.h"


void sim_main(void);
void simTaskHook(void);
void simInitHook(void);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // TASK_MAIN_H
