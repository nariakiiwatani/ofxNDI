#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	camera_.setup(1920, 1080);
	if(sender_.setup("ofxNDISender example")) {
		video_.setup(sender_);
		video_.setAsync(true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	camera_.update();
	if(camera_.isFrameNew()) {
		video_.send(camera_.getPixels());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(camera_.isInitialized()) {
		camera_.draw(ofGetCurrentViewport());
	}
	else {
		int gray = ofGetFrameNum()%256;
		ofPushStyle();
		ofSetColor(gray);
		ofDrawRectangle(ofGetCurrentViewport());
		ofSetColor(255-gray,0,0);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 30);
		ofPopStyle();
		ofPixels pixels;
		ofGetGLRenderer()->saveFullViewport(pixels);
		video_.send(pixels);
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
