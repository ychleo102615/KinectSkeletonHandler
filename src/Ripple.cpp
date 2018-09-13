//
//  Ripple.cpp
//  SkeletonReceiver
//
//  Created by 黃耀昌 on 2018/9/13.
//

#include "Ripple.hpp"

Ripple::Ripple(ofVec2f center_){
    center = center_;
    timeStamp = 0;
    color = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
}

void Ripple::drawRipple(){
    ofSetColor(color=color.getLerped(ofColor::black, 0.2));
    ofDrawCircle(center, ofMap(timeStamp, 0, 1000, 0, radius));
}

void Ripple::checkLife(){
    timeStamp++;
}

bool Ripple::isEnd(){
    return timeStamp > life;
}
