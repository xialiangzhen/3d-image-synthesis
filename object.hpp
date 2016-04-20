//
//  object.hpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include "float.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "light.h"
#include "texture.h"
#include "projector.h"
#include "triangle.h"
#include "cyPoint.h"
using namespace std;

class object {
public:
    cyColor c, c0, c1, cs, cb, csh;
    double ks, kb, kt, ior=1, glossy=0, fresnel;
    texture t0, t1, tsh, tior;
    bool use2Dfunction=false;
    bool juliaSet=false;
    bool use3Dfunction=false;
    

    
    // common functions
    void setTexture(string file0, string file1, string file2, string file3="");
    void normalMapping(cyPoint3f& normalVec, cyPoint3f ph);
    void setColor(cyColor cc0, cyColor cc1, cyColor ccs, cyColor ccb, cyColor ccsh);
    void setKSKBKTior(double kks＝0, double kkb=0, double kkt=1, double iior=1, double gglossy=0);
    void getMixed(cyColor cl, double t, cyColor cc);
    void Diffuse(cyPoint3f nlh, cyPoint3f ph, cyColor cl);
    void getDiffuse(vector<light> alllights, cyPoint3f ph);
    void Specular(cyPoint3f nlh, cyPoint3f ph, cyPoint3f pe, cyColor cl);
    void getSpecular(vector<light> alllights, cyPoint3f ph, cyPoint3f pe);
    void Boundary(cyPoint3f ph, cyPoint3f pe, cyColor cl);
    void getBoundary(vector<light> alllights, cyPoint3f ph, cyPoint3f pe);
    void Shadow(double sourceDistance, cyPoint3f nlh, light light, cyPoint3f ph, vector<object*> allobjects);
    void ShadowSpot(cyPoint3f nl, cyPoint3f nlh, cyColor cl);
    void getShadow(vector<light> alllights, cyPoint3f ph, vector<object*> allobjects);
    void DiffuseSpotLight(cyPoint3f nlh, cyPoint3f ph, cyColor cl, cyPoint3f nl);
    void SpecularSpotLight(cyPoint3f nlh, cyPoint3f ph, cyPoint3f pe, cyColor cl, cyPoint3f nl);
    void RectanSpecular(cyPoint3f nl, cyPoint3f pl, cyColor cl, double sx, double sy, cyPoint3f n0, cyPoint3f n1, cyPoint3f ph, cyPoint3f pe);
    cyColor reflection(cyPoint3f ph, cyPoint3f pe, double k, int number, vector<object*> allobjects, vector<light> alllights);
    cyColor refraction(cyPoint3f ph, cyPoint3f pe, double k, int number, vector<object*> allobjects, vector<light> alllights );
    cyColor getColorSum(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects);
    void motionfunction(double t, cyPoint3f moveVector);
    
    
    
    
    // different functions
    virtual cyPoint3f normalVector(cyPoint3f ph){
        cyPoint3f answer(0,0,0);
        return answer;
    };
    virtual cyColor getColor(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects){
        cyColor answer(0,0,0);
        return answer;
    };
    virtual bool inObject(cyPoint3f p){return false;};
    virtual double miniIntersection(cyPoint3f pe, cyPoint3f npe){return 0;};
    virtual void getIORfromTexture(cyPoint3f ph){return;};
    virtual void translation(cyPoint3f moveVector)=0;
    
};

    
    
#endif /* object_hpp */
