/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  pmsm_foc demo 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.07   wangchongwei    create file 
 **/

#ifndef _PMSM_FOC_H
#define _PMSM_FOC_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/pmsm/pmsm.h"
#include "components/simscape/electrical/inverter/inverter.h"
#include "components/controller/pid/pid.h"

enum PMSM_FOC_CTRL
{
    PMSM_FOC_CTRL_TUR,
    PMSM_FOC_CTRL_VEL,
    PMSM_FOC_CTRL_POS,
};

struct PmsmFoc
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct PmsmFoc *that);
    void (* simTask)(struct PmsmFoc *that);

    // member
    struct 
    {
        struct PID      m_Iq_pid;
        struct PID      m_Id_pid;
        struct PID      m_rpm_pid; 
        struct PID      m_pos_pid;

        struct Pmsm     m_Pmsm;
        struct Inverter m_Inverter;

        struct 
        {
            enum PMSM_FOC_CTRL c_ctrl_mode;
            float c_iq;
            float c_id;
            float c_vel;
            float c_pos;
        }Ctrl;
        
        struct 
        {
            float i_ele_angle;
            float i_cur[3];    // a b c cur
            float i_ref_Iq_pu;
            float i_ref_Id_pu;

            float m_cur_alpha;
            float m_cur_beta;

            float m_Id;
            float m_Iq;

            float m_vol_alpha;
            float m_vol_beta;


            float o_Vq_pu;
            float o_Vd_pu;

            float o_Ua_pu;
            float o_Ub_pu;
            float o_Uc_pu;
        }Tor;

        struct 
        {
            float i_rpm_fb;
            float i_ref_rmp;    // a b c cur

            float o_tor_ctrl_pu;
        }Vel;
        
        struct 
        {
            float i_ref_pos;
            float i_fb_pos;

            float o_vel_ctrl_pu;
        }Pos;

    }pvt;
};

struct PmsmFoc * PmsmFoc_Constructor(struct PmsmFoc *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif
