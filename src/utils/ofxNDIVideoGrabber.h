#pragma once

#include "ofVideoGrabber.h"
#include "ofxNDIReceiver.h"

namespace ofxNDI {
namespace Recv {
class VideoGrabber : public ofVideoGrabber, public Receiver
{
public:
	VideoGrabber();
	bool setup(std::size_t index=0, const Receiver::Settings &settings=Settings());
	bool setup(const Source &source, const Receiver::Settings &settings=Settings());
};
}}

using ofxNDIVideoGrabber = ofxNDI::Recv::VideoGrabber;
