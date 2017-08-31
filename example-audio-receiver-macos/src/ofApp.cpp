#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	receiver_.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		receiver_.update();
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(receiver_.isAudioFrameNew()) {
		ofSoundBuffer buffer;
		receiver_.getAudioFrame(buffer);
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
