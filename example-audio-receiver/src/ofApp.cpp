#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(receiver_.setup()) {
		audio_.setup(receiver_, 1000, true);
		stream_.setup(this, 2, 1, 44100, 256, 2);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		audio_.update();
		if(audio_.isFrameNew()) {
			ofSoundBuffer buffer;
			audio_.decodeTo(buffer);
			buffer_.append(buffer);
		}
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer){
	int length = min(buffer_.getNumFrames(), buffer.getNumFrames());
	buffer_.copyTo(buffer, length, buffer.getNumChannels(), 0, false);
	
	auto &data = buffer_.getBuffer();
	data.erase(begin(data), begin(data) + (buffer.getNumFrames()*buffer_.getNumChannels()));
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
