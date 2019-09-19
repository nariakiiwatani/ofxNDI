#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	auto findSource = [](const std::string &name_or_url) {
		auto sources = ofxNDI::listSources();
		if(name_or_url == "") {
			return make_pair(ofxNDI::Source(), false);
		}
		auto found = find_if(begin(sources), end(sources), [name_or_url](const ofxNDI::Source &s) {
			return ofIsStringInString(s.p_ndi_name, name_or_url) || ofIsStringInString(s.p_url_address, name_or_url);
		});
		if(found == end(sources)) {
			ofLogWarning("ofxNDI") << "no NDI source found by string:" << name_or_url;
			return make_pair(ofxNDI::Source(), false);
		}
		return make_pair(*found, true);
	};
	std::string name_or_url = "";	// Specify name or address of expected NDI source. In case of blank or not found, receiver will grab default(which is found first) source.
	auto result = findSource(name_or_url);
	if(result.second ? receiver_.setup(result.first) : receiver_.setup()) {
		int sample_rate = 48000;
		int num_channels = 2;
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
