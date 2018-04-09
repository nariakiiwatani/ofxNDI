#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	router_.setup("example-router", "ofxNDI");
	sources_ = ofxNDI::listSources();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int label = 1;
	for(auto &s : sources_) {
		ofDrawBitmapString(ofToString(label) + ":" + s.p_ndi_name + "(" + s.p_url_address + ")", 10, label*20);
		++label;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'r':
			sources_ = ofxNDI::listSources();
			break;
		case '0':
			router_.clear();
			break;
	}
	if('1' <= key && key <= '9') {
		int index = key-'1';
		if(index < sources_.size()) {
			router_.setRoute(sources_[index]);
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
