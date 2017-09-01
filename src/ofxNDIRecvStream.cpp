#include "ofxNDIRecvStream.h"

#pragma mark Video Stream

template<> void ofxNDIVideoRecvStream::freeFrame() {
	NDIlib_recv_free_video_v2(receiver_, &frame_.back());
}
template<> bool ofxNDIVideoRecvStream::captureFrame() {
	return NDIlib_recv_capture_v2(receiver_, &frame_.back(), nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}

#pragma mark Audio Stream

template<> void ofxNDIAudioRecvStream::freeFrame() {
	NDIlib_recv_free_audio_v2(receiver_, &frame_.back());
}
template<> bool ofxNDIAudioRecvStream::captureFrame() {
	return NDIlib_recv_capture_v2(receiver_, nullptr, &frame_.back(), nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}
