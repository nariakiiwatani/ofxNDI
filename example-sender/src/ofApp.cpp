#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	
	// get proper grabber device ID 
	vector<ofVideoDevice> devices = camera_.listDevices();
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			cout << devices[i].id << ":" << devices[i].deviceName << endl;
		}
	}
	camera_.setDeviceID(4);		// set proper device ID
	camera_.setup(1920, 1080);

	if(sender_.setup("ofxNDISender example")) {
		video_.setup(sender_);
		int frame_rate_n, frame_rate_d;
		NDIlib_frame_format_type_e frame_format_type;
		genlock_.setup();
		video_.getVideoFormat(frame_rate_n, frame_rate_d, frame_format_type);
		genlock_.setVideoFormat(frame_rate_n, frame_rate_d, frame_format_type);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	camera_.update();
	if(camera_.isFrameNew()) {
		
		ofPixels pix = camera_.getPixels();
		pix.setImageType(OF_IMAGE_COLOR_ALPHA);
		//if(genlock_.waitVideo()) {
		//	video_.send(pix);
		//}
		video_.send(pix);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(camera_.isInitialized()) {
		ofPixels pix = camera_.getPixels();
		ofImage(pix).draw(ofGetCurrentViewport());
		//ofPixels pixels;
		//ofGetGLRenderer()->saveFullViewport(pixels);
		//video_.send(pixels);
	}
	//else {
	//	int gray = ofGetFrameNum()%256;
	//	ofPushStyle();
	//	ofSetColor(gray);
	//	ofDrawRectangle(ofGetCurrentViewport());
	//	ofSetColor(255-gray,0,0);
	//	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 30);
	//	ofPopStyle();
	//	ofPixels pixels;
	//	ofGetGLRenderer()->saveFullViewport(pixels);
	//	video_.send(pixels);
	//}
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
