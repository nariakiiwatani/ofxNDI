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
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	video_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPixels pixels;
	video_.decodeTo(pixels);
	ofImage(pixels).draw(0,0);
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
	
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
	
}
