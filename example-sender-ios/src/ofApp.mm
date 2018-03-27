#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(sender_.setup("ofxNDI sender example ios")) {
		video_.setup(sender_);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	int gray = ofGetFrameNum()%256;
	ofPixels pixels;
	pixels.allocate(320, 240, 3);
	pixels.set(gray);
	video_.send(pixels);
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
