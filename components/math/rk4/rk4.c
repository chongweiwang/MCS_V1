/**
 * @copyright Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Runge-Kutta method for solving differential equations  
 * @changelog:
 * date         author          notes
 * 2021.9.29    wangchongwei    first version   
 **/

#include "rk4.h"


// 传入对象，以及ode 常微分方程
static void _init(struct RK4 *that, const void *obj, const dynamic_eq_calbak dynamic)
{
    that->pvt.mObj = obj;
    that->pvt.mDynamic = dynamic;
}


static void _rk4_solve(struct RK4 *that,float t, float hs, float *dx, float *x,uint32_t size)
{
    float k1[SIZE_MAX], k2[SIZE_MAX], k3[SIZE_MAX], k4[SIZE_MAX], xk[SIZE_MAX];
    uint32_t i;

    that->pvt.mDynamic(that->pvt.mObj, dx, x, size);
    for(i=0;i<size;i++){        
        k1[i] = dx[i] * hs;
        xk[i] = x[i] + k1[i]*0.5;
    }

    that->pvt.mDynamic(that->pvt.mObj, dx, xk, size); 
    for(i=0;i<size;i++){        
        k2[i] = dx[i] * hs;
        xk[i] = x[i] + k2[i]*0.5;
    }

    that->pvt.mDynamic(that->pvt.mObj, dx, xk, size);
    for(i=0;i<size;i++){        
        k3[i] = dx[i] * hs;
        xk[i] = x[i] + k3[i];
    }

    that->pvt.mDynamic(that->pvt.mObj, dx, xk, size);
    for(i=0;i<size;i++){        
        k4[i] = dx[i] * hs;
        x[i] = x[i] + (k1[i] + 2*(k2[i] + k3[i]) + k4[i])/6.0;
    }
}

struct RK4 * RK4_Constructor(struct RK4 *that)
{
    memset(that , 0 , sizeof(struct RK4));
    that->init = _init;
    that->rk4_solve = _rk4_solve;

    return that;
}
