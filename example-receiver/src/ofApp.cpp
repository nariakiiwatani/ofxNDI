#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	NDIlib_initialize();
	ofBackground(0);
	ofSetFrameRate(60);
	if(receiver_.setup()) {
		video_.setup<ofxNDI::Recv::FrameSyncVideo>(receiver_);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		video_.update();
		if(video_.isFrameNew()) {
			video_.decodeTo(pixels_);
		}
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(pixels_.isAllocated()) {
		ofImage(pixels_).draw(0,0);
	}
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
