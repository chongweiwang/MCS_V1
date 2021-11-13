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

#include "inverter.h"

#define own that->pvt


static void _init(struct Inverter *that)
{

}

/**
 *  simulation inverter voltage
 *  you can consider the various non-linearities of the inverter
*/


static void _simulation(struct Inverter *that,double t, double hs)
{
    #define  LIMIT(x,max,min)  (x=(x>=max?max:(x<=min?min:x)))

    LIMIT(that->i_Ua_pu, 1.0, -1.0);
    LIMIT(that->i_Ub_pu, 1.0, -1.0);
    LIMIT(that->i_Uc_pu, 1.0, -1.0);

    that->o_Ua = that->i_Ua_pu * that->i_busVol;
    that->o_Ub = that->i_Ub_pu * that->i_busVol;
    that->o_Uc = that->i_Uc_pu * that->i_busVol;
}


struct Inverter * Inverter_Constructor(struct Inverter *that)
{
    // 1. that function bind
    memset(that , 0 , sizeof(struct Inverter));
    that->simulation = _simulation;
    that->init = _init;

    return that;
}

