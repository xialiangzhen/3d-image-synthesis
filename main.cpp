// =============================================================================
// VIZA656/CSCE647 Image Synthesis at Texas A&M University
// Project 10: Distributed Ray Tracing
// Created by Liangzhen Xia based from Ariel Chisholm's template
// 05.23.2011
//
// This file is supplied with an associated makefile. Put both files in the same
// directory, navigate to that directory from the Linux shell, and type 'make'.
// This will create a program called 'pr01' that you can run by entering
// 'homework0' as a command in the shell.
//
// If you are new to programming in Linux, there is an
// excellent introduction to makefile structure and the gcc compiler here:
//
// http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf
//
// =============================================================================

#include <cstdlib>
#include <iostream>
#include <GLUT/GLUT.h>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "cyPoint.h"
#include "cyColor.h"
#include "object.hpp"
#include "quadrics.hpp"
#include "polygon.hpp"
#include "light.h"



using namespace std;

// =============================================================================
// These variables
// =============================================================================

// picture parameters
int width = 300, height = 400;
int subpixM= 3, subpixN=4;
unsigned char *pixmap = new unsigned char[width * height * 3];

// light parameters
cyPoint3f nl(0,0,-1), pl(20,0,40);
cyPoint3f nl2(1,1,-0.5), pl2(0,-30,15);
cyColor cl(255,255,255),cl2(0,255,0);
vector<light> alllights;

// light parameters for rectangular light
double lsx=20,lsy=20;
cyPoint3f ln0(1,0,0), ln1(0,1,0);


// camera parameters
int Sx = 60, Sy = 90; // field of view
cyPoint3f eye(0,0,10), midPosition(20,0,10), cn00(0,-1,0);
// midPosition is the mid point of camera, randomVector is used to calculate cn0,cn1,cn2, that is to rotate the camera
cyPoint3f iniPosition, cn0, cn1, cn2;
bool outFocus=false, softshadow=true, motionblur=false;
double Sex=1.5, Sey=2;


// spheres parameters
cyPoint3f sphereCenter1(20,0,2), sphereCenter2(30,10,10);
quadrics sphere1(1,1,1,0,-1,10,10,10,sphereCenter1);
cyColor sphc0(40,65,18), sphc1(178,200,108), sphcs(249,242,243), sphcb(210,196,201);
quadrics sphere2(1,1,1,0,-1,5,5,5,sphereCenter2);
cyColor sphcc0(208,86,34), sphcc1(249,220,166), sphccs(249,242,243), sphccb(219,229,157);

cyPoint3f sphereCenter3(40,20,20), sphereCenter4(50,30,30);
quadrics sphere3(1,1,1,0,-1,5,5,5,sphereCenter3);
cyColor sphcc03(197,98,34),sphcc13(234,228,107);
quadrics sphere4(1,1,1,0,-1,5,5,5,sphereCenter4);
cyColor sphcc04(211,139,40),  sphcc14(242,220,53);
quadrics sphere5(1,1,1,0,-1,2,2,2,cyPoint3f(0,0,0));
quadrics background(1,1,1,0,-1,200,200,200,cyPoint3f(0,0,0));

cyColor shadow(9,9,10);


// planes parameters
cyPoint3f pc1(50,0,-10), pc2(50,0,-10), pn1(0,0,1), pn2(-1,0,0);
quadrics plane2(0,0,0,1,0,1,1,1,pc1,pn1);
quadrics plane1(0,0,0,1,0,1,1,1,pc2,pn2);
cyColor plac0(55,67,57), plac1(88,162,165), placs(129,179,175), placb(210,196,201);
cyColor placc0(20,10,11), placc1(140,53,37), placcs(107,102,100), placcb(219,229,157);


// objects set
vector<object*> allobjects;

// texture file
string wallpaperC0="wallpaperC0.bmp", wallpaperC1="wallpaper.bmp", wallpaperCsh="wallpaperCsh.bmp";
string woodC0="woodC0.bmp", wood="wood.bmp", woodCsh="woodCsh.bmp";
string back="background.bmp", text="texture3.bmp";

//projector position
cyPoint3f proj0(0,1,0), proj1(0,0,1), projc(1.5,0,-5),  projo(0,0,-5);
int projwidth=100, projheight=100;

//polygons obj file
string cube="cube.obj";
string tetrahedron="tetrahedron.obj";
string shuttle="shuttle.obj", icosahedron="icosahedron.obj";
string icosphere="icosphere.obj";



// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================/Users/xiaxia/Documents/tamu 2016spring/647 image synthesis/647_project1/screen snap/main.cpp
static void windowResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void windowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
    glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
    if(state == GLUT_UP)
        exit(0);               // Exit on mouse click.
}
static void init(void)
{
    glClearColor(1,1,1,1); // Set background color.
}


// =============================================================================
// Liangzhen's Helper Methods
// =============================================================================

void saveLights(){
    nl.Normalize();
    light area(1, nl, pl, cl, lsx, lsy, ln0, ln1);
    alllights.push_back(area);

}


void saveObjects(){
    object* point=&background;
//    object* point = new polygon(icosahedron);
    allobjects.push_back(point);
//    point = new polygon(tetrahedron);
//    allobjects.push_back(point);
//    point = new polygon(cube);
//    allobjects.push_back(point);
//    object* point = &sphere1;
//    allobjects.push_back(point);
//    point = &sphere3;
//    allobjects.push_back(point);
//    point = &sphere4;
//    allobjects.push_back(point);
//    point = &plane2;
//    allobjects.push_back(point);
//    point = &plane2;
//    allobjects.push_back(point);
//    point = &plane2;
//    allobjects.push_back(point);
    
    point = &sphere1;
    allobjects.push_back(point);
//    point = &sphere2;
//    allobjects.push_back(point);
//    point = &sphere3;
//    allobjects.push_back(point);
//    point = &sphere4;
//    allobjects.push_back(point);
        point = &plane2;
        allobjects.push_back(point);

    
}

void preparation(){
    cn2=midPosition-eye;
    cn2.Normalize();
    cn0=cn00.GetNormalized();
    cn1=cn0.Cross(cn2); // left hand coordinate
    iniPosition=midPosition-Sx/2*cn0-Sy/2*cn1;
}

cyPoint3f coordiTrans(double x, double y){
    cyPoint3f position;
    position = iniPosition + x/width*Sx*cn0 + y/height*Sy*cn1;
    return position;
}


void setColor(){
    allobjects[2]->setColor(sphc0, sphc1, sphcs, sphcb, shadow);
//    allobjects[2]->setColor(sphcc0, sphcc1, sphccs, sphccb, shadow);
//    sphere2.setColor(sphcc0, sphcc1, sphccs, sphccb, shadow);
//    sphere3.setColor(sphcc03, sphcc13, sphccs, sphccb, shadow);
//    sphere4.setColor(sphcc04, sphcc14, sphccs, sphccb, shadow);
//    plane1.setColor(plac0, plac1, placs, placb, shadow);
//    plane2.setColor(placc0, placc1, placcs, placcb, shadow);
    
}
void setKSKBKTior(){
    // ks, kb, kt, ios，glossy
//    allobjects[0]->setKSKBKTior(0, 0, 0);
    allobjects[1]->setKSKBKTior(1, 0, 1 , 0.01, 0); // ios->0 fresnel->1 all reflection
//    allobjects[2]->setKSKBKTior(1, 0, 1 , 0.01, 0);
//    allobjects[3]->setKSKBKTior(1, 0, 1 , 0.01, 0);
//    allobjects[4]->setKSKBKTior(1, 0, 1 , 0.01, 0);
//    allobjects[2]->setKSKB(0.7, 0);
//    allobjects[2]->setKSKB(0.1, 0);
//    sphere2.setKSKB(0.1, 0.3);
//    sphere3.setKSKB(0.1, 0.3);
//    sphere4.setKSKB(0.1, 0.3);
//    plane1.setKSKB(0.1, 0.1);
//    plane2.setKSKB(0.1, 0.1);
}

void setTexture(){
    allobjects[0]->setTexture(back, back, back);
//    allobjects[1]-> use3Dfunction=true;
//    allobjects[1]-> juliaSet=true;
//    allobjects[2]->setTexture(text, text, text, text);
//    allobjects[2]->setTexture(wallpaperC0, wallpaperC1, wallpaperCsh);
//    sphere2.setTexture(woodC0, wood, woodCsh);
//    sphere3.setTexture(woodC0, wood, woodCsh);
//    sphere4.setTexture(woodC0, wood, woodCsh);
////    plane1.use2Dfunction=true;
////    plane1.juliaSet=true;
////    plane2.setTexture(wallpaperC0, wallpaperC1, wallpaperCsh);
//    sphere1.setTexture(wallpaperC0, wallpaperC1, wallpaperCsh);
//
////    plane1.setProjector(projwidth, projheight, proj0, proj1, projc, projo);
////    plane2.setProjector(projwidth, projheight, proj0, proj1, projc, projo);
////    sphere1.setProjector(projwidth, projheight, proj0, proj1, projc, projo);
////    plane1.setProjector(projwidth, projheight, proj0, proj1, projc);
////    plane2.setProjector(projwidth, projheight, proj0, proj1, projc);
//    sphere1.setProjector(projwidth, projheight, proj0, proj1, projc);
//    sphere2.setProjector(projwidth, projheight, proj0, proj1, projc);
//    sphere3.setProjector(projwidth, projheight, proj0, proj1, projc);
//    sphere4.setProjector(projwidth, projheight, proj0, proj1, projc);
    
//    sphere2.use3Dfunction=true;
//    sphere3.use3Dfunction=true;
//    sphere4.use3Dfunction=true;
    
    
//    plane1.setProjector(projwidth, projheight, proj0, proj1, projc);
//    plane2.setProjector(projwidth, projheight, proj0, proj1, projc);
//    sphere1.setProjector(projwidth, projheight, proj0, proj1, projc);

}

void checkEye(){
    // the sphere 0 is the background sphere
    for (int i=1; i<allobjects.size(); i++) {
        if (allobjects[i]->inObject(eye)) {
            cout<<"Caution! The eyes are inside the object No. "<< i <<endl;
            exit(1);
        }
    }
}


// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setPixels()
{
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            cyColor color(0,0,0);
            double r0 = ((double) rand() / (RAND_MAX));
            double r1 = ((double) rand() / (RAND_MAX));
            int k0= int (subpixM*(rand()/RAND_MAX));
            int k1= int (subpixN*(rand()/RAND_MAX));
            for (int m=0; m<subpixM; m++){
                for (int n=0; n<subpixN; n++){
                    double X=x+(m+r0)/subpixM;
                    double Y=y+(n+r1)/subpixN;
                    cyPoint3f position = coordiTrans(X,Y);
                    cyPoint3f eyePosition;
                    // out of focus
                    if (outFocus){
                        eyePosition=eye+((m+k0)%subpixM+r0)/subpixM*cn0*Sex+((n+k1)%subpixN+r1)/subpixN*cn1*Sey;
                    }
                    else eyePosition=eye;
                    
                    //soft shadow
                    if (softshadow) {

                        for (int i=0; i<alllights.size(); i++ ) {
                            alllights[i].pl=alllights[i].originalpl+((m+k0)%subpixM+r0)/subpixM*alllights[i].n0*alllights[i].sx+((n+k1)%subpixN+r1)/subpixN*alllights[i].n1*alllights[i].sy;
                        }

                    }
                    
                    //motion blur
                    if (motionblur) {
                        double time=double(m*subpixM+n)/double(subpixM*subpixN);
                        cyPoint3f vector(0,2,0);
                        allobjects[1]->motionfunction(time, vector);
                        
                    }
                    
                    double tmin=DBL_MAX;
                    cyPoint3f npe=(position-eyePosition).GetNormalized();
                    cyColor newColor(0,0,0);
                    double t;
                    object* closest = nullptr;
                    
                    for (int i=0; i<allobjects.size(); i++){
                        t=allobjects[i]->miniIntersection(eyePosition, npe);
                        if (t>0 && t<tmin) {
                            tmin=t;
                            closest=allobjects[i];
                        }
                    }
                    if (tmin<DBL_MAX){
                        cyPoint3f ph=eyePosition+tmin*npe;
                        newColor=closest->getColorSum(alllights, ph, eyePosition, allobjects);
                        color=color+newColor;
                    }
                }
            }
            pixmap[i++]=color.r/(subpixM*subpixN);
            pixmap[i++]=color.g/(subpixM*subpixN);
            pixmap[i]=color.b/(subpixM*subpixN);
        }
    }
}


// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    preparation();
    saveObjects();
    setColor();
    setKSKBKTior();
    setTexture();

    saveLights();
    checkEye();
    

    setPixels();
    
    
    
    
    // OpenGL Commands:
    // Once "glutMainLoop" is executed, the program loops indefinitely to all
    // glut functions.
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Project 10: Distributed Ray Tracing");
    init();
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    glutMouseFunc(processMouse);
    glutMainLoop();
    
    return 0; //This line never gets reached. We use it because "main" is type int.
}

