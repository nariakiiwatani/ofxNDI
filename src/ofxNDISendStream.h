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
	template<typename Src>
	void send(const Src &src, const std::string &metadata="", int64_t timecode=NDIlib_send_timecode_synthesize) {
		static_assert(!std::is_same<Frame, NDIlib_metadata_frame_t>::value, "this function is not for ofxNDISendMetadataStream");
		Frame frame = ofxNDI::encode(src);
		frame.p_metadata = metadata.c_str();
		frame.timecode = timecode;
		additionalUpdate(frame);
		sendFrame(frame);
	}
	virtual void sendFrame(const Frame &frame){}
protected:
	NDIlib_send_instance_t sender_;
	virtual void additionalUpdate(Frame &frame){}
};

class VideoStream : public Stream<NDIlib_video_frame_v2_t>
{
public:
	void setAsync(bool async) { is_async_ = async; }
	void setFrameRate(int frame_rate_n, int frame_rate_d) { frame_rate_n_=frame_rate_n; frame_rate_d_=frame_rate_d; }
protected:
	bool is_async_=false;
	int frame_rate_n_=30000, frame_rate_d_=1001;
	DoubleBuffer<Frame> frame_;
	void additionalUpdate(Frame &frame) {
		frame.frame_rate_N = frame_rate_n_;
		frame.frame_rate_D = frame_rate_d_;
	}
	void sendFrame(const Frame &frame);
	void reallocFrame(Frame &frame);
	void freeFrame(Frame &frame);
};

class MetadataStream : public Stream<NDIlib_metadata_frame_t>
{
public:
	void send(const std::string &metadata, int64_t timecode=NDIlib_send_timecode_synthesize) {
		Frame frame;
		frame.p_data = const_cast<char*>(metadata.c_str());
		frame.length = metadata.size()+1;
		frame.timecode = timecode;
		sendFrame(frame);
	}
protected:
	void sendFrame(const Frame &frame);
};
}}

using ofxNDIVideoSendStream = ofxNDI::Send::VideoStream;
using ofxNDIAudioSendStream = ofxNDI::Send::Stream<NDIlib_audio_frame_v2_t>;
using ofxNDIMetadataSendStream = ofxNDI::Send::MetadataStream;
