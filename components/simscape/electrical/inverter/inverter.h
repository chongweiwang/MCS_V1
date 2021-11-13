/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  inverter
 * @changelog:
 * date         author          notes
 * 2021.11.05   wangchongwei    first version   
 * 
 * Non-linear reference:https://zhuanlan.zhihu.com/p/309535077
 **/
#ifndef _INVERTER_H_
#define _INVERTER_H_


#ifdef  __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include <math.h>

struct Inverter
{
    void (*init)(struct Inverter *that);
    void (*simulation)(struct Inverter *that,double t, double hs);

    // configuration variable
    double c_dead;  

    // input variable
    double i_busVol;     // bus voltage
    double i_Ua_pu, i_Ub_pu, i_Uc_pu;

    // out put 
    double o_Ua,o_Ub,o_Uc;   

    struct 
    {

    }pvt;
};

struct Inverter * Inverter_Constructor(struct Inverter *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif 
