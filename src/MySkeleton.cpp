//
//  MySkeleton.cpp
//  SkeletonReceiver
//
//  Created by 黃耀昌 on 2018/07/21.
//

#include "MySkeleton.hpp"

MySkeleton::MySkeleton(){
    isDetected = false;
}

void MySkeleton::loadSkeletonFrom(MySkeleton ms){
    for(int i=0;i<25;i++){
        node[i] = ms.node[i];
    }
    isDetected = ms.isDetected;
}

void MySkeleton::setColor(ofColor color_){
    color = color_;
}

void MySkeleton::drawBody(){
    drawBone(Head, Neck);
    drawBone(Neck, SpineShoulder);
    drawBone(SpineShoulder, SpineMid);
    drawBone(SpineMid, SpineBase);
    drawBone(SpineShoulder, ShoulderRight);
    drawBone(SpineShoulder, ShoulderLeft);
    drawBone(SpineBase, HipRight);
    drawBone(SpineBase, HipLeft);
    
    // Right Arm
    drawBone(ShoulderRight, ElbowRight);
    drawBone(ElbowRight, WristRight);
    drawBone(WristRight, HandRight);
    drawBone(HandRight, HandTipRight);
    drawBone(WristRight, ThumbRight);
    
    // Left Arm
    drawBone(ShoulderLeft, ElbowLeft);
    drawBone(ElbowLeft, WristLeft);
    drawBone(WristLeft, HandLeft);
    drawBone(HandLeft, HandTipLeft);
    drawBone(WristLeft, ThumbLeft);
    
    // Right Leg
    drawBone(HipRight, KneeRight);
    drawBone(KneeRight, AnkleRight);
    drawBone(AnkleRight, FootRight);
    
    // Left Leg
    drawBone(HipLeft, KneeLeft);
    drawBone(KneeLeft, AnkleLeft);
    drawBone(AnkleLeft, FootLeft);
}

void MySkeleton::drawBone(int jointType1, int jointType2) {
    ofPushMatrix();
    ofSetColor(color);
    ofTranslate(node[jointType1].x, node[jointType1].y);
    ofDrawCircle(0, 0, 25);
    ofPopMatrix();
    ofDrawLine(node[jointType1].x, node[jointType1].y, node[jointType2].x, node[jointType2].y);
}

float MySkeleton::getFrameDistance(MySkeleton lastFrameMS) {
    
    float distanceSum = 0;
    for(int i=0;i<25;i++) {
        float d = node[i].distance(lastFrameMS.node[i]);
        if(!isnan(d) && !isinf(d)){
            distanceSum += d;
        }
    }
    
    return distanceSum;
}

float MySkeleton::getFrameDistance(MySkeleton lastFrameMS, string type){
    if(type == "hand"){
        
        float distanceSum = 0;
        float d;
        d = node[HandTipRight].distance(lastFrameMS.node[HandTipRight]);
        if(!isnan(d) && !isinf(d))
            distanceSum += d;
        d = node[HandTipLeft].distance(lastFrameMS.node[HandTipLeft]);
        if(!isnan(d) && !isinf(d))
            distanceSum += d;
        
        return distanceSum;
    }
    
    return 0;
}



float MySkeleton::getBodyStretchExtent() {
    int ndnum = 4;
    ofVec2f body[ndnum];
    body[0] = node[HandTipLeft];
    body[1] = node[HandTipRight];
    body[2] = node[FootLeft];
    body[3] = node[FootRight];
    
    ofVec2f centroid = node[SpineMid];
    //return getBodyStretchExtent(ndnum, body, SpineMid);
    
    float a[ndnum];
    float aSum = 0;
    int aValid = ndnum;
    cout << "stretch: ";
    for(int i=0;i<ndnum;i++) {
        ofVec2f limbVector = body[i] - centroid;
        a[i] = limbVector.angle(ofVec2f(0,1));
        a[i] = abs(a[i]);
        float lookAt = a[i];
        if(!isnan(a[i])){
            aSum += a[i];
            cout << a[i] << " " << i << " ";
        }
        else
            aValid--;
        
        
    }
    cout << endl;
    return aSum;
}

float MySkeleton::getBodyStretchExtent(string type) {
    
    int ndnum;
    if(type == "all"){
        ndnum = 4;
        ofVec2f body[ndnum];
        body[0] = node[HandTipLeft];
        body[1] = node[HandTipRight];
        body[2] = node[FootLeft];
        body[3] = node[FootRight];
        
//        ofVec2f centroid = node[SpineMid];
        return getBodyStretchExtent(ndnum, body, SpineMid);
    }
    else if(type == "upper"){
        ndnum = 2;
        ofVec2f body[ndnum];
        body[0] = node[HandTipLeft];
        body[1] = node[HandTipRight];
        
//        ofVec2f centroid = node[SpineShoulder];
        return getBodyStretchExtent(ndnum, body, SpineShoulder);
    }
    else if(type == "lower"){
        ndnum = 2;
        ofVec2f body[ndnum];
        body[0] = node[FootLeft];
        body[1] = node[FootRight];
        
//        ofVec2f centroid = node[SpineBase];
        return getBodyStretchExtent(ndnum, body, SpineBase);
    }
    else{
        cout << "error" << endl;
        return -1;
    }
    
}

float MySkeleton::getBodyStretchExtent(int ndnum, ofVec2f body[], int centroidNodeNumber){
    
    ofVec2f centroid = node[centroidNodeNumber];
    float a[ndnum];
    float aSum = 0;
    int aValid = ndnum;
    for(int i=0;i<ndnum;i++) {
        ofVec2f limbVector = body[i] - centroid;
        a[i] = limbVector.angle(ofVec2f(0,1));
        a[i] = abs(a[i]);
        float lookAt = a[i];
        if(!isnan(a[i])){
            aSum += a[i];
        }
        else
            aValid--;
    }
    return aSum;
}


float MySkeleton::getBodyDiscreteExtent() {
    
    int ndnum = 4;
    ofVec2f body[ndnum];
    body[0] = node[HandTipLeft];
    body[1] = node[HandTipRight];
    body[2] = node[FootLeft];
    body[3] = node[FootRight];
    
    ofVec2f centroid;
    int validNodeNum = ndnum;
    for(int i=0;i<ndnum;i++) {
        if(!isinf(body[i].x)){
            ofVec2f lookAt = body[i];
            centroid += body[i];
        }
        else
            validNodeNum--;
    }
    centroid /= validNodeNum;
//    cout << "centroid is: " << centroid << endl;
    
    float d[ndnum];
    float dSum = 0;
    int dValid = ndnum;
    cout << "discrete: ";
    for(int i=0;i<ndnum;i++) {
        d[i] = centroid.squareDistance(body[i]);
        float lookAt = d[i];
        if(!isinf(d[i]) && !isnan(d[i])){
            dSum += d[i];
            cout << d[i] << " "  << i << " ";
        }
        else {
            dValid--;
        }
    }
    cout << endl;

    float standardDeviation;
    standardDeviation = sqrt(dSum);
    
    return standardDeviation;
}

float MySkeleton::handDistance(){
    float d;
    d = node[HandTipRight].distance(node[HandTipLeft]);
//    if(!isnan(d) && !isinf(d))
//        return d;
//
//    return -1;
    
//    return makeSureValidNumber(d);
    return isUsableNumber(d)?d:-1;
}

bool MySkeleton::isUsableNumber(float value){
    if(!isnan(value) && !isinf(value))
        return true;
    else
        return false;
}

float MySkeleton::getHandLength(string side){
    float distance = 0;
    float d;
    int handJoint[4];
    
    if(side == "right"){
        handJoint[0] = ShoulderRight;
        handJoint[1] = ElbowRight;
        handJoint[2] = WristRight;
        handJoint[3] = HandRight;
    }
    else if(side == "left"){
        handJoint[0] = ShoulderLeft;
        handJoint[1] = ElbowLeft;
        handJoint[2] = WristLeft;
        handJoint[3] = HandLeft;
    }
    else
        return -1;
    
    for(int i=0;i<3;i++){
        d = node[handJoint[i]].distance(node[handJoint[i+1]]);
        if(isUsableNumber(d))
            distance += d;
    }
    
    return distance;
}


float MySkeleton::twoHandsPositioningMethod(string direction, string side){
    
    float lowerBound, upperBound;
    float handRelatedPosition;
    float result;
    ofVec2f vec;
    int hand, shoulder;
    
    if(direction == "vertical" && side == "left"){
        lowerBound = 100;
        upperBound = 0;
        hand = HandLeft;
        shoulder = ShoulderLeft;
    }
    else if(direction == "horizontal" && side == "left"){
        lowerBound = 0;
        upperBound = 13;
        hand = HandLeft;
        shoulder = ShoulderLeft;
    }
    else if(direction == "vertical" && side == "right"){
        lowerBound = 8;
        upperBound = 0;
        hand = HandRight;
        shoulder = ShoulderRight;
    }
    else if(direction == "horizontal" && side == "right"){
        lowerBound = 4;
        upperBound = 0;
        hand = HandRight;
        shoulder = ShoulderRight;
    }
    else
        return -1;
    
    vec = (node[hand] - node[shoulder]) / getHandLength(side);
    
    if(direction == "vertical")
        handRelatedPosition = vec.y;
    else if(direction == "horizontal")
        handRelatedPosition = vec.x;
    
    result = ofMap(handRelatedPosition, -1, 1, lowerBound, upperBound, true);
    
    return result;
    
}









