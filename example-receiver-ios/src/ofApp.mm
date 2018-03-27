#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	if(receiver_.setup()) {
		video_.setup(receiver_, 1000, true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	video_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPixels pixels;
	video_.decodeTo(pixels);
	ofImage(pixels).draw(0,0);
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
	
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
	
}
