#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(sender_.setup("ofxNDI audio sender example")) {
		audio_.setup(sender_);
	}
	else {
		ofLogError("NDI setup failed.");
	}
	if(!sound_.setup(this, 0, 1, 48000, 512, 1)) {
		ofLogError("sound stream setup error");
	}
}

void ofApp::audioIn(ofSoundBuffer &buffer)
{
	buffer_.setSampleRate(buffer.getSampleRate());
	buffer_.setNumChannels(buffer.getNumChannels());
	buffer_.append(buffer);
}

void ofApp::clearBuffer()
{
	buffer_.clear();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(buffer_.getDurationMS() >= minimum_buffer_duration_*1000) {
		audio_.send(buffer_);
		buffer_.clear();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString("press any key to clear sound buffer", 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	clearBuffer();
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
