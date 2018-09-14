//
//  Ripple.cpp
//  SkeletonReceiver
//
//  Created by 黃耀昌 on 2018/9/13.
//

#include "Ripple.hpp"

// define static here in cpp file
int Ripple::life;
int Ripple::radius;
float Ripple::colorStep;
//ofColor Ripple::color;

Ripple::Ripple(ofVec2f center_, string type_){
    center = center_;
    type = type_;
    timeStamp = 0;
//    color = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
    
    int len = 70;
    p1 = ofVec2f(0,-1).getNormalized()*len;
    p2 = ofVec2f(1,1).getNormalized()*len;
    p3 = ofVec2f(-1,1).getNormalized()*len;
    
    c_3[0].setHex(0xf9621f);
    c_3[1].setHex(0xf31717);
    c_3[2].setHex(0xf58db6);

    c_2[0].setHex(0x367ABD);
    c_2[1].setHex(0xF3A530);
    
    cout << "show color: ";
    cout << c_2[0] << "   /" <<  c_2[1] << endl;
    cout << c_3[0] << c_3[1] << c_3[2] << endl;
    
    color = getColor(ofSignedNoise(center_*0.001), type);
}

void Ripple::drawRipple(){
    ofPushMatrix();
    ofSetColor(color=color.getLerped(ofColor(0,0,0,0), colorStep));
    ofNoFill();
    ofSetLineWidth(4);
    float scale = ofMap(timeStamp, 0, life, 0, radius);
    if(type == "right")
        ofDrawCircle(center, scale);
    else if(type == "left"){
//        ofDrawTriangle(center+p1*scale, center+p2*scale, center+p3*scale);
//        ofDrawTriangle(center+p1, center+p2, center+p3);
        ofDrawCircle(center, scale);
    }
    ofPopMatrix();
}

void Ripple::checkLife(){
    timeStamp++;
}

bool Ripple::isEnd(){
    return timeStamp > life;
}

void Ripple::setCircleSpeedByEmotion(int emotions){
    switch(emotions){
        case 0:
            life = 100;
            radius = 1000;
            colorStep = 0.01;
            break;
        case 1:life = 100;
            radius = 1000;
            colorStep = 0.002;
            break;
        case 2:
            life = 50;
            radius = 1000;
            colorStep = 0.01;
            break;
        case 3:
            life = 300;
            radius = 700;
            colorStep = 0.002;
            break;
        case 4:
            life = 100;
            radius = 1000;
            colorStep = 0.008 ;
            break;
        case 5:
            life = 300;
            radius = 700;
            colorStep = 0.002;
            break;
        default:;
            
    }
}

ofColor Ripple::getColor(float seed, string type){
    
    if(type == "right"){
        if(seed < 0){
            return c_3[0].getLerped(c_3[1], seed+1);
        }
        else
            return c_3[1].getLerped(c_3[2], seed);
    }
    else if(type == "left"){
        
        return c_2[0].getLerped(c_2[1], seed/2+0.5);
    }
    else return ofColor::black;
}

