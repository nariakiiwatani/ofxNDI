#include "ofxNDIRecvStream.h"


template<>
bool ofxNDIRecvVideoBlocking::captureFrame(ofxNDI::VideoFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, &frame, nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}
template<>
void ofxNDIRecvVideoBlocking::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_recv_free_video_v2(instance_, &frame);
}
template<>
bool ofxNDIRecvVideoThreading::captureFrame(ofxNDI::VideoFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, &frame, nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}
template<>
void ofxNDIRecvVideoThreading::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_recv_free_video_v2(instance_, &frame);
}
bool ofxNDIRecvVideoFrameSync::captureFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_framesync_capture_video(sync_, &frame, field_type_);
	return frame.p_data != nullptr;
}
void ofxNDIRecvVideoFrameSync::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_framesync_free_video(sync_, &frame);
}

#pragma mark Audio Stream
template<>
bool ofxNDIRecvAudioBlocking::captureFrame(ofxNDI::AudioFrame &frame) {
	return NDIlib_recv_capture_v3(instance_, nullptr, &frame, nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}
template<>
void ofxNDIRecvAudioBlocking::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_recv_free_audio_v3(instance_, &frame);
}
template<>
bool ofxNDIRecvAudioThreading::captureFrame(ofxNDI::AudioFrame &frame) {
	return NDIlib_recv_capture_v3(instance_, nullptr, &frame, nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}
template<>
void ofxNDIRecvAudioThreading::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_recv_free_audio_v3(instance_, &frame);
}
bool ofxNDIRecvAudioFrameSync::captureFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_framesync_capture_audio_v2(sync_, &frame, sample_rate_, num_channels_, num_samples_);
	return frame.no_channels != 0;
}
void ofxNDIRecvAudioFrameSync::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_framesync_free_audio_v2(sync_, &frame);
}
int ofxNDIRecvAudioFrameSync::getNumQueuedSamples() const {
	return NDIlib_framesync_audio_queue_depth(sync_);
}


#pragma mark Metadata Stream

template<>
bool ofxNDIRecvMetadata::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, nullptr, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}
template<>
void ofxNDIRecvMetadata::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_recv_free_metadata(instance_, &frame);
}

