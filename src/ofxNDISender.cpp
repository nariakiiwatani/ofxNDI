#include "ofxNDISender.h"
#include "ofLog.h"
#include <Processing.NDI.Advanced.h>

using namespace std;

ofxNDI::Source ofxNDISender::getSourceName() const
{
	return toV2(*NDIlib_send_get_source_name(instance_));
}

bool ofxNDISender::setup(const string &name, const string &group, bool clock_video, bool clock_audio)
{
	NDIlib_send_create_t create_settings = {
		name.c_str(),
		group.c_str(),
		clock_video,
		clock_audio };
	instance_ = NDIlib_send_create_v2(&create_settings);
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

void ofxNDISender::addConnectionMetadata(const string &metadata, int64_t timecode) const
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_send_add_connection_metadata(instance_, &data);
}
void ofxNDISender::clearConnectionMetadata() const
{
	NDIlib_send_clear_connection_metadata(instance_);
}

void ofxNDISender::setFailover(const ofxNDI::Source &source) const
{
	auto src = toV1(source);
	NDIlib_send_set_failover(instance_, &src);
}
bool ofxNDISender::getTally(bool *on_program, bool *on_preview, int64_t timeout_ms) const
{
	NDIlib_tally_t tally;
	if(NDIlib_send_get_tally(instance_, &tally, timeout_ms)) {
		if(on_program) *on_program = tally.on_program;
		if(on_preview) *on_preview = tally.on_preview;
		return true;
	}
	return false;
}
bool ofxNDISender::setTally(bool on_program, bool on_preview) const
{
	NDIlib_tally_t tally{on_program, on_preview};
	return NDIlib_send_set_tally(instance_, &tally);
}

ofxNDISender::~Sender()
{
	NDIlib_send_destroy(instance_);
	NDIlib_destroy();
}
