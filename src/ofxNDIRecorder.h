#pragma once

#include "ofxNDIReceiver.h"

namespace ofxNDI {
namespace Recv {

class Recorder
{
public:
	bool setup(Receiver &receiver);
	bool setup(Receiver::Instance instance);
	
	bool start(const std::string &filename="") const;
	bool stop() const;
	bool isRecording() const;
	
	bool setAudioLevel(float dB) const;
	
	std::string getFilename() const;
	
	bool isError(std::string *info) const;
	
	bool getTimes(int64_t *frames, int64_t *start, int64_t *last) const;
private:
	Receiver::Instance instance_;
};
}}

using ofxNDIRecorder = ofxNDI::Recv::Recorder;
