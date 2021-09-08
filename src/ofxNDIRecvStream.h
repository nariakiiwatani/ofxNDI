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

template<typename FrameType, typename Wrapper=ofxNDIReceiver>
class Stream
{
public:
	virtual void setup(Wrapper &wrapper) { instance_ = wrapper.getInstance(); }
	virtual void update(){}
	bool isFrameNew() const { return is_frame_new_; }
	virtual const FrameType& getFrame() const=0;
	template<typename Output> void decodeTo(Output &dst) const { Frame::decode(getFrame(), dst); }
	void setTimeout(uint32_t milliseconds) { timeout_ms_ = milliseconds; }

	std::string getMetadata() const { 
		static_assert(!std::is_same<FrameType, ofxNDI::MetadataFrame>::value, "this function is not for ofxNDIRecvMetadata");
		auto ptr = getFrame().p_metadata;
		return ptr ? ptr : "";
	}

	int getQueue() const;
	int64_t getNumReceivedFrame() const;
	int64_t getNumDroppedFrame() const;

	ofEvent<const FrameType> onFrameReceived;

	template<typename Allocator, typename Deallocator>
	void setAllocator(void* p_opaque, Allocator p_allocator, Deallocator p_deallocator);

protected:
	typename Wrapper::Instance instance_;
	bool is_frame_new_=false;
	uint32_t timeout_ms_=1000;

	virtual bool captureFrame(FrameType &frame);
	virtual void freeFrame(FrameType &frame);
};

template<typename FrameType, typename Wrapper=ofxNDIReceiver>
class Blocking : public Stream<FrameType, Wrapper>
{
public:
	void update() override {
		FrameType frame;
		if((is_frame_new_ = captureFrame(frame))) {
			std::swap(frame, frame_);
			ofNotifyEvent(Stream<FrameType, Wrapper>::onFrameReceived, getFrame(), this);
			freeFrame(frame);
		}
	}
	const FrameType& getFrame() const override { return frame_; }
	bool isFrameNew() const { return is_frame_new_; }
protected:
	using Stream<FrameType, Wrapper>::captureFrame;
	using Stream<FrameType, Wrapper>::freeFrame;
	FrameType frame_;
	bool is_frame_new_=false;
};
template<typename FrameType, typename Wrapper=ofxNDIReceiver>
class Threading : public Stream<FrameType, Wrapper>, private ofThread
{
public:
	void setup(Wrapper &wrapper) override {
		Stream<FrameType, Wrapper>::setup(wrapper);
		startThread();
	}
	~Threading() {
		if(isThreadRunning()) {
			waitForThread();
		}
	}
	void update() override {
		std::lock_guard<std::mutex> lock(mutex_);
		is_frame_new_ = has_new_frame_;
		has_new_frame_ = false;
	}
	
	void threadedFunction() override {
		while(isThreadRunning()) {
			updateFrame();
			std::this_thread::yield();
		}
	}
	void updateFrame() {
		if(captureFrame(frame_.back())) {
			std::lock_guard<std::mutex> lock(mutex_);
			frame_.swap();
			has_new_frame_ = true;
			ofNotifyEvent(Stream<FrameType, Wrapper>::onFrameReceived, getFrame(), this);
			freeFrame(frame_.back());
		}
	}
	const FrameType& getFrame() const override { return frame_.front(); }
	template<typename Output> void decodeTo(Output &dst) const {
		std::lock_guard<std::mutex> lock(mutex_);
		Stream<FrameType, Wrapper>::decodeTo(dst);
	}

	bool isFrameNew() const { return is_frame_new_; }
protected:
	mutable std::mutex mutex_;
	DoubleBuffer<FrameType> frame_;
	bool is_frame_new_=false;
	bool has_new_frame_=false;

	using Stream<FrameType, Wrapper>::captureFrame;
	using Stream<FrameType, Wrapper>::freeFrame;
};
template<typename FrameType>
class FrameSync : public Blocking<FrameType, ofxNDIReceiver>
{
public:
	void setup(ofxNDIReceiver &wrapper) override {
		Blocking<FrameType, ofxNDIReceiver>::setup(wrapper);
		sync_ = wrapper.getFrameSync();
		if(!sync_) {
			sync_ = wrapper.createFrameSync();
		}
	}
protected:
	NDIlib_framesync_instance_t sync_;
};

class FrameSyncVideo : public FrameSync<ofxNDI::VideoFrame>
{
public:
	void setFieldType(NDIlib_frame_format_type_e format) { field_type_ = format; }
protected:
	bool captureFrame(ofxNDI::VideoFrame &frame) override;
	void freeFrame(ofxNDI::VideoFrame &frame) override;
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
	bool captureFrame(ofxNDI::AudioFrame &frame) override;
	void freeFrame(ofxNDI::AudioFrame &frame) override;
	int sample_rate_=0, num_channels_=0, num_samples_=0;
};

class AVSyncAudio : public Threading<ofxNDI::AudioFrame>
{
public:
	~AVSyncAudio();
	using SourceFrame = ofxNDI::VideoFrame;
	
	void setup(ofxNDIReceiver &receiver) override;
	void setupSourceStream(Stream<SourceFrame> &source);

protected:
	NDIlib_avsync_instance_t av_sync_;
	void onSourceFrameReceived(const SourceFrame &source);
	std::function<void()> unsubscribe_=[]{};

	mutable std::mutex source_mtx_;
	const SourceFrame *source_frame_;
	bool captureFrame(ofxNDI::AudioFrame &frame) override;
	void freeFrame(ofxNDI::AudioFrame &frame) override;
};


}}
using ofxNDIRecvVideoBlocking = ofxNDI::Recv::Blocking<ofxNDI::VideoFrame>;
using ofxNDIRecvVideoThreading = ofxNDI::Recv::Threading<ofxNDI::VideoFrame>;
using ofxNDIRecvVideoFrameSync = ofxNDI::Recv::FrameSyncVideo;
using ofxNDIRecvAudioBlocking = ofxNDI::Recv::Blocking<ofxNDI::AudioFrame>;
using ofxNDIRecvAudioThreading = ofxNDI::Recv::Threading<ofxNDI::AudioFrame>;
using ofxNDIRecvAudioFrameSync = ofxNDI::Recv::FrameSyncAudio;
using ofxNDIRecvAVSyncAudio = ofxNDI::Recv::AVSyncAudio;
using ofxNDIRecvMetadata = ofxNDI::Recv::Blocking<ofxNDI::MetadataFrame>;
