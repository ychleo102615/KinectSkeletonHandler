#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxPubSubOsc.h"
#include "ofxGui.h"
#include "MySkeleton.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    float getHandMovement();
    float getMovementAmount(deque<float> frameDistances);
    string getBinaryList(int target, int length);
    string getAngryBeatsList(int density);
    void checkRecording();
    void showSkeleton();
    void stockDistances();
    float convertMovementAmount(int ma, string type);
    int phaseHandler(int recordCounter);
    int densityShiftedByPhase(int density);
    
    ofxOscReceiver receiverPC;
    ofxOscSender sender;
    ofVec2f skeleton[26];
    
    int choice;
    int choiceLength;
    ofxOscReceiver receiverPureData;
    
    int scaleNumber;
    
    ofxPanel gui;
    //ofParameter<int> bufferSize;
    ofxIntSlider bufferSize;
    
    //float timeSignature;
    float movementAmountConverted;
    float discreteNumber;
    float minDis;
    float maxDis;
    float stretchNumber;
    float movementAmount;
    float handMovementAmount;
    
    // recording setup
    bool showingSkeleton;
    bool recording;
    bool replaying;
    bool showingDatas;
    int isRecording;
    int isReplaying;
    
    int skeletonFrameCounter;
    int replayFrameCounter;
    const int oneMinutesCount = 2400;
    const int twoMinutesCount = 4800;
    vector<MySkeleton> trail;
    
    int hands_volume;
    int hands_range;
    int hands_density;
    int hands_pause;
    
};

