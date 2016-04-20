//
//  polygon.hpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef polygon_hpp
#define polygon_hpp

#include <stdio.h>
#include "object.hpp"

class polygon: public object
{
public:
    vector<triangle> triangles;
    vector<triangle> triangles0;
    int intersection;
    
    polygon(string filePath);
    bool inObject(cyPoint3f p);
    cyPoint3f normalVector(cyPoint3f ph);
    double miniIntersection(cyPoint3f pe, cyPoint3f npe);
    cyColor getColor(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects);
    void getIORfromTexture(cyPoint3f ph);
    void translation(cyPoint3f moveVector);
};

#endif /* polygon_hpp */
