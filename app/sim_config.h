/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License
 * @brief:  仿真全局配置文件
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.10   wangchongwei    first version
 **/

#ifndef _SIM_CONFIG_H_
#define _SIM_CONFIG_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * 仿真的全局宏
 * */
#define  SIM_STEP_SIZE   (0.0000001)                   /*仿真的步长*/
#define  SIM_TIME_S      (1)                          /*仿真时间*/
#define  SIM_TIME_LEN    (SIM_TIME_S/SIM_STEP_SIZE)   /*仿真计算的次数*/


/**
 *  "components/service/base_obj.h"     宏定义
 * */

#define  BASEOBJ_WAVE_MAX_SIZE  5000000lu               /*波形最大点个数*/
#define  BASEOBJ_WAVE_MAX_CH    8                       /*最多波形数*/


#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _SIM_CONFIG_H_*/
