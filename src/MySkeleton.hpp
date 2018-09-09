//
//  MySkeleton.hpp
//  SkeletonReceiver
//
//  Created by 黃耀昌 on 2018/07/21.
//

#ifndef MySkeleton_hpp
#define MySkeleton_hpp

#include "ofMain.h"

#endif /* MySkeleton_hpp */

class MySkeleton {
    
public:
    ofVec2f node[25];
    enum body {
        SpineBase       = 0,
        SpineMid        = 1,
        Neck            = 2,
        Head            = 3,
        ShoulderLeft    = 4,
        ElbowLeft       = 5,
        WristLeft       = 6,
        HandLeft        = 7,
        ShoulderRight   = 8,
        ElbowRight      = 9,
        WristRight      = 10,
        HandRight       = 11,
        HipLeft         = 12,
        KneeLeft        = 13,
        AnkleLeft       = 14,
        FootLeft        = 15,
        HipRight        = 16,
        KneeRight       = 17,
        AnkleRight      = 18,
        FootRight       = 19,
        SpineShoulder   = 20,
        HandTipLeft     = 21,
        ThumbLeft       = 22,
        HandTipRight    = 23,
        ThumbRight      = 24};
    bool isDetected;
    ofColor color;
    
    MySkeleton();
    void loadSkeletonFrom(MySkeleton ms);
    void setColor(ofColor color_);
    void drawBody();
    void drawBone(int jointType1, int jointType2);
    
    float getFrameDistance(MySkeleton lastFrameMS);
    float getFrameDistance(MySkeleton lastFrameMS, string type);
    
    float getBodyDiscreteExtent();
//    float getBodyDiscreteExtent(string type);
    float getBodyStretchExtent();
    float getBodyStretchExtent(string type);
    float getBodyStretchExtent(int ndnum, ofVec2f body[], int centroidNodeNumber);
    
    float handDistance();
    
    bool isUsableNumber(float value);
    
    float getHandLength(string side);
    
    float twoHandsPositioningMethod(string direction, string side);
};
