#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(sender_.setup("ofxNDI audio sender example")) {
		audio_.setup(sender_);
	}
	else {
		ofLogError("NDI setup failed.");
	}
	if(!sound_.setup(this, 0, 2, 48000, 256, 4)) {
		ofLogError("sound stream setup error");
	}
}

void ofApp::audioIn(ofSoundBuffer &buffer)
{
	audio_.send(buffer);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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
