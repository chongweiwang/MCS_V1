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
    void (*init)(struct Inverter *that,float hs);
    void (*simulation)(struct Inverter *that,float t, float hs);


    void (*svpwmSim)(struct Inverter *that,float t, float abc_duty[3]);

    // configuration variable
    float c_dead;  
    float c_hs;

    // input variable
    float i_busVol;     // bus voltage
    float i_Ua_pu, i_Ub_pu, i_Uc_pu;

    // out put 
    float o_Ua,o_Ub,o_Uc;   

    struct 
    {
        float m_abcDuty[3];
    }pvt;
};

struct Inverter * Inverter_Constructor(struct Inverter *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif 
