//
//  polygon.cpp
//  demo
//
//  Created by 夏 夏 on 16/3/30.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#include "polygon.hpp"

polygon:: polygon(string filePath){
    int TempNumOne = filePath.size();
    char path[100];
    for (int a = 0; a <= TempNumOne; a++)
    {
        path[a] = filePath[a];
    }
    vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    vector< cyPoint3f > temp_vertices;
    vector< cyPoint2f > temp_uvs;
    vector< cyPoint3f > temp_normals;
    FILE * file = fopen(path, "r");
    if (file == NULL){
        printf("Impossible to open the file !\n");
        return;
    }
    bool flag=false;
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        if ( strcmp( lineHeader, "v" ) == 0 ){
            cyPoint3f vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            flag=true;
            cyPoint2f uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            cyPoint3f normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            if (flag){
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            else {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3];
                fscanf(file, "%u %u %u\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
            }
        }
    }
    //verticies = temp_vertices;
    cout << "Finished reading " << filePath << "\n";
    fclose(file);
    
    if (flag) {
        for (int i = 0; i < vertexIndices.size(); i++)
        {
            cyPoint2f uv1 = temp_uvs[uvIndices[i] - 1];
            cyPoint3f nm1 = temp_normals[normalIndices[i] - 1];
            cyPoint3f v1 = temp_vertices[vertexIndices[i++] - 1];
            
            cyPoint2f uv2 = temp_uvs[uvIndices[i] - 1];
            cyPoint3f nm2 = temp_normals[normalIndices[i] - 1];
            cyPoint3f v2 = temp_vertices[vertexIndices[i++] - 1];
            
            cyPoint2f uv3 = temp_uvs[uvIndices[i] - 1];
            cyPoint3f nm3 = temp_normals[normalIndices[i] - 1];
            cyPoint3f v3 = temp_vertices[vertexIndices[i] - 1];
            
            
            triangles.push_back(triangle(v1, v2, v3, uv1, uv2, uv3, nm1, nm2, nm3));
        }
    }
    else {
        for (int i = 0; i < vertexIndices.size(); i++)
        {
            cyPoint3f v1 = temp_vertices[vertexIndices[i++] - 1];
            cyPoint3f v2 = temp_vertices[vertexIndices[i++] - 1];
            cyPoint3f v3 = temp_vertices[vertexIndices[i] - 1];
            triangles.push_back(triangle(v1, v2, v3));
        }
        
    }
    triangles0=triangles;
}

bool polygon::inObject(cyPoint3f p){
    double F;
    for (int i=0; i<triangles.size(); i++){
        F=triangles[i].normal.Dot(p-triangles[i].a);
        if (F>0){
            return false;
        }
    }
    return true;
}

cyPoint3f polygon::normalVector(cyPoint3f ph){
    return triangles[intersection].normal;
};

double polygon::miniIntersection(cyPoint3f pe, cyPoint3f npe){
    double tmin=DBL_MAX;
    for (int i=0; i<triangles.size(); i++){
        double t=triangles[i].intersection(pe, npe);
        if(t<tmin){
            tmin=t;
            intersection=i;
        }
    }
    if (tmin<0.01){
        tmin=0;
    }
    return tmin;
}


cyColor polygon::getColor(vector<light> alllights, cyPoint3f ph, cyPoint3f pe, vector<object*> allobjects){
    if (t1.flag){
        float u, v;
        triangles[intersection].map(ph, u, v);
        c0=t0.GetColorAt(u, v);
        c1=t1.GetColorAt(u, v);
        csh=tsh.GetColorAt(u, v);
    }
    c=c0;
    getDiffuse(alllights, ph);
    getSpecular(alllights, ph, pe);
    getBoundary(alllights, ph, pe);
    getShadow(alllights, ph, allobjects);
    return c;
    
}

void polygon::getIORfromTexture(cyPoint3f ph){
    float u, v;
    triangles[intersection].map(ph, u, v);
    cyColor cior=tior.GetColorAt(u,v);
    ior=cior.r/255+cior.g/255+cior.b/255;
    ior=ior/6+1;


}

void polygon::translation(cyPoint3f moveVector){
    for (int i=0; i<triangles.size(); i++) {
        triangles[i]=triangles0[i];
        triangles[i].translation(moveVector);
    }
}
