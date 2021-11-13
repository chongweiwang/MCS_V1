/**
 * @copyright  Copyright 2021 zerodrive wangchongwei 
 * @license:    Apache-2.0 License  
 * @brief:  PMSM test
 * @details:
 * @changelog:
 * date         author          notes
 * 2021.10.21   wangchongwei    just test pmsm model
 **/

#ifndef _TEST_PMSM_H
#define _TEST_PMSM_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "components/service/base_obj.h"
#include "components/simscape/electrical/pmsm/pmsm.h"
#include "components/math/coordinate/coordinate.h" 


struct TstPmsm
{
    CLASS_BaseObj(m_obj);

    // method
    void (* init)(struct TstPmsm *that);
    void (* simTask)(struct TstPmsm *that);

    // member
    struct 
    {
        struct Pmsm  m_Pmsm;
    }pvt;
};

struct TstPmsm * TstPmsm_Constructor(struct TstPmsm *that);




#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */


#endif /* _TEST_PMSM_H */
