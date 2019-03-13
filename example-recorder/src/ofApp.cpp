#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	if(receiver_.setup()) {
		video_.setup<ofxNDI::Recv::FrameSyncVideo>(receiver_);
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
