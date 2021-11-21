/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      pid test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.21   wangchongwei    pid test
 **/

#ifndef _TEST_PID_H
#define _TEST_PID_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/dc_brush/dc_brush.h"

#include "components/controller/pid/pid.h"
#include "components/controller/pid/back_cal_pid.h"
#include "components/controller/pid/dclamp_pid.h"
#include "components/controller/pid/inc_pid.h"
#include "components/controller/pid/sclamp_pid.h"

struct TstPid
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct TstPid *that);
    void (* simTask)(struct TstPid *that);

    // member
    struct 
    {
        struct PID          m_rpm_pid;
        struct BackCalPID   m_rpm_backcal_pid;
        struct DClampPID    m_rpm_dclamp_pid;
        struct SClampPID    m_rpm_sclamp_pid;
        struct IncPID       m_rpm_inc_pid;

        struct DcBrush  m_DcBrush;
    }pvt;
};

struct TstPid * TstPid_Constructor(struct TstPid *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _TEST_DC_BRUSH_H */
