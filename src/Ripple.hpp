//
//  Ripple.hpp
//  SkeletonReceiver
//
//  Created by 黃耀昌 on 2018/9/13.
//

#ifndef Ripple_hpp
#define Ripple_hpp

#include "ofMain.h"

#endif /* Ripple_hpp */

class Ripple{
public:
    ofVec2f center;
    static int life;// = 300;
    static int radius;// = 700;
    static float colorStep;
    int timeStamp;
//    static ofColor color;
    ofColor color;
    string type;
    ofVec2f p1, p2, p3;
    ofColor c_3[3], c_2[2];
    
    Ripple(ofVec2f center_, string type_);
    void drawRipple();
    void checkLife();
    bool isEnd();
    
    void static setCircleSpeedByEmotion(int emotions);
    ofColor getColor(float seed, string type);
};
