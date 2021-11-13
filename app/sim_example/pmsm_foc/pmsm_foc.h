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
            double c_iq;
            double c_id;
            double c_vel;
            double c_pos;
        }Ctrl;
        
        struct 
        {
            double i_ele_angle;
            double i_cur[3];    // a b c cur
            double i_ref_Iq_pu;
            double i_ref_Id_pu;

            double m_cur_alpha;
            double m_cur_beta;

            double m_Id;
            double m_Iq;

            double m_vol_alpha;
            double m_vol_beta;


            double o_Vq_pu;
            double o_Vd_pu;

            double o_Ua_pu;
            double o_Ub_pu;
            double o_Uc_pu;
        }Tor;

        struct 
        {
            double i_rpm_fb;
            double i_ref_rmp;    // a b c cur

            double o_tor_ctrl_pu;
        }Vel;
        
        struct 
        {
            double i_ref_pos;
            double i_fb_pos;

            double o_vel_ctrl_pu;
        }Pos;

    }pvt;
};

struct PmsmFoc * PmsmFoc_Constructor(struct PmsmFoc *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif
