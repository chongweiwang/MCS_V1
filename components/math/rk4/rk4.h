/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Runge-Kutta method for solving differential equations  
 * @changelog:
 * date         author          notes
 * 2021.9.29    wangchongwei    first version   
 **/
#ifndef _RK4_H_
#define _RK4_H_


#ifdef  __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include <stdint.h>

// rk4求解 矩阵大小
#define SIZE_MAX  30


// dynamic equation
// void dynamic_eq(obj, dx, x, size)
typedef void (*dynamic_eq_calbak)(void *, float *, float *, uint32_t);

struct RK4
{
    void (* init)(struct RK4 *that, void *obj, const dynamic_eq_calbak dynamics);
    void (* rk4_solve)(struct RK4 *that, float t, float hs, float *dx, float *x,uint32_t size);

    struct 
    {
        void *mObj;
        dynamic_eq_calbak  mDynamic;
    }pvt;
};

struct RK4 * RK4_Constructor(struct RK4 *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // _RK4_H_
