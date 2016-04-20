//
//  light.h
//  demo
//
//  Created by 夏 夏 on 16/2/26.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef light_h
#define light_h

#include "cyPoint.h"
#include "cyColor.h"

class light {
public:
    int type;  // 0 directed, 1 point, 2 spot
    cyPoint3f nl, pl, n0, n1;
    cyPoint3f originalpl;
    cyColor cl;
    double sx, sy;
    
    light(){
    }
    light(int ttype, cyPoint3f nnl, cyPoint3f ppl, cyColor ccl, double ssx=5, double ssy=5, cyPoint3f nn0=cyPoint3f(1,0,0), cyPoint3f nn1=cyPoint3f(0,1,0)): type(ttype), nl(nnl), pl(ppl), cl(ccl), sx(ssx), sy(ssy){
        n0=nn0;
        n1=nn1;
        originalpl=ppl-sx/2*n0-sy/2*n1;
    }
};


#endif /* light_h */
