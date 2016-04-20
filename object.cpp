//
//  object.cpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#include "object.hpp"

void object::setTexture(string file0, string file1, string file2, string file3){
    t0.setPixmap(file0);
    t1.setPixmap(file1);
    tsh.setPixmap(file2);
    if (file3!="") {
        tior.setPixmap(file3);
    }
}

void object::normalMapping(cyPoint3f& normalVec, cyPoint3f ph){
    double r=c1.r/255, g=c1.g/255, b=c1.b/255;
    cyPoint3f v(2*r-1, 2*g-1, 2*b-1);
    double s=1;
    normalVec=normalVec+s*v;
    normalVec.Normalize();
}

void object::setColor(cyColor cc0, cyColor cc1, cyColor ccs, cyColor ccb, cyColor ccsh){
    c0=cc0;
    c1=cc1;
    cs=ccs;
    cb=ccb;
    csh=ccsh;
}

void object::setKSKBKTior(double kks, double kkb, double kkt, double iior, double gglossy){
    ks=kks;
    kb=kkb;
    kt=kkt;
    ior=iior;
    glossy=gglossy;
}

void object::getMixed(cyColor cl, double t, cyColor cc){
    cyColor I(255,255,255), T;
    cl=cl/I;
    I=I/I;
    T=cl*t;
    c=c*(I-T)+cc*T;
}

void object::Diffuse(cyPoint3f nlh, cyPoint3f ph, cyColor cl){
    cyPoint3f nh=normalVector(ph);
    double cos=nh.Dot(nlh), t=(cos+1)/2;
    getMixed(cl, t, c1);
}

void object::getDiffuse(vector<light> alllights, cyPoint3f ph){
    cyPoint3f nlh;
    for (int i=0; i<alllights.size(); i++) {
        if (alllights[i].type==0) {
            nlh=-alllights[i].nl;
            Diffuse(nlh, ph, alllights[i].cl);
        }
        if (alllights[i].type==1) {
            nlh=(alllights[i].pl-ph).GetNormalized();
            Diffuse(nlh, ph, alllights[i].cl);
        }
        if (alllights[i].type==2) {
            nlh=(alllights[i].pl-ph).GetNormalized();
            DiffuseSpotLight(nlh, ph, alllights[i].cl, alllights[i].nl);
        }
    }
}



void object::Specular(cyPoint3f nlh, cyPoint3f ph, cyPoint3f pe, cyColor cl){
    cyPoint3f nh=normalVector(ph), rlh=-nlh+2*(nh.Dot(nlh))*nh, nep=pe-ph;
    nep.Normalize();
    double cos=rlh.Dot(nep), max=1, min=0.95;
    double s=(cos-min)/(max-min);
    if (s>1) {
        s=1;
    }
    if (s<0){
        s=0;
    }
    s=s*ks;
    getMixed(cl, s, cs);
}

void object::getSpecular(vector<light> alllights, cyPoint3f ph, cyPoint3f pe){
    cyPoint3f nlh;
    for (int i=0; i<alllights.size(); i++) {
        if (alllights[i].type==0) {
            nlh=-alllights[i].nl;
            Specular(nlh, ph, pe, alllights[i].cl);
        }
        if (alllights[i].type==1) {
            nlh=(alllights[i].pl-ph).GetNormalized();
            Specular(nlh, ph, pe, alllights[i].cl);
        }
        if (alllights[i].type==2) {
            nlh=(alllights[i].pl-ph).GetNormalized();
            SpecularSpotLight(nlh, ph, pe,alllights[i].cl, alllights[i].nl);
        }
    }
}


void object::Boundary(cyPoint3f ph, cyPoint3f pe, cyColor cl){
    cyPoint3f neh=(pe-ph).GetNormalized(),nh=normalVector(ph);
    double b=1-neh.Dot(nh), max=0.95, min=0.9;
    b=(b-min)/(max-min);
    if (b>1){
        b=1;
    }
    if (b<0){
        b=0;
    }
    b=b*kb;
    getMixed(cl, b, cb);
}

void object::getBoundary(vector<light> alllights, cyPoint3f ph, cyPoint3f pe){
    for (int i=0; i<alllights.size(); i++) {
        Boundary(ph, pe, alllights[i].cl);
    }
}


//void object::Shadow(double sourceDistance, cyPoint3f nlh, cyColor cl, cyPoint3f ph, vector<object*> allobjects){
//    double  t=-1, sh=0;
//    cyPoint3f nh=normalVector(ph);
//    double cos=nh.Dot(nlh);
//    if (cos<0) {
//        sh=0.5;
//        getMixed(cl, sh, csh);
//        return;
//    }
//    for (int i=0; i<allobjects.size(); i++){
//        t=allobjects[i]->miniIntersection(ph, nlh);
//        if (t>1e-3 && t<sourceDistance){
//            sh=0.5;
//            break;
//        }
//    }
//    getMixed(cl, sh, csh);
//}

void object::Shadow(double sourceDistance, cyPoint3f nlh, light light , cyPoint3f ph, vector<object*> allobjects){
    double  t=-1, sh=0;
    cyPoint3f nh=normalVector(ph);
    double cos=nh.Dot(nlh);
    if (cos<0) {
        sh=0.5;
        getMixed(light.cl, sh, csh);
        return;
    }
    for (int i=0; i<allobjects.size(); i++){
        t=allobjects[i]->miniIntersection(ph, nlh);
        if (t>1e-20 && t<sourceDistance){
            sh=0.5;
            break;
        }
    }
    getMixed(light.cl, sh, csh);
}


void object::ShadowSpot(cyPoint3f nl, cyPoint3f nlh, cyColor cl){
    double sh=0;
    if (nl.Dot(-nlh)<0.6){
        sh=0.5;
    }
    getMixed(cl, sh, csh);
}

void object::getShadow(vector<light> alllights, cyPoint3f ph, vector<object*> allobjects){
    cyPoint3f nlh;
    double sourceDistance=DBL_MAX;
    for (int i=0; i<alllights.size(); i++) {
        if (alllights[i].type==0) {
            nlh=-alllights[i].nl;
            Shadow(sourceDistance, nlh, alllights[i], ph, allobjects);
        }
        if (alllights[i].type==1){
            nlh=(alllights[i].pl-ph).GetNormalized();
            sourceDistance= (alllights[i].pl-ph).Length();
            Shadow(sourceDistance, nlh, alllights[i], ph, allobjects);
        }
        if (alllights[i].type==2){
            nlh=(alllights[i].pl-ph).GetNormalized();
            sourceDistance= (alllights[i].pl-ph).Length();
            ShadowSpot(alllights[i].nl, nlh, alllights[i].cl);
            Shadow(sourceDistance, nlh, alllights[i], ph, allobjects);
        }
        
    }
    
}



void object::DiffuseSpotLight(cyPoint3f nlh, cyPoint3f ph, cyColor cl, cyPoint3f nl){
    cyPoint3f nh=normalVector(ph);
    double cos1=nlh.Dot(nh), cos2=-nlh.Dot(nl), t=(cos1+1)*(cos2+1)/4;
    if (cos2<0.6) {
        t=0.1;
    }
    getMixed(cl, t, c1);
}

void object::SpecularSpotLight(cyPoint3f nlh, cyPoint3f ph, cyPoint3f pe, cyColor cl, cyPoint3f nl){
    cyPoint3f nh=normalVector(ph), rlh=-nlh+2*(nh.Dot(nlh))*nh, nep=pe-ph;
    nep.Normalize();
    double cos1=rlh.Dot(nep), cos2=-nlh.Dot(nl), cos=cos1*cos2, max=0.95, min=0.9;
    double s=(cos-min)/(max-min);
    if (s>1) {
        s=1;
    }
    if (s<0){
        s=0;
    }
    s=s*ks;
    getMixed(cl, s, cs);
    
}


void object::RectanSpecular(cyPoint3f nl, cyPoint3f pl, cyColor cl, double sx, double sy, cyPoint3f n0, cyPoint3f n1, cyPoint3f ph, cyPoint3f pe){
    cyPoint3f nh=normalVector(ph), npe=ph-pe;
    npe.Normalize();
    cyPoint3f reh=npe-2*(npe.Dot(nh))*nh;
    double k0=reh.Dot(n0), k2=reh.Dot(nl), k1=reh.Dot(n1);
    double x=(ph-pl).Dot(n0-nl*(k0/k2));
    double y=(ph-pl).Dot(n1-nl*(k1/k2));
    double s=(fabs(x)/sx)< (fabs(y)/sy) ? (fabs(y)/sy):(fabs(x)/sx);
    if (s<1 && k2<0) {
        s=1;
    }
    else {
        s=0;
    }
    s=s*ks;
    getMixed(cl, s, cs);
    
}



cyColor object::reflection(cyPoint3f ph, cyPoint3f pe, double k, int number, vector<object*> allobjects, vector<light> alllights){
    
    if ((k*ks)<0.02 || number>1) {
        return getColor(alllights, ph, pe, allobjects);
    }
    cyPoint3f n=normalVector(ph), v=(pe-ph).GetNormalized();
    cyPoint3f r=-v+2*(n.Dot(v))*n;
    double x=(rand() / (RAND_MAX / 2) - 1)*glossy, y=(rand() / (RAND_MAX / 2) - 1)*glossy, z=(rand() / (RAND_MAX / 2) - 1)*glossy;
    cyPoint3f vr(x,y,z);
    r=r+vr;
    r.Normalize();
    double t, tmin=DBL_MAX;
    object* closest = nullptr;
    
    for (int i=0; i<allobjects.size(); i++){
        t=allobjects[i]->miniIntersection(ph, r);
        if (t>0 && t<tmin) {
            tmin=t;
            closest=allobjects[i];
            
        }
    }
    if (tmin<DBL_MAX){
        cyPoint3f nextPh=ph+tmin*r;
        cyColor newColor=closest->reflection(nextPh, ph, k*ks, number+1, allobjects, alllights);
        cyColor color=c*(1-ks)+newColor*ks;
        return color;
    }
    else {
        cyColor black(10,10,10);
        return c*(1-ks)+black*ks;
    }
}


cyColor object::refraction(cyPoint3f ph, cyPoint3f pe, double k, int number, vector<object*> allobjects, vector<light> alllights ){
    if ((k*kt)<0.02 || number>1) {
        return getColor(alllights, ph, pe, allobjects);
    }
    if (tior.flag) {
        getIORfromTexture(ph);
    }
    cyPoint3f n=normalVector(ph), v=(pe-ph).GetNormalized(), tr;
    double iior=ior;
    if (v.Dot(n)<0) {
        n=-n;
        iior=1/ior;
    }
    if (log2(iior)<0) {
        tr=-v*(1+log2(iior))+n*log2(iior);
    }
    else{
        tr=-v*(1-log2(iior))+(-v+n.Dot(v)*n)*log2(iior);
    }
    tr.Normalize();
    
    // fresnel coefficient
    double cos1=n.Dot(v), cos2=-n.Dot(tr);
    fresnel=fabs((ior*cos2-cos1)/(ior*cos2+cos1));
    
    double t, tmin=DBL_MAX;
    object* closest = nullptr;
    
    for (int i=0; i<allobjects.size(); i++){
        t=allobjects[i]->miniIntersection(ph, tr);
        if (t>0 && t<tmin) {
            tmin=t;
            closest=allobjects[i];
        }
    }
    if (tmin<DBL_MAX){
        cyPoint3f nextPh=ph+tmin*tr;
        cyColor newColor=closest->refraction(nextPh, ph, k*kt, number+1, allobjects, alllights);
        cyColor color=c*(1-kt)+newColor*kt;
        return color;
    }
    else {
        cyColor black(10,10,10);
        return c*(1-kt)+black*kt;
    }

}

cyColor object::getColorSum(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects){
    cyColor newColor;
//    if (ks > 0.02) {
//        newColor=reflection(ph, pe, 1, 0, allobjects, alllights);
//    }
//    else if (kt > 0.02) {
//        newColor=refraction(ph, pe, 1, 0, allobjects, alllights);
//    }
//    else newColor=getColor(alllights, ph, pe, allobjects);
    
    // fresnel equation
    cyColor cdirect=getColor(alllights, ph, pe, allobjects), cr=reflection(ph, pe, 1, 0, allobjects, alllights), ct=refraction(ph, pe, 1, 0, allobjects, alllights);

    cyColor cindirect=cr*fresnel+ct*(1-fresnel);
    newColor=cdirect*(1-kt)+cindirect*kt;
    return newColor;

}

void object::motionfunction(double t, cyPoint3f moveVector){
    translation(t*moveVector);
}


