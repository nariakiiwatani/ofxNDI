#include "ofxNDIRecorder.h"


bool ofxNDIRecorder::setup(ofxNDIReceiver &receiver)
{
	return setup(receiver.getInstance());
}
bool ofxNDIRecorder::setup(ofxNDIReceiver::Instance instance)
{
	instance_ = instance;
	return NDIlib_recv_recording_is_supported(instance_);
}

bool ofxNDIRecorder::start(const std::string &filename) const
{
	return NDIlib_recv_recording_start(instance_, filename.c_str());
}
bool ofxNDIRecorder::stop() const
{
	return NDIlib_recv_recording_stop(instance_);
}
bool ofxNDIRecorder::isRecording() const
{
	return NDIlib_recv_recording_is_recording(instance_);
}

bool ofxNDIRecorder::setAudioLevel(float dB) const
{
	return NDIlib_recv_recording_set_audio_level(instance_, dB);
}

std::string ofxNDIRecorder::getFilename() const
{
	const char *str = NDIlib_recv_recording_get_filename(instance_);
	if(str == nullptr) {
		return "";
	}
	std::string ret = str;
	NDIlib_recv_free_string(instance_, str);
	return ret;
}

bool ofxNDIRecorder::isError(std::string *info) const
{
	const char *str = NDIlib_recv_recording_get_filename(instance_);
	if(str == nullptr) {
		return false;
	}
	if(info) { *info = str; }
	NDIlib_recv_free_string(instance_, str);
	return true;
}

bool ofxNDIRecorder::getTimes(int64_t *frames, int64_t *start, int64_t *last) const
{
	NDIlib_recv_recording_time_t times;
	bool ret = NDIlib_recv_recording_get_times(instance_, &times);
	if(frames) { *frames = times.no_frames; }
	if(start) { *start = times.start_time; }
	if(last) { *last = times.last_time; }
	return ret;
}

