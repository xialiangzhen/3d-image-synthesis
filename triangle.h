//
//  triangle.h
//  demo
//
//  Created by 夏 夏 on 16/3/22.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef triangle_h
#define triangle_h

#include "cyPoint.h"

class triangle
{
public:
    cyPoint3f a, b, c;
    cyPoint3f normala, normalb, normalc, normal;
    cyPoint2f auv, buv, cuv;
    cyPoint3f area;
    
    triangle(){};
    
    triangle(cyPoint3f aa, cyPoint3f bb, cyPoint3f cc){
        a=aa; b=bb; c=cc;
        area=(b-a).Cross(c-a);
        normal=(b-a).Cross(c-a);
        normal.Normalize();
    }
    
    triangle(cyPoint3f aa, cyPoint3f bb, cyPoint3f cc, cyPoint2f aauv, cyPoint2f bbuv, cyPoint2f ccuv, cyPoint3f nnormala, cyPoint3f nnormalb, cyPoint3f nnormalc){
        a=aa; b=bb; c=cc; auv=aauv; buv=bbuv; cuv=ccuv;
        normala=nnormala; normalb=nnormalb; normalc=nnormalb;
        // if the triangle is in a plane
        area=(b-a).Cross(c-a);
        if (normala==normalb && normala==normalc){
            normal=normala;
        }
    }
    
    
    double intersection(cyPoint3f pe, cyPoint3f npe){
        if (normal.Dot(npe)==0) {
            return DBL_MAX;
        }
        double t=normal.Dot(a-pe)/normal.Dot(npe);
        cyPoint3f pp=pe+t*npe;
        
        cyPoint3f area0, area1, area2;
        area0=(b-pp).Cross(c-pp);
        area1=(c-pp).Cross(a-pp);
        area2=(a-pp).Cross(b-pp);
        if (area0.Dot(area)>0 && area1.Dot(area)>0 && area2.Dot(area)>0){
            return t;
        }
        else return DBL_MAX;
    }
    
    void map(cyPoint3f pp, float & umap, float & vmap){
        
        cyPoint3f area0, area1, area2;
        area0=(b-pp).Cross(c-pp);
        area1=(c-pp).Cross(a-pp);
        area2=(a-pp).Cross(b-pp);
        if (area0.Dot(area)>0 && area1.Dot(area)>0 && area2.Dot(area)>0){
            double w=area0.Length()/area.Length(), u=area1.Length()/area.Length(), v=area2.Length()/area.Length();
            umap=w*auv.x+u*buv.x+v*cuv.x;
            vmap=w*auv.y+u*buv.y+v*cuv.y;
        }
    }
    
    void translation(cyPoint3f moveVector){
        a=a+moveVector;
        b=b+moveVector;
        c=c+moveVector;
    }
};

#endif /* triangle_h */
