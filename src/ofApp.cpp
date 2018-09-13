#include "ofApp.h"

//--------------------------------------------------------------


MySkeleton ms1;
MySkeleton lastMs;
vector<MySkeleton> skeletonBuffer;
deque<float> frameDistances;
#define BUFFER_MAX 10



int oddTable[] = {1, 1, 2, 4, 5, 6, 7, 8};
int evenTable[] = {0, 1, 2, 4, 5, 6, 7, 8};




void ofApp::setup(){
    receiverPC.setup(5000);
    receiverPureData.setup(5100);
    sender.setup("127.0.0.1", 5101);
    ofSetFrameRate(40);
    
    ms1.setColor(ofColor::white);
    lastMs.setColor(ofColor::red);
    ofBackground(0);
    
    gui.setup("Title");
    gui.setPosition(30, 400);
    gui.add(bufferSize.setup("buffer size", 10, 5, 25));
    
    minDis = 10000;
    maxDis = 0;
    scaleNumber = 13;
    
    movementAmount = 0;
    handMovementAmount = 0;
    
    showingSkeleton = false;
    recording = false;
    replaying = false;
    showingDatas = false;
    isRecording = 0;
    isReplaying = 0;
    skeletonFrameCounter = 0;
    replayFrameCounter = 0;
    hands_volume = 0;
    hands_range = 0;
    hands_density = 0;
    hands_pause = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // get skeleton from pc
    while(receiverPC.hasWaitingMessages()){
        ofxOscMessage m;
        receiverPC.getNextMessage(&m);
        if(m.getAddress() == "/skltn"){
            int oscArgumentNum = m.getNumArgs();
            
            if(oscArgumentNum > 0)
                ms1.isDetected = true;
            else
                ms1.isDetected = false;
            
            cout << "skeleton number: "<< oscArgumentNum << endl;
            for (int j=0;j<oscArgumentNum/50;j++){ 
                if(j==0) {
                    for(int i=0;i<oscArgumentNum/2;i++){
                        ms1.node[i].x = m.getArgAsFloat(2*i);
                        ms1.node[i].y = m.getArgAsFloat(2*i+1);
                    }
                }
            }
        }
    }
    
    checkRecording();
    
    // stock distance each frame
    stockDistances();

//    movementAmount = ofLerp(movementAmount, getMovementAmount(frameDistances), 0.5) ;
//    handMovementAmount = ofLerp(handMovementAmount, getHandMovement(), 0.5);
    
    // stock previous skeleton
    skeletonBuffer.push_back(ms1);
    if(skeletonBuffer.size() > bufferSize){
        lastMs.loadSkeletonFrom(skeletonBuffer.front());
        while(skeletonBuffer.size() > bufferSize)
            skeletonBuffer.erase(skeletonBuffer.begin());
    }
    
    // get data from puredata
    while(receiverPureData.hasWaitingMessages()){
        ofxOscMessage m;
        receiverPureData.getNextMessage(&m);
        if(m.getAddress() == "/timeCode"){
            choice = m.getArgAsFloat(0);
            choiceLength = m.getArgAsFloat(1);
        }
        
        if(m.getAddress() == "/melodyBeat"){
//            string msg = m.getArgAsString(0);
            int msg = m.getArgAsInt32(0);
            cout << "This sholud be bang: " << msg << endl << "--------------------------------------------------------------\n";
        }
    }
    
    // send data to puredata
    ofxOscMessage m;
    m.setAddress("/signature");
    m.addStringArg(getBinaryList(choice, choiceLength));
    sender.sendMessage(m);
    
    // send body message to puredata
    ofxOscMessage m2;
    m2.setAddress("/discrete");
    
//    discreteNumber = ms1.getBodyDiscreteExtent();
//    stretchNumber = ms1.getBodyStretchExtent();
    
    if(discreteNumber < minDis && discreteNumber != 0)
        minDis = discreteNumber;
    if(discreteNumber > maxDis)
        maxDis = discreteNumber;
    
    float discreteLevel=0;// = ofMap(discreteNumber, 100, 1000, 0, scaleNumber, true);
    cout << "Discrete level: " << discreteLevel << endl;
    if(discreteLevel<0)
        discreteLevel = 0;
    else if(discreteLevel > scaleNumber)
        discreteLevel = scaleNumber;
    m2.addFloatArg(discreteLevel);
    sender.sendMessage(m2);
    
    // send movement to puredata
    ofxOscMessage m3;
    m3.setAddress("/movement");
//    movementAmountConverted = convertMovementAmount(movementAmount);
    movementAmountConverted = convertMovementAmount(handMovementAmount, "hand");
    m3.addFloatArg(movementAmountConverted);
    sender.sendMessage(m3);
    
    
    // Send hands positioning method
    ofxOscMessage mHandsPos;
    mHandsPos.setAddress("/hands_pos");
    hands_volume = ms1.twoHandsPositioningMethod("vertical", "left");
    hands_range = ms1.twoHandsPositioningMethod("horizontal", "left");
    hands_density = ms1.twoHandsPositioningMethod("vertical", "right");
//    hands_density = densityShiftedByPhase(hands_density);
    hands_pause = ms1.twoHandsPositioningMethod("horizontal", "right");
    
    mHandsPos.addIntArg(hands_volume);
    mHandsPos.addIntArg(hands_range);
    mHandsPos.addIntArg(hands_density);
    mHandsPos.addIntArg(hands_pause);
//    mHandsPos.addFloatArg(hands_density);
    sender.sendMessage(mHandsPos);
    
    
    // near every 2 seconds get one string of ANGER type
    if(ofGetFrameNum() % 80 == 0) {
        ofxOscMessage m4;
        m4.setAddress("/angerlist");
//        m4.addStringArg(getAngryBeatsList((int)movementAmountConverted));
        hands_density = densityShiftedByPhase(hands_density);
        m4.addStringArg(getAngryBeatsList(hands_density));
        sender.sendMessage(m4);
    }
    
    // Send hand joint
    ofxOscMessage m5;
    m5.setAddress("/hand_joint");
    m5.addFloatArg(ms1.handDistance());
    sender.sendMessage(m5);
    
    // Send start recording or stop recording message to puredata
    ofxOscMessage mRecording;
    mRecording.setAddress("/recording");
    isRecording = (recording)?1:0;
    mRecording.addIntArg(isRecording);
    mRecording.addIntArg(phaseHandler(skeletonFrameCounter));
    mRecording.addFloatArg(ofMap(ms1.node[MySkeleton::HandTipLeft].y, 0, 1080, 0, 4));
    isReplaying = (replaying)?1:0;
    mRecording.addIntArg(isReplaying);
    sender.sendMessage(mRecording);
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(showingSkeleton)
        showSkeleton();
    ofSetColor(255);
    
    if(showingDatas){
        ofScale(2);
        ofDrawBitmapString(choice, 10, 10);
        ofDrawBitmapString(choiceLength, 10, 30);
        ofDrawBitmapString(getBinaryList(choice, choiceLength), 10, 50);
        if(ms1.isDetected){
            ofDrawBitmapString("Body discrete: " + to_string(discreteNumber) ,10,70);
            ofDrawBitmapString("Body range: " + to_string(minDis) + " / " + to_string(maxDis) ,10,80);
            ofDrawBitmapString("Body stretch: " + to_string(stretchNumber) ,10,90);
        }
        ofDrawBitmapString("Movement amount: " + to_string(movementAmount) ,10,110);
        ofDrawBitmapString("Hand movement: " + to_string(handMovementAmount) ,10,130);
        
        //string msg = ms1.isHandJoint()?"true":"false";
        ofDrawBitmapString("Hand distance: " + to_string(ms1.handDistance()), 10, 150);
        
        ofDrawBitmapString("record stamp: " + to_string(skeletonFrameCounter) +
                           " / seconds: " + to_string(skeletonFrameCounter/40) +
                           " / is recording:" + to_string(isRecording), 10, 200);
        ofDrawBitmapString("replay stamp: " + to_string(replayFrameCounter) +
                           " / seconds: " + to_string(replayFrameCounter/40), 10, 220);
        
        gui.draw();
        
        ofDrawBitmapString(/*ms1.twoHandsPositioningMethod("vertical", "left")*/hands_volume, 160, 10);
        ofDrawBitmapString(/*ms1.twoHandsPositioningMethod("horizontal", "left")*/hands_range, 160, 30);
        ofDrawBitmapString(/*ms1.twoHandsPositioningMethod("vertical", "right")*/hands_density, 160, 50);
        ofDrawBitmapString(/*densityShiftedByPhase(ms1.twoHandsPositioningMethod("horizontal", "right"))*/hands_pause, 160, 70);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r' && !replaying){
        recording = !recording;
    }
    if(key == 's'){
        showingSkeleton = !showingSkeleton;
    }
    if(key == 'p' && !recording){
        replaying = !replaying;
    }
    if(key == 'c'){
        skeletonFrameCounter = 0;
        replayFrameCounter = 0;
        trail.clear();
    }
    if(key == 'd'){
        showingDatas = !showingDatas;
    }
    if(key == 'h'){
        replayFrameCounter = 0;
    }
}

//--------------------------------------------------------------
void ofApp::checkRecording(){
    if(recording){
        skeletonFrameCounter++;
        trail.push_back(ms1);
    }
}

//--------------------------------------------------------------
void ofApp::showSkeleton(){
    if(!replaying){
        
        if(recording){
            ms1.setColor(ofColor::pink);
            ofColor shadowColor = ofColor::black;
            for(int i=bufferSize;i>0;i--){
//            for(int i=1;i<=bufferSize;i++){
                if(skeletonFrameCounter > i){
                    lastMs.loadSkeletonFrom(trail.at(skeletonFrameCounter - i));
                    lastMs.setColor(shadowColor = shadowColor.getLerped(ofColor::pink, 0.3));
                    if(lastMs.isDetected){
                        lastMs.drawBody();
                    }
                }
                
            }
        }
        else
            ms1.setColor(ofColor::white);
        
        if(ms1.isDetected)
            ms1.drawBody();
        
    }
    else{
        if(replayFrameCounter < skeletonFrameCounter){
            int i = replayFrameCounter;
            ms1 = trail.at(i);
            if(ms1.isDetected)
                ms1.drawBody();
            replayFrameCounter++;
            if(replayFrameCounter == skeletonFrameCounter)
                replaying = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::stockDistances(){
    // stock distance each frame
    if(skeletonBuffer.size() > 0){
        float distanceSum = ms1.getFrameDistance(skeletonBuffer.back());
        frameDistances.push_back(distanceSum);
    }
    if(frameDistances.size() > bufferSize){
        while(frameDistances.size() > bufferSize)
            frameDistances.pop_front();
    }
}

//--------------------------------------------------------------
float ofApp::convertMovementAmount(int ma, string type){
    float movementAmountConverted;
    float movementLowbound;
    float movementUpbound;
    
    if(type == "hand"){
        movementLowbound = 0;
        movementUpbound = 2750;
    }
    else{
        movementLowbound = 300;
        movementUpbound = 8000;
    }
    
    
    
    if(skeletonFrameCounter < twoMinutesCount){
        movementAmountConverted = ofMap(movementAmount, movementLowbound, movementUpbound, 0, 4);
    }
    else{
        movementAmountConverted = ofMap(movementAmount, movementLowbound, movementUpbound, 3, 7);
    }
    
    //movementAmountConverted = ofMap(movementAmount, 300, 8000, 0, 7);
    if(movementAmountConverted < 0)
        movementAmountConverted = 0;
    else if(movementAmountConverted > 7)
        movementAmountConverted = 7;
    
    return movementAmountConverted;
}


//--------------------------------------------------------------

float ofApp::getHandMovement(){
    
    float distanceSum = 0;
    int buffersize = skeletonBuffer.size();
//    for(int i=0;i<skeletonBuffer.size()-1;i++){
    for(int i=0;i<buffersize-1;i++){
        float d =  skeletonBuffer[i].getFrameDistance(skeletonBuffer[i+1], "hand");
        distanceSum += d;
        
    }
    
    return distanceSum;
}

//--------------------------------------------------------------
float ofApp::getMovementAmount(deque<float> frameDistances){

    float sumOfFrame = 0;
    for(int i=0;i<frameDistances.size();i++){
        sumOfFrame += frameDistances.at(i);
    }

    return sumOfFrame;
}

//--------------------------------------------------------------
string ofApp::getBinaryList(int target, int length) {
    string list;
    for(int i=0;i<length;i++){
        list = to_string(target%2) + " " + list;
        target /= 2;
    }
    cout << list << endl;
    return list;
}

//--------------------------------------------------------------
string ofApp::getAngryBeatsList(int density) {
    
    int oddNum = oddTable[density];
    int evenNum = evenTable[density];
    
    /*
    vector<int> choseOdd;
    vector<int> choseEven;
    for(int i=0;i<8;i++){
        choseOdd.push_back(i);
        choseEven.push_back(i);
    }
     */
    
    string angryBeatsList;
    /*
    int angryBeatsScript[16];
    for(int i=0;i<16;i++){
        angryBeatsScript[i] = 0;
    }
    
    for(int i=0;i<oddNum;i++) {
        int remainsIndex = rand()*choseOdd.size()/RAND_MAX;
        int oddsIndex = choseOdd.at(remainsIndex);
        angryBeatsScript[oddsIndex*2+1] = 1;
        
        choseOdd.erase(choseOdd.begin() + remainsIndex);
    }
    
    for(int i=0;i<evenNum;i++) {
        int remainsIndex = rand()*choseEven.size()/RAND_MAX;
        int evensIndex = choseEven.at(remainsIndex);
        angryBeatsScript[evensIndex*2] = 1;
        
        choseEven.erase(choseEven.begin() + remainsIndex);
    }
    
    for(int i=0;i<16;i++){
        //cout << angryBeatsScript[i] << endl;
        angryBeatsList = angryBeatsList + to_string(angryBeatsScript[i]) + " ";
    }
    */
    
    for(int i=0;i<8;i++){
        int n;
        if(ofRandom(8)+1 <= oddNum)
            n = 1;
        else
            n = 0;
        angryBeatsList = angryBeatsList + to_string(n) + " ";
        if(ofRandom(8)+1 <= evenNum)
            n = 1;
        else
            n = 0;
        angryBeatsList = angryBeatsList + to_string(n) + " ";
    }
    
    return angryBeatsList;
    
}


int ofApp::phaseHandler(int recordCounter){
    int phase = -1;
    if(recordCounter < oneMinutesCount){
        phase = 0;
    }
    else if(recordCounter < twoMinutesCount){
        phase = 1;
    }
    else if(recordCounter < endCount){
        phase = 2;
    }
    else{
        phase = 3;
    }
    
    return phase;
}

int ofApp::densityShiftedByPhase(int density){
    int phase = phaseHandler(skeletonFrameCounter);
    int lowbound;
    int upbound;
    if(phase == 0){
        lowbound = 0;
        upbound = 4;
    }
    else if(phase == 1){
        lowbound = 2;
        upbound = 6;
    }
    else if(phase == 2){
        lowbound = 4;
        upbound = 8;
    }
    else{
        lowbound = 0;
        upbound = 8;
    }
    
    return ofMap(density, 0, 8, lowbound, upbound, true);
}


