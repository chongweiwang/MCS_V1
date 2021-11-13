/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  dc brush motor test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.12   wangchongwei    test dc brush motor model
 **/

#ifndef _TEST_DC_BRUSH_H
#define _TEST_DC_BRUSH_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/dc_brush/dc_brush.h"


struct TstDcBrush
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct TstDcBrush *that);
    void (* simTask)(struct TstDcBrush *that);

    // member
    struct 
    {
        struct DcBrush  m_DcBrush;
    }pvt;
};

struct TstDcBrush * TstDcBrush_Constructor(struct TstDcBrush *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _TEST_DC_BRUSH_H */
