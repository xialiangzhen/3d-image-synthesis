//
//  quadrics.hpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef quadrics_hpp
#define quadrics_hpp

#include <stdio.h>
#include "object.hpp"

class quadrics: public object
{
public:
    int a02, a12, a22, a21, a00;
    int s0, s1, s2;
    cyPoint3f pc, pc0, n0, n1, n2;
    //    cyColor c, c0, c1, cs, cb, csh;
    //    double ks, kb;
    //    texture t0, t1, tsh;
    //    bool useTexture=false;
//    bool use2Dfunction=false;
//    bool juliaSet=false;
    
    bool solidTexture=false;
    projector p1;
    
//    bool use3Dfunction=false;
    
    quadrics(){}
    quadrics (int aa02, int aa12, int aa22, int aa21, int aa00, int ss0, int ss1, int ss2, cyPoint3f ppc, cyPoint3f nn2=cyPoint3f(1,0,0), cyPoint3f nn1=cyPoint3f(0,1,0));
    
    void setProjector(int w, int h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc);
    void setProjector(int w, int h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc, cyPoint3f ppo);
    void solidTextureColor(cyPoint3f ph);
    void set3Dfunction(cyPoint3f ph);
    void set2Dfunction(cyPoint3f ph);
    void textureSetColor(cyPoint3f ph);
    cyPoint3f normalVector(cyPoint3f ph);
    cyColor getColor(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects);
    bool inObject(cyPoint3f p);
    double miniIntersection(cyPoint3f pe, cyPoint3f npe);
    void getIORfromTexture(cyPoint3f ph);
    void translation(cyPoint3f moveVector);
    

};

#endif /* quadrics_hpp */
