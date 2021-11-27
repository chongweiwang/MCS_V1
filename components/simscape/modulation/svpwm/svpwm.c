/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      SVPWM
 * @changelog:
 * date         author          notes
 * 2021.11.25   wangchongwei    first version   
 **/

#include "svpwm.h"

#define own that->pvt 


static void _init(struct Svpwm *that,float limit)
{
    own.m_limit = limit;
}



/*
 * @brief:  fast invsqrt (Newton-Raphson Method,NR)
 * @date:   2020.11.8
 * @author: wangchongwei
 */
float fast_inv_sqrt(float x) 
{
    float x_half = x*0.5f;
    uint32_t u32x = *(int *)&x;

    u32x  = 0x5f3759df - (u32x >> 1);     // (0x5f3759df)Carmack operator
    x  = * (float *) &u32x;

    x  = x * (1.5f - (x_half * x * x));  // Newton-Raphson

    return x;
}

/*Vector interval*/
const uint8_t SectorTable[7] = {0,2,6,1,4,3,5};

#define SQRT3_DIV_2  0.866025403f

#define SQRT3  1.73205f

void over_mod(float *t1, float *t2)
{
   if (*t1 + *t2 > 0.99)
   {
       *t1 = *t1/(*t1 + *t2);
       *t2 = *t2/(*t1 + *t2);
   }
}

float limit(float x, float min,float max)
{
    if (x>max) x = max;
    else if(x<min) x = min;

    return x;
}


static void _run(struct Svpwm *that)
{
    /* The modulation factor is SQRT3
     * so the max uref = 2/3udc is hexagon
     * so the max uref = SQRT3/3udc is Maximum inscribed circle
    */
    own.m_mod = SQRT3;  

    own.m_u[0] = that->i_vol_beta_pu;
    own.m_u[1] = that->i_vol_alpha_pu*SQRT3_DIV_2 - that->i_vol_beta_pu*0.5f;
    own.m_u[2] = -that->i_vol_alpha_pu*SQRT3_DIV_2 - that->i_vol_beta_pu*0.5f;

    /* get vector interval*/
    own.m_sector = (own.m_u[0]>0) + (own.m_u[1]>0)*2 + (own.m_u[2]>0)*4;
    own.m_sector = SectorTable[own.m_sector];

    switch (own.m_sector)
    {
    case 1:
        own.m_t[4] = own.m_mod * own.m_u[1];
        own.m_t[6] = own.m_mod * own.m_u[0];

        over_mod(&own.m_t[4],&own.m_t[6]);

        own.m_t[0] = (1.0f - own.m_t[4] - own.m_t[6])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] = own.m_t[4] + own.m_t[6] + own.m_t[7];
        that->o_abc_duty_pu[1] =              own.m_t[6] + own.m_t[7];
        that->o_abc_duty_pu[2] =                           own.m_t[7];
        break;

    case 2:
        own.m_t[2] = -own.m_mod * own.m_u[1];
        own.m_t[6] = -own.m_mod * own.m_u[2];

        over_mod(&own.m_t[2],&own.m_t[6]);

        own.m_t[0] = (1.0f - own.m_t[2] - own.m_t[6])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] =              own.m_t[6] + own.m_t[7];
        that->o_abc_duty_pu[1] = own.m_t[2] + own.m_t[6] + own.m_t[7];
        that->o_abc_duty_pu[2] =                           own.m_t[7];
        break;

    case 3:
        own.m_t[2] = own.m_mod * own.m_u[0];
        own.m_t[3] = own.m_mod * own.m_u[2];

        over_mod(&own.m_t[2],&own.m_t[3]);

        own.m_t[0] = (1.0f - own.m_t[2] - own.m_t[3])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] =                           own.m_t[7];
        that->o_abc_duty_pu[1] = own.m_t[2] + own.m_t[3] + own.m_t[7];
        that->o_abc_duty_pu[2] =              own.m_t[3] + own.m_t[7];
        break;

    case 4:
        own.m_t[1] = -own.m_mod * own.m_u[0];
        own.m_t[3] = -own.m_mod * own.m_u[1];

        over_mod(&own.m_t[1],&own.m_t[3]);

        own.m_t[0] = (1.0f - own.m_t[1] - own.m_t[3])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] =                           own.m_t[7];
        that->o_abc_duty_pu[1] =              own.m_t[3] + own.m_t[7];
        that->o_abc_duty_pu[2] = own.m_t[1] + own.m_t[3] + own.m_t[7];
        break;
    case 5:
        own.m_t[1] = own.m_mod * own.m_u[2];
        own.m_t[5] = own.m_mod * own.m_u[1];

        over_mod(&own.m_t[1], &own.m_t[5]);

        own.m_t[0] = (1.0f - own.m_t[1] - own.m_t[5])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] =              own.m_t[5] + own.m_t[7];
        that->o_abc_duty_pu[1] =                           own.m_t[7];
        that->o_abc_duty_pu[2] = own.m_t[1] + own.m_t[5] + own.m_t[7];
        break;

    case 6:
        own.m_t[4] = -own.m_mod * own.m_u[2];
        own.m_t[5] = -own.m_mod * own.m_u[0];

        over_mod(&own.m_t[4], &own.m_t[5]);

        own.m_t[0] = (1.0f - own.m_t[4] - own.m_t[5])*0.5;
        own.m_t[7] = own.m_t[0];

        /*Calculate the duty cycle*/
        that->o_abc_duty_pu[0] = own.m_t[4] + own.m_t[5] + own.m_t[7];
        that->o_abc_duty_pu[1] =                           own.m_t[7];
        that->o_abc_duty_pu[2] =              own.m_t[5] + own.m_t[7];
        break;
    default:
        that->o_abc_duty_pu[0] = 0;
        that->o_abc_duty_pu[1] = 0;
        that->o_abc_duty_pu[2] = 0;
    }

    // pwm limit
    that->o_abc_duty_pu[0] = limit(that->o_abc_duty_pu[0],0,0.99);
    that->o_abc_duty_pu[1] = limit(that->o_abc_duty_pu[1],0,0.99);
    that->o_abc_duty_pu[2] = limit(that->o_abc_duty_pu[2],0,0.99);
}

struct Svpwm * Svpwm_Constructor(struct Svpwm *that)
{
    // 1. that function bind
    memset(that , 0 , sizeof(struct Svpwm));
    that->run = _run;
    that->init = _init;

    return that; 
}
