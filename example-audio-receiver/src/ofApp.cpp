#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(receiver_.setup()) {
		audio_.setup(receiver_, 1000, true);
		stream_.setup(this, 1, 0, 48000, 512, 2);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		do {
			// trying to receive all buffers from remote source
			audio_.update();
			if(audio_.isFrameNew()) {
				ofSoundBuffer buffer;
				audio_.decodeTo(buffer);
				buffer_.setSampleRate(buffer.getSampleRate());
				buffer_.setNumChannels(buffer.getNumChannels());
				buffer_.append(buffer);
			}
			else {
				break;
			}
		} while(true);
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer){
	static bool valid = false;
	// length required by system
	int length = buffer.getNumFrames();
	// once receiving buffer length is enough, start audio out
	if(!valid) {
		valid = buffer_.getDurationMS() >= initial_delay_*1000;
	}
	// when out of buffer, stop audio
	if(valid && buffer_.getNumFrames() < length) {
		valid = false;
	}
	if(valid) {
		buffer_.copyTo(buffer, length, buffer.getNumChannels(), 0, false);
		auto &data = buffer_.getBuffer();
		data.erase(begin(data), begin(data) + (length*buffer_.getNumChannels()));
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
