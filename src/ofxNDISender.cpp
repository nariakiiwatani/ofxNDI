#include "ofxNDISender.h"
#include "ofLog.h"

using namespace std;

bool ofxNDISender::setup(const string &name, const string &group, bool clock_video, bool clock_audio)
{
	NDIlib_send_create_t create_settings = {
		name.c_str(),
		group.c_str(),
		clock_video,
		clock_audio };
	instance_ = NDIlib_send_create(&create_settings);
	if(!instance_) {
		ofLogError("NDI Sender failed to initialize");
		return false;
	}
	return true;
}

bool ofxNDISender::isConnected(int64_t timeout_ms) const
{
	return isSetup() && NDIlib_send_get_no_connections(instance_, timeout_ms);
}

void ofxNDISender::addConnectionMetadata(const string &metadata, int64_t timecode)
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_send_add_connection_metadata(instance_, &data);
}
void ofxNDISender::clearConnectionMetadata()
{
	NDIlib_send_clear_connection_metadata(instance_);
}

ofxNDISender::~Sender()
{
	NDIlib_send_destroy(instance_);
	NDIlib_destroy();
}
