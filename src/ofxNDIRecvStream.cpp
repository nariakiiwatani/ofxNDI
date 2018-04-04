#include "ofxNDIRecvStream.h"

#pragma mark Video Stream

template<> void ofxNDIVideoRecvStream::freeFrame() {
	NDIlib_recv_free_video_v2(instance_, &frame_.back());
}
template<> bool ofxNDIVideoRecvStream::captureFrame() {
	return NDIlib_recv_capture_v2(instance_, &frame_.back(), nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}

#pragma mark Audio Stream

template<> void ofxNDIAudioRecvStream::freeFrame() {
	NDIlib_recv_free_audio_v2(instance_, &frame_.back());
}
template<> bool ofxNDIAudioRecvStream::captureFrame() {
	return NDIlib_recv_capture_v2(instance_, nullptr, &frame_.back(), nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}

#pragma mark Metadata Stream

template<> void ofxNDIMetadataRecvStream::freeFrame() {
	NDIlib_recv_free_metadata(instance_, &frame_.back());
}
template<> bool ofxNDIMetadataRecvStream::captureFrame() {
	return NDIlib_recv_capture_v2(instance_, nullptr, nullptr, &frame_.back(), timeout_ms_) == NDIlib_frame_type_metadata;
}
