#include "ofxNDIGenlock.h"

ofxNDIGenlock::~Genlock()
{
	destroy();
}
void ofxNDIGenlock::destroy()
{
	if(instance_) {
		NDIlib_genlock_destroy(instance_);
		instance_ = nullptr;
	}
}

bool ofxNDIGenlock::setup(std::size_t index)
{
	destroy();
	auto &&sources = ofxNDI::listSources();
	if(index < sources.size()) {
		return setup(sources[index]);
	}
	instance_ = NDIlib_genlock_create(nullptr);
	return isSetup();
}
bool ofxNDIGenlock::setup(const ofxNDI::Source &source)
{
	destroy();
	auto src = toV1(source);
	instance_ = NDIlib_genlock_create(&src);
	return isSetup();
}

bool ofxNDIGenlock::isSourceActive() const
{
	return NDIlib_genlock_is_active(instance_);
}

void ofxNDIGenlock::changeConnection(const ofxNDI::Source &source)
{
	auto src = toV1(source);
	NDIlib_genlock_connect(instance_, &src);
}
void ofxNDIGenlock::disconnect()
{
	NDIlib_genlock_connect(instance_, nullptr);
}

bool ofxNDIGenlock::waitVideo() const
{
	return NDIlib_genlock_wait_video(instance_, &video_format_);
}
bool ofxNDIGenlock::waitAudio() const
{
	return NDIlib_genlock_wait_audio(instance_, &audio_format_);
}
