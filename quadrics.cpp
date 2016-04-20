//
//  quadrics.cpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#include "quadrics.hpp"

quadrics::quadrics (int aa02, int aa12, int aa22, int aa21, int aa00, int ss0, int ss1, int ss2, cyPoint3f ppc, cyPoint3f nn2, cyPoint3f nn1){
    a02=aa02; a12=aa12; a22=aa22; a21=aa21; a00=aa00; s0=ss0; s1=ss1; s2=ss2; pc=ppc;
    n2=nn2.GetNormalized();
    n1=n2.Cross(nn1);
    n1.Normalize();
    n0=n1.Cross(n2);
    pc0=ppc;
}


void quadrics::setProjector(int w, int h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc){
    solidTexture=true;
    p1.setProject(w, h, nn0, nn1, ppc);
}

void quadrics::setProjector(int w, int h, cyPoint3f nn0, cyPoint3f nn1, cyPoint3f ppc, cyPoint3f ppo){
    solidTexture=true;
    p1.setProject(w, h, nn0, nn1, ppc, ppo);
}

void quadrics::solidTextureColor(cyPoint3f ph){
    float u=0,v=0;
    p1.projectPosition(u, v, ph);
    c0=t0.GetColorAt(u, v);
    c1=t1.GetColorAt(u, v);
    csh=tsh.GetColorAt(u, v);
    
}

void quadrics::set3Dfunction(cyPoint3f ph){
    cyColor sphc0(40,65,18), sphc1(178,200,108), sphcs(254,249,245), sphcb(210,196,201);
    cyColor sphcc0(208,86,34), sphcc1(249,220,166), sphccs(254,249,245), sphccb(219,229,157);
    double F;
    cyPoint3f n0(0,0,1);
    cyPoint3f v=ph-pc-(ph-pc).Dot(n0)*n0;
    double T=0.5;
    F=sin(2*M_PI*v.Length()/T);
    
    if (F>0){
        c0=sphc0;
        c1=sphc1;
        cs=sphcs;
        cb=sphcb;
    }
    else {
        c0=sphcc0;
        c1=sphcc1;
        cs=sphccs;
        cb=sphccb;
    }
    
}

void quadrics::set2Dfunction(cyPoint3f ph){
    double u,v;
    //if it is an ellipsoid
    if (a02 && a12 && a22 && !a21 && a00){
        ph=(ph-pc)/s0;
        u=acos(ph.y/sqrt(1-pow(ph.z,2)))/(2*M_PI);
        v=acos(ph.z)/M_PI;
        if (ph.x<0){
            u=1-u;
        }
    }
    //if it is a plane
    if (!a02 && !a12 && !a22 && a21 && !a00){
        int size=5;
        double x=(ph-pc).Dot(n0), y=(ph-pc).Dot(n1);
        u=((int  (x)%size)+x-(int)x)/size;
        v=((int  (y)%size)+y-(int)y)/size;
    }
    
    if (u<0){
        u=u+1;
    }
    if (v<0){
        v=v+1;
    }
    double multi;
    if (juliaSet){
        u =3*u-1.5;
        v =3*v-1.5;
        double u0=-0.4, v0=0.6;
        int times=10, threshold=1;
        for (int i=0; i<times; i++) {
            double tempu=pow(u, 2)-pow(v,2)+u0;
            double tempv=2*u*v+v0;
            u=tempu;
            v=tempv;
        }
        multi=pow(u, 2)+pow(v, 2)-pow(threshold, 2);
    }
    else {
        double f1=pow(u,2)+pow(v,2)-1, f2=pow(u-1,2)+pow(v-1,2)-1, f4=pow(u-1,2)+pow(v,2)-1, f5=pow(u,2)+pow(v-1,2)-1;
        multi=f1*f2*f4*f5;
    }
    cyColor sphc0(40,65,18), sphc1(178,200,108), sphcs(254,249,245), sphcb(210,196,201);
    cyColor sphcc0(208,86,34), sphcc1(249,220,166), sphccs(254,249,245), sphccb(219,229,157);
    
    if (multi>0){
        c0=sphc0;
        c1=sphc1;
        cs=sphcs;
        cb=sphcb;
    }
    else {
        c0=sphcc0;
        c1=sphcc1;
        cs=sphccs;
        cb=sphccb;
    }
}



void quadrics::textureSetColor(cyPoint3f ph){
    double u,v;
    //if it is an ellipsoid
    if (a02 && a12 && a22 && !a21 && a00){
        ph=(ph-pc).GetNormalized();
        if (ph.y/sqrt(1-pow(ph.z,2))>1){
            u=acos(1)/(2*M_PI);
        }
        else if (ph.y/sqrt(1-pow(ph.z,2))<-1){
            u=acos(-1)/(2*M_PI);
        }
        else{
            u=acos(ph.y/sqrt(1-pow(ph.z,2)))/(2*M_PI);
        }
        v=acos(ph.z)/M_PI;
        if (ph.x<0){
            u=1-u;
        }
    }
    //if it is a plane
    if (!a02 && !a12 && !a22 && a21 && !a00){
        int size=100;
        double x=(ph-pc).Dot(n0), y=(ph-pc).Dot(n1);
        u=((int  (x)%size)+x-(int)x)/size;
        v=((int  (y)%size)+y-(int)y)/size;
    }
    c0=t0.GetColorAt(u, v);
    c1=t1.GetColorAt(u, v);
    csh=tsh.GetColorAt(u, v);
}


cyPoint3f quadrics::normalVector(cyPoint3f ph){
    cyPoint3f Fp=2*a02*n0.Dot(ph-pc)/pow(s0, 2)*n0+2*a12*n1.Dot(ph-pc)/pow(s1, 2)*n1+2*a22*n2.Dot(ph-pc)/pow(s2, 2)*n2+a21/s2*n2;
    Fp.Normalize();
    if (t1.flag || use2Dfunction || use3Dfunction){
        normalMapping(Fp, ph);
    }
    return Fp;
}


cyColor quadrics::getColor(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects){
    if (t1.flag && solidTexture){
        solidTextureColor(ph);
    }
    if (t1.flag && !solidTexture) {
        textureSetColor(ph);
    }
    if (use2Dfunction){
        set2Dfunction(ph);
    }
    if (use3Dfunction) {
        set3Dfunction(ph);
    }
    c=c0;
    getDiffuse(alllights, ph);
    getSpecular(alllights, ph, pe);
    getBoundary(alllights, ph, pe);
    getShadow(alllights, ph, allobjects);
    return c;
    
}





bool quadrics::inObject(cyPoint3f p){
    double F;
    F=a02*pow(n0.Dot(p-pc)/s0,2)+a12*pow(n1.Dot(p-pc)/s1,2)+a22*pow(n2.Dot(p-pc)/s2,2)+a21/s2*n2.Dot(p-pc)+a00;
    if (F<0){
        return true;
    }
    else return false;
}

double quadrics::miniIntersection(cyPoint3f pe, cyPoint3f npe){
    double A, B, C, D, t1, t2, t;
    A=a02*pow(n0.Dot(npe)/s0,2)+a12*pow(n1.Dot(npe)/s1,2)+a22*pow(n2.Dot(npe)/s2,2);
    B=a02*2*n0.Dot(npe)*n0.Dot(pe-pc)/pow(s0, 2)+a12*2*n1.Dot(npe)*n1.Dot(pe-pc)/pow(s1, 2)+a22*2*n2.Dot(npe)*n2.Dot(pe-pc)/pow(s2, 2)+a21/s2*n2.Dot(npe);
    C=a02*pow(n0.Dot(pe-pc)/s0,2)+a12*pow(n1.Dot(pe-pc)/s1,2)+a22*pow(n2.Dot(pe-pc)/s2,2)+a21/s2*n2.Dot(pe-pc)+a00;
    D=pow(B,2)-4*A*C;
    if(A==0){
        t=-C/B;
    }
    else {
        if (D<0){
            return -1;
        }
        else if (D==0){
            t=(-B-sqrt(D))/(2*A);
        }
        else {
            t1=(-B-sqrt(D))/(2*A);
            t2=(-B+sqrt(D))/(2*A);
            if (fabs(t1)<0.01) {
                t1=0;
            }
            if (fabs(t2)<0.01) {
                t2=0;
            }
            if (t1<0 && t2>=0) {
                t=t2;
            }
            else if (t2<0 && t1>=0){
                t=t1;
            }
            else if (t1>=0 && t2>=0)
                t=(t1<t2)?t1:t2;
            else t=-1;

        }
    }
    return t;
}


void quadrics::getIORfromTexture(cyPoint3f ph){
    ph=(ph-pc)/s0;
    double u,v;
    if (ph.y/sqrt(1-pow(ph.z,2))>1){
        u=acos(1)/(2*M_PI);
    }
    else if (ph.y/sqrt(1-pow(ph.z,2))<-1){
        u=acos(-1)/(2*M_PI);
    }
    else{
        u=acos(ph.y/sqrt(1-pow(ph.z,2)))/(2*M_PI);
    }
    v=acos(ph.z)/M_PI;
    if (ph.x<0){
        u=1-u;
    }
    cyColor cior=tior.GetColorAt(u,v);
    ior=cior.r/255+cior.g/255+cior.b/255;
    ior=ior/6+1;

}

void quadrics::translation(cyPoint3f moveVector){
    pc=pc0+moveVector;
}
