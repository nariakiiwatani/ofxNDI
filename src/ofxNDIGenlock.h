#pragma once

#include <stddef.h>
#include <Processing.NDI.Advanced.h>
#include <Processing.NDI.Genlock.h>
#include "ofxNDIFinder.h"
#include "ofxNDISender.h"
#include "ofxNDIFrame.h"

namespace ofxNDI {

class Genlock
{
public:
	using Instance = NDIlib_genlock_instance_t;
	~Genlock();
	void destroy();
	
	bool setup(std::size_t index=0);
	bool setup(const Source &source);
	bool isSetup() const { return instance_ != nullptr; }

	bool isSourceActive() const;
	
	bool isConnected() const;
	void changeConnection(const Source &source);
	void disconnect();

	void setVideoFormat(int frame_rate_n, int frame_rate_d, NDIlib_frame_format_type_e frame_format_type) {
		video_format_.frame_rate_N = frame_rate_n;
		video_format_.frame_rate_D = frame_rate_d;
		video_format_.frame_format_type = frame_format_type;
	}
	bool waitVideo() const;
	void setAudioFormat(int no_samples, int sample_rate) {
		audio_format_.no_samples = no_samples;
		audio_format_.sample_rate = sample_rate;
	}
	bool waitAudio() const;
	
private:
	NDIlib_genlock_instance_t instance_=nullptr;
	ofxNDI::VideoFrame video_format_;
	ofxNDI::AudioFrame audio_format_;
};

}

using ofxNDIGenlock = ofxNDI::Genlock;
