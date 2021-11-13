/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  pmsm_foc demo 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.09   wangchongwei    create file 
 **/

#ifndef _SIM_PMSM_FOC_H_
#define _SIM_PMSM_FOC_H_

#ifdef  __cplusplus
extern "C" {
#endif


void sim_pmsm_foc_init(void);
void sim_pmsm_foc_task(void);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _SIM_PMSM_FOC_H_ */