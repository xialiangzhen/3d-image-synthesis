//
//  texture.h
//  demo
//
//  Created by 夏 夏 on 16/3/7.
//  Copyright © 2016年 Liangzhen Xia. All rights reserved.
//

#ifndef texture_h
#define texture_h
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "cyPoint.h"
#include "cyColor.h"
#include "EasyBMP.h"

using namespace std;


class texture
{
public:
    int imageWidth = 0;
    int imageHeight = 0;
    bool flag=false;
    cyColor * pixmap;
    
    
    texture(){};
    
    void setPixmap(string filePath)
    {
        BMP Input;
        char fileName[100];
        for (int a = 0; a <= filePath.size(); a++)
        {
            fileName[a] = filePath[a];
        }
        if (!Input.ReadFromFile(fileName))
        {
            cout << "Load file fail!"<<endl;
            return;
        }
        imageWidth = Input.TellWidth();
        imageHeight = Input.TellHeight();
//        cout<<imageHeight<<endl<<imageWidth<<endl;
        
        pixmap = new cyColor[imageWidth * imageHeight];
        for (int x = 0; x < imageWidth; x++)
        {
            for (int y = 0; y < imageHeight; y++)
            {
                int currentPixel = y * imageWidth + x;
                float r = (int)Input(x, y)->Red;
                float g = (int)Input(x, y)->Green;
                float b = (int)Input(x, y)->Blue;
                pixmap[currentPixel] = cyColor(r, g, b);
            }
        }
        cout << "Load file success!"<<endl;
        flag=true;
        
    }
    
    
    // float x, y is assumed to be (0,1)
    cyColor GetColorAt(float x, float y)
    {
        
        double X = x*imageWidth;
        double Y = y*imageHeight;
        while (X < 0){
            X = X+imageWidth;
        }
        while (Y < 0){
            Y = Y+imageHeight;
        }
        while (X>imageWidth) {
            X=X-imageWidth;
        }
        while (Y>imageHeight) {
            Y=Y-imageHeight;
        }
        int currentPixel = int(Y) * imageWidth + int (X);
        return pixmap[currentPixel];
    }
    
        
};




#endif /* texture_h */
