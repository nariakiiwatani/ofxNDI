#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	camera_.setup(750, 1334);
	if(sender_.setup("ofxNDISender example ios")) {
		video_.setup(sender_);
		video_.setAsync(true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	camera_.update();
	if(camera_.isFrameNew()) {
		video_.send(camera_.getPixels());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(camera_.isInitialized()) {
		camera_.draw(ofGetCurrentViewport());
	}
	else {
		int gray = ofGetFrameNum()%256;
		ofPixels pixels;
		pixels.allocate(320, 240, 3);
		pixels.set(gray);
		video_.send(pixels);
		ofImage(pixels).draw(0,0);
	}
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
