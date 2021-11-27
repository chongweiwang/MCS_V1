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
    void (*simulation)(struct Pmsm *that,float t, float hs);

    // configuration variable
    float c_rated_cur;         /*额定电流*/
    float c_rated_vol;         /*额定电压*/
    float c_rated_tor;         /*额定力矩*/
    float c_rated_vel;         /*额定速度*/
    float c_rated_pow;         /*额定功率*/

    float c_R;          // Ohm 
    float c_Lq,c_Ld;    // dq H
    float c_Pn;         // 极对数
    float c_B;          // 粘滞摩擦系数,阻尼系数N*m*s 
    float c_psi;        // 永磁体磁链
    float c_ke;         // 反电动势系数

    // input variable
    float i_Ua, i_Ub, i_Uc;    // 输入电压
    float i_TL;                // 输入负载转矩N*m
    float i_J;                 // kg.m^2 转动惯量

    // Internal ode calculate
    float o_Ia,o_Ib,o_Ic;          // A
    float o_omegaE,o_omegaR;       // rad/s
    float o_Te;                    // N*m
    float o_thetaE,o_thetaR;       // 位置信息
    float o_rpm;                   // 机械转速 rpm

    struct 
    {
        float m_Uq,m_Ud;
        float m_Iq,m_Id;

        float  m_x[5];
        float  m_dx[5];
        uint32_t m_size;

        struct RK4 m_rk4;
    }pvt;
};

struct Pmsm * Pmsm_Constructor(struct Pmsm *that);



#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif // _PMSM_H_
