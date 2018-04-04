#pragma once

#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include <string>

namespace ofxNDI {
namespace Send {
class Sender
{
public:
	using Instance = NDIlib_send_instance_t;
	~Sender();
	
	bool setup(const std::string &name, const std::string &group="", bool clock_video=false, bool clock_audio=false);
	void addConnectionMetadata(const std::string &metadata, int64_t timecode=NDIlib_send_timecode_synthesize);
	void clearConnectionMetadata();
	
	bool isSetup() const { return instance_ != nullptr; }
	NDIlib_send_instance_t getInstance() const { return instance_; }
private:
	NDIlib_send_instance_t instance_=nullptr;
};
}}

using ofxNDISender = ofxNDI::Send::Sender;
