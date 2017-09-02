#pragma once

#include "DoubleBuffer.h"
#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include "ofxNDISender.h"
#include "ofxNDI.h"

namespace ofxNDI {
namespace Send {

template<typename Frame>
class Stream
{
public:
	void setup(Sender &sender);
	void setup(NDIlib_send_instance_t sender);
	template<typename Src> void send(Src &src, bool async);
protected:
	NDIlib_send_instance_t sender_;
	DoubleBuffer<Frame> frame_;
	virtual void sendFrame(Frame &frame, bool async){}
	virtual void reallocFrame(Frame &frame){}
	virtual void freeFrame(Frame &frame){}
};
	
template<typename Frame>
void Stream<Frame>::setup(Sender &sender)
{
	setup(sender.getSender());
}
template<typename Frame>
void Stream<Frame>::setup(NDIlib_send_instance_t sender)
{
	sender_ = sender;
}
template<typename Frame> template<typename Src>
void Stream<Frame>::send(Src &src, bool async) {
	if(async) {
		ofxNDI::encode(src, frame_.back());
		reallocFrame(frame_.back());
		frame_.swap();
		sendFrame(frame_.front(), true);
		freeFrame(frame_.back());
	}
	else {
		Frame frame;
		ofxNDI::encode(src, frame);
		sendFrame(frame, false);
	}
}

class VideoStream : public Stream<NDIlib_video_frame_v2_t>
{
public:
private:
	void sendFrame(NDIlib_video_frame_v2_t &frame, bool async);
	void reallocFrame(NDIlib_video_frame_v2_t &frame);
	void freeFrame(NDIlib_video_frame_v2_t &frame);
};
}}

using ofxNDIVideoSendStream = ofxNDI::Send::VideoStream;
