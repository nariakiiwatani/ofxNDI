#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	finder_.watchSources();
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
	using namespace std;
	auto sources = finder_.getSources();
	auto names = accumulate(begin(sources), end(sources), vector<string>(), [](vector<string> result, const ofxNDI::Source &src) {
		result.push_back(ofToString(result.size()+1, 2, '0')+". "+src.p_ndi_name+"("+src.p_url_address+")");
		return result;
	});
	ofDrawBitmapString("press 1-9 to select available source", 10, 20);
	ofDrawBitmapString(ofJoinString(names, "\n"), 10, 30);
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
	if(key >= '1' && key <= '9') {
		int index = key - '1';
		auto sources = finder_.getSources();
		if(sources.size() > index) {
			if(receiver_.isSetup() ? (receiver_.changeConnection(sources[index]), true) : receiver_.setup(sources[index])) {
				video_.setup(receiver_);
				recorder_.setup(receiver_);
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
