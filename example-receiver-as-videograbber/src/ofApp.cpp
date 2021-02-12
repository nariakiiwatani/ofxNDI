#include "ofApp.h"

using namespace ofxNDI::Recv;

//--------------------------------------------------------------
void ofApp::setup(){
	grabber_.setUseTexture(true);
	grabber_.setup();

	finder_.watchSources();
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	grabber_.draw(0,0);
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
void ofApp::keyPressed(int key){
	if(key >= '1' && key <= '9') {
		int index = key - '1';
		auto sources = finder_.getSources();
		if(sources.size() > index) {
			grabber_.setup(sources[index]);
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
