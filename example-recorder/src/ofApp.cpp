#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	auto findSource = [](const string &name_or_url) {
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
	string name_or_url = "";	// Specify name or address of expected NDI source. In case of blank or not found, receiver will grab default(which is found first) source.
	auto result = findSource(name_or_url);
	if(result.second ? receiver_.setup(result.first) : receiver_.setup()) {
		video_.setup(receiver_);
		recorder_.setup(receiver_);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if(receiver_.isConnected()) {
		video_.update();
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPixels pixels;
	video_.decodeTo(pixels);
	ofImage(pixels).draw(0,0);
	if(recorder_.isRecording()) {
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofDrawRectangle(50,50,100,100);
		ofPopStyle();
	}
	std::string err;
	if(recorder_.isError(&err)) {
		ofDrawBitmapString(err, 10,10);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case ' ':
			if(recorder_.isRecording()) {
				recorder_.stop();
			}
			else {
				recorder_.start("test");
			}
			break;
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
