#include "ofxNDIReceiver.h"
#include "ofUtils.h"

using namespace std;
using namespace ofxNDI;

bool ofxNDIReceiver::setup(size_t index, const Settings &settings)
{
	auto &&sources = ofxNDI::listSources();
	if(index < sources.size()) {
		return setup(sources[index], settings);
	}
	ofLogWarning("no NDI Source found");
	return false;
}
bool ofxNDIReceiver::setup(const ofxNDI::Source &source, const Settings &settings)
{
	NDIlib_recv_create_v3_t creator = { source, settings.color_format, settings.bandwidth, settings.deinterlace, nullptr };
	creator.p_ndi_recv_name = settings.name==""?nullptr:settings.name.c_str();
	instance_ = NDIlib_recv_create_v3(&creator);

	if (!instance_) {
		ofLogError("NDI Receiver failed to initialize");
		return false;
	}
	return true;
}
void ofxNDIReceiver::changeConnection(const Source &source)
{
	NDIlib_source_t src = source;
	NDIlib_recv_connect(instance_, &src);
}
void ofxNDIReceiver::disconnect()
{
	NDIlib_recv_connect(instance_, nullptr);
}

NDIlib_framesync_instance_t ofxNDIReceiver::createFrameSync()
{
	frame_sync_ = NDIlib_framesync_create(instance_);
	return frame_sync_;
}

void ofxNDIReceiver::addConnectionMetadata(const string &metadata, int64_t timecode) const
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_recv_add_connection_metadata(instance_, &data);
}
void ofxNDIReceiver::clearConnectionMetadata() const
{
	NDIlib_recv_clear_connection_metadata(instance_);
}

bool ofxNDIReceiver::isConnected() const
{
	return isSetup() && NDIlib_recv_get_no_connections(instance_);
}

bool ofxNDIReceiver::setTally(bool on_program, bool on_preview) const
{
	const NDIlib_tally_t tally = {
		on_program, on_preview
	};
	
	return NDIlib_recv_set_tally(instance_, &tally);
}

void ofxNDIReceiver::getQueue(int *video, int *audio, int *metadata) const
{
	NDIlib_recv_queue_t frames;
	NDIlib_recv_get_queue(instance_, &frames);
	if(video) { *video = frames.video_frames; }
	if(audio) { *audio = frames.audio_frames; }
	if(metadata) { *metadata = frames.metadata_frames; }
}

std::string ofxNDIReceiver::getWebControl() const
{
	const char *ptr = NDIlib_recv_get_web_control(instance_);
	std::string ret = ptr;
	NDIlib_recv_free_string(instance_, ptr);
	return ret;
}

void ofxNDIReceiver::getNumReceivedFrame(int64_t *video, int64_t *audio, int64_t *metadata) const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, &frames, nullptr);
	if(video) { *video = frames.video_frames; }
	if(audio) { *audio = frames.audio_frames; }
	if(metadata) { *metadata = frames.metadata_frames; }
}
void ofxNDIReceiver::getNumDroppedFrame(int64_t *video, int64_t *audio, int64_t *metadata) const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, nullptr, &frames);
	if(video) { *video = frames.video_frames; }
	if(audio) { *audio = frames.audio_frames; }
	if(metadata) { *metadata = frames.metadata_frames; }
}

ofxNDIReceiver::~Receiver()
{
	if(frame_sync_) NDIlib_framesync_destroy(frame_sync_);
	if(instance_) NDIlib_recv_destroy(instance_);
	NDIlib_destroy();
}
