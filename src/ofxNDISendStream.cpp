#include "ofxNDISendStream.h"

#pragma mark video

void ofxNDISendVideo::reallocFrame(NDIlib_video_frame_v2_t &frame)
{
	size_t data_size = frame.p_data!=nullptr?frame.line_stride_in_bytes*frame.yres:0;
	if(data_size > 0) {
		auto p_data = new uint8_t[data_size];
		memcpy(p_data, frame.p_data, sizeof(uint8_t)*data_size);
		frame.p_data = p_data;
	}
	size_t metadata_size = frame.p_metadata!=nullptr?strlen(frame.p_metadata)+1:0;
	if(metadata_size > 0) {
		auto p_metadata = new char[strlen(frame.p_metadata)+1];
		strcpy(p_metadata, frame.p_metadata);
		frame.p_metadata = p_metadata;
	}
}

void ofxNDISendVideo::freeFrame(NDIlib_video_frame_v2_t &frame)
{
	if(frame.p_data != nullptr) {
		delete[] frame.p_data;
		frame.p_data = nullptr;
	}
	if(frame.p_metadata != nullptr) {
		delete[] frame.p_metadata;
		frame.p_metadata = nullptr;
	}
}

void ofxNDISendVideo::sendFrame(const NDIlib_video_frame_v2_t &_frame)
{
	if(is_async_) {
		auto &frame = frame_.back();
		frame = _frame;
		reallocFrame(frame);
		NDIlib_send_send_video_async_v2(instance_, &frame);
		frame_.swap();
		freeFrame(frame_.back());
	}
	else {
		NDIlib_send_send_video_v2(instance_, &_frame);
	}
}

template<>
void ofxNDISendAudio::sendFrame(const NDIlib_audio_frame_v2_t &frame)
{
	NDIlib_send_send_audio_v2(instance_, &frame);
}

void ofxNDISendMetadata::sendFrame(const NDIlib_metadata_frame_t &frame)
{
	NDIlib_send_send_metadata(instance_, &frame);
}

template<>
void ofxNDISendMetadataRecv::freeFrame()
{
	NDIlib_send_free_metadata(instance_, &frame_.back());
}
template<>
bool ofxNDISendMetadataRecv::captureFrame() {
	return NDIlib_send_capture(instance_, &frame_.back(), timeout_ms_) == NDIlib_frame_type_metadata;
}
