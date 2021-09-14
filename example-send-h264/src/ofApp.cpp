#include "ofApp.h"
#include "ofxNDIFrameCompressed.h"

struct video_frame
{	const uint8_t* p_data;
	const uint8_t* p_extra;
	int64_t  dts;
	int64_t  pts;
	uint32_t data_size;
	uint32_t extra_size;
	bool     is_keyframe;
};
namespace highQ {
#include "h264_lowQ.h"
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	if(sender_.setup("ofxNDISender send H.264 example")) {
		video_.setup(sender_);
		video_.setFrameRate(highQ::video_frame_rate_n, highQ::video_frame_rate_d);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofxNDI::CompressedVideoInfo info;
	info.FourCC = NDIlib_compressed_FourCC_type_H264;
	info.xres = highQ::video_xres;
	info.yres = highQ::video_yres;
	info.video_FourCC = NDIlib_FourCC_video_type_ex_H264_highest_bandwidth;

	int index = ofGetFrameNum() % highQ::num_video_frames;
	auto &data = highQ::video_frames[index];
	info.data = data.p_data;
	info.data_size = data.data_size;
	info.is_keyframe = data.is_keyframe;
	info.pts = data.pts;
	info.dts = data.dts;
	info.extra_data = data.p_extra;
	info.extra_data_size = data.extra_size;
	video_.send(info);
}

//--------------------------------------------------------------
void ofApp::draw(){

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
