#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(receiver_.setup()) {
		audio_.setup(receiver_, 1000, true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		audio_.update();
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(audio_.isFrameNew()) {
		ofSoundBuffer buffer;
		audio_.decodeTo(buffer);
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
