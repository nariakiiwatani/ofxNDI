#include "ofxNDISendStream.h"


void ofxNDIVideoSendStream::reallocFrame(NDIlib_video_frame_v2_t &frame)
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
void ofxNDIVideoSendStream::freeFrame(NDIlib_video_frame_v2_t &frame)
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

void ofxNDIVideoSendStream::sendFrame(NDIlib_video_frame_v2_t &frame, bool async)
{
	if(async) {
		NDIlib_send_send_video_async_v2(sender_, nullptr);
		NDIlib_send_send_video_async_v2(sender_, &frame);
	}
	else {
		NDIlib_send_send_video_v2(sender_, &frame);
	}
}
