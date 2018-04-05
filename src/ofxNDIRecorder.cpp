#include "ofxNDIRecorder.h"


bool ofxNDIRecorder::setup(Receiver &receiver)
{
	return setup(receiver.getInstance());
}
bool ofxNDIRecorder::setup(Receiver::Instance instance)
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
	std::string ret = str;
	NDIlib_recv_free_string(instance_, str);
	return ret;
}

bool ofxNDIRecorder::isError(std::string &info) const
{
	const char *str = NDIlib_recv_recording_get_filename(instance_);
	info = str;
	if(str == nullptr) {
		return false;
	}
	NDIlib_recv_free_string(instance_, str);
	return true;
}

bool ofxNDIRecorder::getTimes(int64_t &frames, int64_t &start, int64_t &last) const
{
	NDIlib_recv_recording_time_t times;
	bool ret = NDIlib_recv_recording_get_times(instance_, &times);
	frames = times.no_frames;
	start = times.start_time;
	last = times.last_time;
	return ret;
}

