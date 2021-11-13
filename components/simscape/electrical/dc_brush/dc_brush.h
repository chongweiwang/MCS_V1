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
    void (*simulation)(struct DcBrush *that,double t, double hs);
    

    // configuration variable
    double  c_ke;    // V/rad
    double  c_psi;   // V/rad
    double  c_R;     // Ohm
    double  c_L;     // H
    double  c_Kt;    // N*m/A

    // input variable
    double  i_TL;     // N*m
    double  i_J;      // kg.m^2
    double  i_Ua;     // V  

    // output variable
    double  o_Ia;     // A
    double  o_omega;   // rad/s
    double  o_rpm;   // rad/s
    double  o_Te;     // N*m
    double  o_theta;     // 0-2pi

    struct 
    {
        double  m_x[3];
        double  m_dx[3];
        uint32_t m_size;

        struct RK4 m_rk4;
    }pvt;
};

struct DcBrush * DcBrush_Constructor(struct DcBrush *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // _DC_BRUSH_H_
