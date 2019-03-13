#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	if(receiver_.setup()) {
		int sample_rate = 44100;
		int num_channels = 1;
		int num_buffers = 1;
		int num_samples = 512;
		audio_.setup(receiver_);
		audio_.setSampleRate(sample_rate);
		audio_.setNumChannels(num_channels);
		audio_.setNumSamples(num_samples);
		ofSoundStreamSettings sss;
		sss.numInputChannels = 0;
		sss.numOutputChannels = num_channels;
		sss.sampleRate = sample_rate;
		sss.numBuffers = num_buffers;
		sss.bufferSize = num_samples;
		sss.setOutListener(this);
		stream_.setup(sss);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer){
	if(receiver_.isConnected()) {
		audio_.update();
		if(audio_.isFrameNew()) {
			audio_.decodeTo(buffer);
		}
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
