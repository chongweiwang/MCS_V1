/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      SVPWM
 * @changelog:
 * date         author          notes
 * 2021.11.25   wangchongwei    first version   
 **/
#ifndef _SVPWM_H_
#define _SVPWM_H_


#ifdef  __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "components/math/coordinate/coordinate.h" 

struct Svpwm
{
    void (*init)(struct Svpwm *that, float limit);

    void (*run)(struct Svpwm *that);

    float i_vol_alpha_pu;
    float i_vol_beta_pu;

    float o_abc_duty_pu[3];

    struct 
    {
        float  m_mod;    
        float  m_limit;
        uint8_t m_sector;
        float  m_u[3];
        float  m_t[8];
    }pvt;
};

struct Svpwm * Svpwm_Constructor(struct Svpwm *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _SVPWM_H_ */
