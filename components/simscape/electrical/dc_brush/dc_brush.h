/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Mathematical Model of DC Brushed Motor
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.09.28   wangchongwei    first version
 * 2021.10.19   wangchongwei    Change the naming convention and add interfaces
 **/
#ifndef _DC_BRUSH_H_
#define _DC_BRUSH_H_


#ifdef  __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include "components/math/rk4/rk4.h" 

struct DcBrush
{
    void (*init)(struct DcBrush *that);
    void (*simulation)(struct DcBrush *that,float t, float hs);
    

    // configuration variable
    float  c_ke;    // V/rad
    float  c_psi;   // V/rad
    float  c_R;     // Ohm
    float  c_L;     // H
    float  c_Kt;    // N*m/A

    // input variable
    float  i_TL;     // N*m
    float  i_J;      // kg.m^2
    float  i_Ua;     // V  

    // output variable
    float  o_Ia;     // A
    float  o_omega;   // rad/s
    float  o_rpm;   // rad/s
    float  o_Te;     // N*m
    float  o_theta;     // 0-2pi

    struct 
    {
        float  m_x[3];
        float  m_dx[3];
        uint32_t m_size;

        struct RK4 m_rk4;
    }pvt;
};

struct DcBrush * DcBrush_Constructor(struct DcBrush *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // _DC_BRUSH_H_
