#pragma once

#include <stddef.h>
#include <Processing.NDI.Lib.h>
#include "ofxNDIFrame.h"
#include "ofxNDIReceiver.h"
#include "ofxNDI.h"
#include "ofThread.h"
#include "DoubleBuffer.h"
#include "ofEvents.h"

namespace ofxNDI {
namespace Recv {

template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Stream
{
public:
	virtual void setup(Wrapper &wrapper) { instance_ = wrapper.getInstance(); }
	virtual void update(){}
	bool isFrameNew() const { return is_frame_new_; }
	virtual const Frame& getFrame() const=0;
	template<typename Output> void decodeTo(Output &dst) const { getFrame().decode(dst); }

	std::string getMetadata() const { 
		static_assert(!std::is_same<Frame, ofxNDI::MetadataFrame>::value, "this function is not for ofxNDIRecvMetadata");
		auto ptr = getFrame().p_metadata;
		return ptr ? ptr : "";
	}
	ofEvent<const Frame> onFrameReceived;
protected:
	typename Wrapper::Instance instance_;
	bool is_frame_new_=false;
};
template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Blocking : public Stream<Frame, Wrapper>
{
public:
	void setTimeout(uint32_t milliseconds) { timeout_ms_ = milliseconds; }
	void update() {
		Frame frame;
		if(captureFrame(frame)) {
			using ::std::swap;
			swap(frame, frame_);
			freeFrame(frame);
			is_frame_new_ = true;
			ofNotifyEvent(Stream<Frame, Wrapper>::onFrameReceived, getFrame(), this);
		}
		else {
			is_frame_new_ = false;
		}
	}
	const Frame& getFrame() const { return frame_; }
	bool isFrameNew() const { return is_frame_new_; }
private:
	virtual bool captureFrame(Frame &frame);
	virtual void freeFrame(Frame &frame);
	Frame frame_;
	bool is_frame_new_=false;
	uint32_t timeout_ms_=1000;
};
template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Threading : public Stream<Frame, Wrapper>, private ofThread
{
public:
	void setup(Wrapper &wrapper) {
		Stream<Frame,Wrapper>::setup(wrapper);
		startThread();
	}
	~Threading() {
		if(isThreadRunning()) {
			waitForThread();
		}
	}
	void setTimeout(uint32_t milliseconds) { timeout_ms_ = milliseconds; }
	void update() {
		std::lock_guard<std::mutex> lock(mutex_);
		is_frame_new_ = has_new_frame_;
		has_new_frame_ = false;
	}
	
	void threadedFunction() {
		while(isThreadRunning()) {
			updateFrame();
			sleep(1);
		}
	}
	void updateFrame() {
		if(captureFrame(frame_.back())) {
			std::lock_guard<std::mutex> lock(mutex_);
			frame_.swap();
			freeFrame(frame_.back());
			has_new_frame_ = true;
			ofNotifyEvent(Stream<Frame, Wrapper>::onFrameReceived, getFrame(), this);
		}
	}
	const Frame& getFrame() const { return frame_.front(); }
	template<typename Output> void decodeTo(Output &dst) const {
		std::lock_guard<std::mutex> lock(mutex_);
		Stream<Frame, Wrapper>::decodeTo(dst);
	}

	bool isFrameNew() const { return is_frame_new_; }
private:
	bool captureFrame(Frame &frame);
	void freeFrame(Frame &frame);
	
	mutable std::mutex mutex_;
	DoubleBuffer<Frame> frame_;
	uint32_t timeout_ms_=1000;
	bool is_frame_new_=false;
	bool has_new_frame_=false;
};
template<typename Frame>
class FrameSync : public Blocking<Frame, ofxNDIReceiver>
{
public:
	void setup(ofxNDIReceiver &wrapper) {
		Blocking<Frame,ofxNDIReceiver>::setup(wrapper);
		sync_ = wrapper.getFrameSync();
		if(!sync_) {
			sync_ = wrapper.createFrameSync();
		}
	}
	using Blocking<Frame, ofxNDIReceiver>::getFrame;
protected:
	NDIlib_framesync_instance_t sync_;
	virtual bool captureFrame(Frame &frame)=0;
	virtual void freeFrame(Frame &frame)=0;
};

class FrameSyncVideo : public FrameSync<ofxNDI::VideoFrame>
{
public:
	void setFieldType(NDIlib_frame_format_type_e format) { field_type_ = format; }
protected:
	bool captureFrame(ofxNDI::VideoFrame &frame);
	void freeFrame(ofxNDI::VideoFrame &frame);
	NDIlib_frame_format_type_e field_type_=NDIlib_frame_format_type_progressive;
};

class FrameSyncAudio : public FrameSync<ofxNDI::AudioFrame>
{
public:
	void setSampleRate(int sample_rate) { sample_rate_ = sample_rate; }
	void setNumChannels(int num) { num_channels_ = num; }
	void setNumSamples(int num) { num_samples_ = num; }
	int getNumQueuedSamples() const;
protected:
	bool captureFrame(ofxNDI::AudioFrame &frame);
	void freeFrame(ofxNDI::AudioFrame &frame);
	int sample_rate_=0, num_channels_=0, num_samples_=0;
};

}}
using ofxNDIRecvVideoBlocking = ofxNDI::Recv::Blocking<ofxNDI::VideoFrame>;
using ofxNDIRecvVideoThreading = ofxNDI::Recv::Threading<ofxNDI::VideoFrame>;
using ofxNDIRecvVideoFrameSync = ofxNDI::Recv::FrameSyncVideo;
using ofxNDIRecvAudioBlocking = ofxNDI::Recv::Blocking<ofxNDI::AudioFrame>;
using ofxNDIRecvAudioThreading = ofxNDI::Recv::Threading<ofxNDI::AudioFrame>;
using ofxNDIRecvAudioFrameSync = ofxNDI::Recv::FrameSyncAudio;
using ofxNDIRecvMetadata = ofxNDI::Recv::Blocking<ofxNDI::MetadataFrame>;
