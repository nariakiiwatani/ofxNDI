#pragma once

#include "DoubleBuffer.h"
#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include <Processing.NDI.Advanced.h>
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
	void setup(Type &type) { setup(type.getInstance()); }
	void setup(typename Type::Instance instance) { instance_ = instance; }

	template<typename SrcType>
	void send(const SrcType &src, int64_t timecode=NDIlib_send_timecode_synthesize) { send(src, "", timecode); }
	template<typename SrcType>
	void send(const SrcType &src, const std::string &metadata, int64_t timecode=NDIlib_send_timecode_synthesize) {
		auto frame = createFrame(src);
		frame.p_metadata = metadata.c_str();
		frame.timecode = timecode;
		beforeSend(frame);
		sendFrame(frame);
		data_buffer_.swap();
	}
protected:
	typename Type::Instance instance_;
	virtual void sendFrame(const FrameType &frame) const;
	virtual void beforeSend(FrameType &frame){}

	template<typename SrcType>
	FrameType createFrame(const SrcType &src) {
		auto &buffer = data_buffer_.back();
		buffer.resize(Frame::calcDataSize(src));
		return Frame::encode<SrcType, FrameType>(src, buffer.data(), buffer.size());
	}

	DoubleBuffer<std::vector<uint8_t>> data_buffer_;
};

template<> template<typename SrcType>
void Stream<AudioFrameInterleaved>::send(const SrcType &src, const std::string &metadata, int64_t timecode) {
	auto frame = createFrame(src);
	frame.timecode = timecode;
	beforeSend(frame);
	sendFrame(frame);
	data_buffer_.swap();
}

class VideoStream : public Stream<ofxNDI::VideoFrame>
{
public:
	void setFrameRate(int frame_rate_n, int frame_rate_d) { frame_rate_n_=frame_rate_n; frame_rate_d_=frame_rate_d; }
protected:
	int frame_rate_n_=30000, frame_rate_d_=1001;

	void beforeSend(ofxNDI::VideoFrame &frame) override {
		frame.frame_rate_N = frame_rate_n_;
		frame.frame_rate_D = frame_rate_d_;
	}
};

class VideoStreamAsync : public VideoStream
{
protected:
	void sendFrame(const ofxNDI::VideoFrame &frame) const override;
};
}}

using ofxNDISendVideo = ofxNDI::Send::VideoStream;
using ofxNDISendVideoAsync = ofxNDI::Send::VideoStreamAsync;
using ofxNDISendAudio = ofxNDI::Send::Stream<ofxNDI::AudioFrame>;
using ofxNDISendMetadata = ofxNDI::Send::Stream<ofxNDI::MetadataFrame>;
using ofxNDISenderRecvMetadata = ofxNDI::Recv::Blocking<ofxNDI::MetadataFrame, ofxNDISender>;
 
