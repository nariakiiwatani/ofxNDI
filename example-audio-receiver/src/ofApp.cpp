#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	int sample_rate = 48000;
	int num_channels = 2;
	int num_buffers = 1;
	int num_samples = 512;

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

	finder_.watchSources();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace std;
	auto sources = finder_.getSources();
	auto names = accumulate(begin(sources), end(sources), vector<string>(), [](vector<string> result, const ofxNDI::Source &src) {
		result.push_back(ofToString(result.size()+1, 2, '0')+". "+src.p_ndi_name+"("+src.p_url_address+")");
		return result;
	});
	ofDrawBitmapString("press 1-9 to select available source", 10, 10);
	ofDrawBitmapString(ofJoinString(names, "\n"), 10, 20);
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
	if(key >= '1' && key <= '9') {
		int index = key - '1';
		auto sources = finder_.getSources();
		if(sources.size() > index) {
			if(receiver_.isSetup() ? (receiver_.changeConnection(sources[index]), true) : receiver_.setup(sources[index])) {
				audio_.setup(receiver_);
			}
		}
	}
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
