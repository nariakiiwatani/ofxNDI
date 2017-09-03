#pragma once

#include "DoubleBuffer.h"
#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include "ofxNDISender.h"
#include "ofxNDI.h"

namespace ofxNDI {
namespace Send {

template<typename FrameType>
class Stream
{
public:
	using Frame = FrameType;
	void setup(Sender &sender) { setup(sender.getSender()); }
	void setup(NDIlib_send_instance_t sender) { sender_ = sender; }
	template<typename Src> void send(const Src &src) { sendFrame(ofxNDI::encode(src)); }
	virtual void sendFrame(const Frame &frame){}
protected:
	NDIlib_send_instance_t sender_;
};

class VideoStream : public Stream<NDIlib_video_frame_v2_t>
{
public:
	void setAsync(bool async) { is_async_ = async; }
protected:
	bool is_async_=false;
	DoubleBuffer<Frame> frame_;
	void sendFrame(const Frame &frame);
	void reallocFrame(Frame &frame);
	void freeFrame(Frame &frame);
};

}}

using ofxNDIVideoSendStream = ofxNDI::Send::VideoStream;
using ofxNDIAudioSendStream = ofxNDI::Send::Stream<NDIlib_audio_frame_v2_t>;
