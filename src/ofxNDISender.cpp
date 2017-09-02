#include "ofxNDISender.h"

#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <chrono>

using namespace std;

bool ofxNDISender::setup(const string &name, const string &group, bool clock_video, bool clock_audio)
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
void ofxNDISender::addConnectionMetadata(const string &metadata, int64_t timecode)
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

ofxNDISender::~Sender()
{
	NDIlib_send_destroy(sender_);
	NDIlib_destroy();
}
