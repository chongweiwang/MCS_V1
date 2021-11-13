/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  Permanent magnet synchronous motor, dq coordinate system modeling
 * @changelog:
 * date         author          notes
 * 2021.10.14   wangchongwei    first version   
 * 2021.11.11   wangchongwei    add config value rated cur vol tor vel pow   
 **/
#ifndef _PMSM_H_
#define _PMSM_H_


#ifdef  __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include <math.h>
#include "components/math/rk4/rk4.h" 
#include "components/math/coordinate/coordinate.h" 

struct Pmsm
{
    void (*init)(struct Pmsm *that);
    void (*simulation)(struct Pmsm *that,double t, double hs);

    // configuration variable
    double c_rated_cur;         /*额定电流*/
    double c_rated_vol;         /*额定电压*/
    double c_rated_tor;         /*额定力矩*/
    double c_rated_vel;         /*额定速度*/
    double c_rated_pow;         /*额定功率*/

    double c_R;          // Ohm 
    double c_Lq,c_Ld;    // dq H
    double c_Pn;         // 极对数
    double c_B;          // 粘滞摩擦系数,阻尼系数N*m*s 
    double c_psi;        // 永磁体磁链
    double c_ke;         // 反电动势系数

    // input variable
    double i_Ua, i_Ub, i_Uc;    // 输入电压
    double i_TL;                // 输入负载转矩N*m
    double i_J;                 // kg.m^2 转动惯量

    // Internal ode calculate
    double o_Ia,o_Ib,o_Ic;          // A
    double o_omegaE,o_omegaR;       // rad/s
    double o_Te;                    // N*m
    double o_thetaE,o_thetaR;       // 位置信息
    double o_rpm;                   // 机械转速 rpm

    struct 
    {
        double m_Uq,m_Ud;
        double m_Iq,m_Id;

        double  m_x[5];
        double  m_dx[5];
        uint32_t m_size;

        struct RK4 m_rk4;
    }pvt;
};

struct Pmsm * Pmsm_Constructor(struct Pmsm *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // _PMSM_H_
