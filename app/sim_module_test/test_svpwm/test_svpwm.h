/**
 * @copyright   Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:      svpwm test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.11.25   wangchongwei    svpwm test
 **/

#ifndef _TEST_SVPWM_H
#define _TEST_SVPWM_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/pmsm/pmsm.h"
#include "components/simscape/electrical/pmsm/pmsm.h"
#include "components/simscape/modulation/svpwm/svpwm.h"


struct TstSvpwm
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct TstSvpwm *that);
    void (* simTask)(struct TstSvpwm *that);

    // member
    struct 
    {
        struct Svpwm m_svpwm;
    }pvt;
};

struct TstSvpwm * TstSvpwm_Constructor(struct TstSvpwm *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _TEST_SVPWM_H */
