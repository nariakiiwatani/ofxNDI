#include "ofxNDISender.h"

#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <chrono>

ofxNDISender::ofxNDISender()
{
	video_frame_.frame_rate_N = 60000;
	video_frame_.frame_rate_D = 1000;
	video_frame_.FourCC = NDIlib_FourCC_type_RGBA;
	video_frame_.frame_format_type = NDIlib_frame_format_type_progressive;
	video_frame_.timecode = NDIlib_send_timecode_synthesize;
	video_frame_.p_data = nullptr;
}

bool ofxNDISender::setup(const std::string &name, const std::string &group, bool clock_video, bool clock_audio)
{
	NDIlib_send_create_t create_settings = {
		name.c_str(),
		group.c_str(),
		clock_video,
		clock_audio };
	sender_ = NDIlib_send_create(&create_settings);
	if(!sender_) {
		ofLogError("NDI Sender failed to initialize");
		return false;
	}
	return true;
}
void ofxNDISender::addConnectionMetadata(const std::string &metadata, int64_t timecode)
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_send_add_connection_metadata(sender_, &data);
}
void ofxNDISender::clearConnectionMetadata()
{
	NDIlib_send_clear_connection_metadata(sender_);
}

ofxNDISender::~ofxNDISender()
{
	// Destroy the NDI sender
	NDIlib_send_destroy(sender_);
	
	// Not required, but nice
	NDIlib_destroy();
}
