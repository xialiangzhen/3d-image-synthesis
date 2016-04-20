//
//  projector.h
//  demo
//
//  Created by 夏 夏 on 16/3/20.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef projector_h
#define projector_h
#include "cyPoint.h"

class projector
{
public:
    cyPoint3f po, p00, ph, n0, n1;
    double Height=0, Width=0;
    string type="";
    
    void setProject(double w, double h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc, cyPoint3f ppo){
        Width=w;
        Height=h;
        n0=nn0;
        n1=nn1;
        p00=ppc-w/2*n0-h/2*n1;
        po=ppo;
        type="spot";
    }
    void setProject(double w, double h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc){
        Width=w;
        Height=h;
        n0=nn0;
        n1=nn1;
        p00=ppc-w/2*n0-h/2*n1;
        type="parallel";
    }

    
    
    void projectPosition(float & u, float & v, cyPoint3f ph){
        cyPoint3f n2=n1.Cross(n0);
        cyPoint3f noh;
        if (type=="parallel") {
            noh=n2;
        }
        else noh=(po-ph).GetNormalized();
        double t=n2.Dot(p00-ph)/n2.Dot(noh);
        cyPoint3f pp=ph+t*noh;
        u=n0.Dot(pp-p00)/Width, v=n1.Dot(pp-p00)/Height;
    }

};


#endif /* projector_h */
