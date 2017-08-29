#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	sender_.setup("ofxNDISender example macos");
	sender_.setFourCCType(NDIlib_FourCC_type_RGBX);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int gray = ofGetFrameNum()%256;
	ofPushStyle();
	ofSetColor(gray);
	ofDrawRectangle(ofGetCurrentViewport());
	ofSetColor(255-gray);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 30);
	ofPopStyle();
	ofPixels pixels;
	ofGetGLRenderer()->saveFullViewport(pixels);
	sender_.sendVideoFrame(pixels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
