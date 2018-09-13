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
    const int life = 1000;
    const int radius = 300;
    int timeStamp;
    ofColor color;
    
    Ripple(ofVec2f center_);
    void drawRipple();
    void checkLife();
    bool isEnd();
};
