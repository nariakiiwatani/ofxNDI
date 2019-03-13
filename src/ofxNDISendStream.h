#pragma once

#include "DoubleBuffer.h"
#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include "ofxNDISender.h"
#include "ofxNDIFrame.h"
#include "ofxNDIRecvStream.h"
#include "ofxNDI.h"

namespace ofxNDI {
namespace Send {

template<typename FrameType, typename Type=ofxNDISender>
class Stream
{
public:
	using Frame = FrameType;
	void setup(Type &type) { setup(type.getInstance()); }
	void setup(typename Type::Instance instance) { instance_ = instance; }
	virtual bool isAsync() const { return false; }

	template<typename Src>
	void send(Src &&src, const std::string &metadata="", int64_t timecode=NDIlib_send_timecode_synthesize) {
		static_assert(!std::is_same<Frame, MetadataFrame>::value, "this function is not for ofxNDISendMetadata");
		auto &frame = frame_.back();
		frame.encode(std::move(src), isAsync());
		frame.setMetadata(metadata);
		frame.timecode = timecode;
		additionalUpdate(frame);
		sendFrame(frame);
		frame_.swap();
	}
	virtual void sendFrame(const Frame &frame) const{}
protected:
	typename Type::Instance instance_;
	DoubleBuffer<Frame> frame_;
	virtual void additionalUpdate(Frame &frame){}
};

class VideoStream : public Stream<VideoFrame>
{
public:
	bool isAsync() const { return is_async_; }
	void setAsync(bool async) { is_async_ = async; }
	void setFrameRate(int frame_rate_n, int frame_rate_d) { frame_rate_n_=frame_rate_n; frame_rate_d_=frame_rate_d; }
protected:
	bool is_async_=false;
	int frame_rate_n_=30000, frame_rate_d_=1001;
	void additionalUpdate(Frame &frame) {
		frame.frame_rate_N = frame_rate_n_;
		frame.frame_rate_D = frame_rate_d_;
	}
	void sendFrame(const Frame &frame) const;
};

class MetadataStream : public Stream<ofxNDI::MetadataFrame>
{
public:
	void send(std::string &&src, int64_t timecode=NDIlib_send_timecode_synthesize) {
		auto &frame = frame_.back();
		frame.encode(std::move(src), isAsync());
		frame.timecode = timecode;
		additionalUpdate(frame);
		sendFrame(frame);
		frame_.swap();
	}
protected:
	void sendFrame(const Frame &frame) const;
};

}}

using ofxNDISendVideo = ofxNDI::Send::VideoStream;
using ofxNDISendAudio = ofxNDI::Send::Stream<ofxNDI::AudioFrame>;
using ofxNDISendMetadata = ofxNDI::Send::MetadataStream;
using ofxNDISenderRecvMetadata = ofxNDI::Recv::Blocking<ofxNDI::MetadataFrame, ofxNDISender>;
 
