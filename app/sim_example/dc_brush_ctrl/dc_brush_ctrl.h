/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  dc brush motor ctrl test 
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.24   wangchongwei    dc brush motor ctrl test
 **/
#ifndef _DC_BRUSH_CTRL_H
#define _DC_BRUSH_CTRL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/dc_brush/dc_brush.h"
#include "components/controller/pid/pid.h"

struct DcBrushCtrl
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct DcBrushCtrl *that);
    void (* simTask)(struct DcBrushCtrl *that);

    // member
    struct 
    {
        struct PID      m_rpm_pid;

        struct DcBrush  m_DcBrush;
    }pvt;
};

struct DcBrushCtrl * DcBrushCtrl_Constructor(struct DcBrushCtrl *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _DC_BRUSH_CTRL_H */
